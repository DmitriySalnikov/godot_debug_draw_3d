#include "render_instances.h"

#ifndef DISABLE_DEBUG_RENDERING

#include "stats_3d.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/time.hpp>
GODOT_WARNING_RESTORE()

DelayedRendererInstance::DelayedRendererInstance() :
		DelayedRenderer() {
	DEV_PRINT_STD("New " NAMEOF(DelayedRendererInstance) " created\n");
}

DelayedRendererLine::DelayedRendererLine() :
		DelayedRenderer(),
		lines_count(0) {
	DEV_PRINT_STD("New " NAMEOF(DelayedRendererLine) " created\n");
}

void GeometryPool::fill_instance_data(const std::vector<Ref<MultiMesh> *> &p_meshes, const std::shared_ptr<GeometryPoolCullingData> &p_culling_data, const double &delta) {
	ZoneScoped;

	// reset timers
	time_spent_to_cull_instances = 0;
	time_spent_to_fill_buffers_of_instances = 0;

	for (auto &proc : pools) {
		// Update viewport timers
		for (auto &vp : proc.viewports) {
			vp.second.is_phys_used = true;
		}
	}

	for (int type = 0; type < (int)InstanceType::MAX; type++) {
		ZoneScopedN("Fill iteration");
		GODOT_STOPWATCH_ADD(&time_spent_to_fill_buffers_of_instances);

		size_t max_instances = 0;
		{
			ZoneScopedN("Update visibility");
			GODOT_STOPWATCH_ADD(&time_spent_to_cull_instances);

			for (auto &proc : pools) {
				auto &itype = proc.instances[type];

				auto &inst_arr = itype.instant;
				for (int i = 0; i < itype.used_instant; i++) {
					if (inst_arr[i].update_visibility(p_culling_data)) {
						max_instances++;
					}
				}

				itype.used_delayed = 0;
				for (auto &inst : itype.delayed) {
					if (!inst.is_expired()) {
						inst.is_used_one_time = true;
						itype.used_delayed++;

						if (inst.update_visibility(p_culling_data)) {
							max_instances++;
						}
					}
				}
			}

			stat_visible_instances += max_instances;
		}

		PackedFloat32Array &buffer = temp_instances_buffers[type];
		size_t used_buffer_size = max_instances * INSTANCE_DATA_FLOAT_COUNT;

		{
			ZoneScopedN("Prepare buffer");
			ZoneValue(buffer.size());

			if ((int64_t)used_buffer_size > buffer.size()) {
				ZoneScopedN("Resize buffer (grew)");
				ZoneValue(used_buffer_size);
				buffer.resize(used_buffer_size);
			}

			// shrink the buffer only if half of it is required.
			if ((int64_t)used_buffer_size < (int64_t)ceil(buffer.size() * 0.5)) {
				ZoneScopedN("Resize buffer (shrink)");
				ZoneValue(used_buffer_size);
				buffer.resize(used_buffer_size);
			}
		}

		size_t last_added = 0;
		{
			ZoneScopedN("Fill buffer");
			auto w = buffer.ptrw();

			for (auto &proc : pools) {
				auto &itype = proc.instances[type];

				auto &inst_arr = itype.instant;
				for (int i = 0; i < itype.used_instant; i++) {
					auto &inst = inst_arr[i];

					if (inst.is_visible) {
						memcpy(w + last_added * INSTANCE_DATA_FLOAT_COUNT, reinterpret_cast<const real_t *>(&inst.data), INSTANCE_DATA_FLOAT_COUNT * sizeof(real_t));
						last_added++;
					}
				}

				for (auto &inst : itype.delayed) {
					if (!inst.is_expired()) {
						if (inst.is_visible) {
							memcpy(w + last_added * INSTANCE_DATA_FLOAT_COUNT, reinterpret_cast<const real_t *>(&inst.data), INSTANCE_DATA_FLOAT_COUNT * sizeof(real_t));
							last_added++;
						}
					}
				}
			}
		}

		// resize if the buffer size has changed.
		auto &mesh = *p_meshes[type];
		int32_t new_inst_count = (int)(buffer.size() / INSTANCE_DATA_FLOAT_COUNT);
		if (new_inst_count != mesh->get_instance_count()) {
			ZoneScopedN("Changing amount of instances");
			ZoneValue(new_inst_count);
			mesh->set_instance_count(new_inst_count);
		}

		// just change the visible instances instead of resizing the entire buffer.
		{
			int32_t new_visible_count = (int32_t)used_buffer_size / INSTANCE_DATA_FLOAT_COUNT;
			ZoneScopedN("Set visible instances");
			ZoneValue(new_visible_count);
			mesh->set_visible_instance_count(new_visible_count);
		}

		if (buffer.size()) {
			// TODO PR for native pointer!
			ZoneScopedN("Set buffer");
			mesh->set_buffer(buffer);
		}
	}

	time_spent_to_fill_buffers_of_instances -= time_spent_to_cull_instances;
}

void GeometryPool::fill_lines_data(Ref<ArrayMesh> _ig, const std::shared_ptr<GeometryPoolCullingData> &p_culling_data, const double &delta) {
	ZoneScoped;

	uint64_t used_lines = 0;
	for (auto &proc : pools) {
		used_lines += proc.lines.used_instant;
		used_lines += proc.lines.delayed.size();
	}
	if (used_lines == 0) {
		return;
	}

	GODOT_STOPWATCH(&time_spent_to_fill_buffers_of_lines);

	// Avoiding a large number of resizes increased the speed from 1.9-2.0ms to 1.4-1.5ms
	size_t used_vertexes = 0;

	PackedVector3Array vertexes;
	PackedColorArray colors;

	{
		ZoneScopedN("Prepare buffers");

		// pre calculate buffer size
		for (auto &proc : pools) {
			for (size_t i = 0; i < proc.lines.used_instant; i++) {
				auto &o = proc.lines.instant[i];
				if (o.update_visibility(p_culling_data)) {
					used_vertexes += o.lines_count;
				}
			}
			for (size_t i = 0; i < proc.lines.delayed.size(); i++) {
				auto &o = proc.lines.delayed[i];
				if (o.update_visibility(p_culling_data) && !o.is_expired()) {
					used_vertexes += o.lines_count;
				}
			}
		}
		ZoneValue(used_vertexes);

		vertexes.resize(used_vertexes);
		colors.resize(used_vertexes);
	}

	size_t prev_pos = 0;
	auto vertexes_write = vertexes.ptrw();
	auto colors_write = colors.ptrw();

#define WRITE_DATA(_inst)                                                                      \
	size_t lines_size = _inst.lines_count;                                                     \
	memcpy(vertexes_write + prev_pos, _inst.lines.get(), _inst.lines_count * sizeof(Vector3)); \
	std::fill(colors_write + prev_pos, colors_write + prev_pos + lines_size, _inst.color);     \
	prev_pos += lines_size;

	for (auto &proc : pools) {
		for (size_t i = 0; i < proc.lines.used_instant; i++) {
			auto &o = proc.lines.instant[i];
			if (o.is_visible) {
				WRITE_DATA(o);
				stat_visible_lines++;
			}
			o.is_used_one_time = true;
		}

		proc.lines.used_delayed = 0;
		for (size_t i = 0; i < proc.lines.delayed.size(); i++) {
			auto &o = proc.lines.delayed[i];
			if (!o.is_expired()) {
				proc.lines.used_delayed++;
				if (o.is_visible) {
					WRITE_DATA(o);
					stat_visible_lines++;
				}
			}
			o.is_used_one_time = true;
		}
	}
#undef WRITE_DATA

	if (used_vertexes > 1) {
		ZoneScopedN("Set mesh arrays");

		Array mesh = Array();
		mesh.resize(ArrayMesh::ArrayType::ARRAY_MAX);
		mesh[ArrayMesh::ArrayType::ARRAY_VERTEX] = vertexes;
		mesh[ArrayMesh::ArrayType::ARRAY_COLOR] = colors;

		_ig->add_surface_from_arrays(Mesh::PrimitiveType::PRIMITIVE_LINES, mesh);
	}
}

void GeometryPool::reset_counter(const double &_delta, const ProcessType &p_proc) {
	ZoneScoped;
	if (p_proc == ProcessType::MAX) {
		for (auto &proc : pools) {
			for (int i = 0; i < (int)InstanceType::MAX; i++) {
				proc.instances[i].reset_counter(_delta, i);
			}
			proc.lines.reset_counter(_delta);
		}
	} else {
		auto &proc = pools[(int)p_proc];
		for (int i = 0; i < (int)InstanceType::MAX; i++) {
			proc.instances[i].reset_counter(_delta, i);
		}
		proc.lines.reset_counter(_delta);
	}
}

void GeometryPool::reset_visible_objects() {
	ZoneScoped;
	stat_visible_instances = 0;
	stat_visible_lines = 0;
}

void GeometryPool::set_stats(Ref<DebugDraw3DStats> &stats) const {
	ZoneScoped;

	struct {
		size_t used_instances = 0;
		size_t used_lines = 0;
	} counts[(int)ProcessType::MAX];

	for (int proc_i = 0; proc_i < (int)ProcessType::MAX; proc_i++) {
		auto &proc = pools[proc_i];
		for (auto &i : proc.instances) {
			counts[proc_i].used_instances += i._prev_used_instant;
			counts[proc_i].used_instances += i.used_delayed;
		}

		counts[proc_i].used_lines += proc.lines._prev_used_instant + proc.lines.used_delayed;
	}

	const int p = (int)ProcessType::PROCESS;
	const int py = (int)ProcessType::PHYSICS_PROCESS;

	stats->set_render_stats(
			/* t_instances */ counts[p].used_instances,
			/* t_lines */ counts[p].used_lines,
			/* t_visible_instances */ stat_visible_instances,
			/* t_visible_lines */ stat_visible_lines,

			/* t_instances_phys */ counts[py].used_instances,
			/* t_lines_phys */ counts[py].used_lines,

			/* t_time_filling_buffers_instances_usec */ time_spent_to_fill_buffers_of_instances,
			/* t_time_filling_buffers_lines_usec */ time_spent_to_fill_buffers_of_lines,

			/* t_time_culling_instances_usec */ time_spent_to_cull_instances,
			/* t_time_culling_lines_usec */ time_spent_to_cull_lines);
}

void GeometryPool::clear_pool() {
	ZoneScoped;
	for (auto &proc : pools) {
		for (auto &i : proc.instances) {
			i.clear_pools();
		}
		proc.lines.clear_pools();
	}
}

void GeometryPool::for_each_instance(const std::function<void(DelayedRendererInstance *)> &_func) {
	ZoneScoped;
	for (auto &proc : pools) {
		for (auto &inst : proc.instances) {
			for (size_t i = 0; i < inst.used_instant; i++) {
				_func(&inst.instant[i]);
			}
			for (size_t i = 0; i < inst.delayed.size(); i++) {
				if (!inst.delayed[i].is_expired())
					_func(&inst.delayed[i]);
			}
		}
	}
}

void GeometryPool::for_each_line(const std::function<void(DelayedRendererLine *)> &_func) {
	ZoneScoped;
	for (auto &proc : pools) {
		for (size_t i = 0; i < proc.lines.used_instant; i++) {
			_func(&proc.lines.instant[i]);
		}
		for (size_t i = 0; i < proc.lines.delayed.size(); i++) {
			if (!proc.lines.delayed[i].is_expired())
				_func(&proc.lines.delayed[i]);
		}
	}
}

void GeometryPool::update_expiration(const double &_delta, const ProcessType &p_proc) {
	ZoneScoped;

	auto &proc = pools[(int)p_proc];
	if (p_proc == ProcessType::PHYSICS_PROCESS) {
		for (auto &t : proc.instances) {
			for (size_t i = 0; i < t.delayed.size(); i++) {
				auto &o = t.delayed[i];
				if (o.is_used_one_time) {
					o.update_expiration(_delta);
				}
			}
		}

		for (size_t i = 0; i < proc.lines.delayed.size(); i++) {
			auto &o = proc.lines.delayed[i];
			if (o.is_used_one_time) {
				o.update_expiration(_delta);
			}
		}

		for (auto &vp : proc.viewports) {
			if (vp.second.is_phys_used) {
				vp.second.exp_time -= _delta;
			}
		}
	} else {
		for (auto &t : proc.instances) {
			for (size_t i = 0; i < t.delayed.size(); i++) {
				t.delayed[i].update_expiration(_delta);
			}
		}

		for (size_t i = 0; i < proc.lines.delayed.size(); i++) {
			proc.lines.delayed[i].update_expiration(_delta);
		}

		for (auto &vp : proc.viewports) {
			vp.second.exp_time -= _delta;
		}
	}
}

std::unordered_set<Viewport *> GeometryPool::get_and_validate_viewports() {
	ZoneScoped;
	std::unordered_set<Viewport *> res;

	for (int i = 0; i < (int)ProcessType::MAX; i++) {
		auto &proc = pools[(int)i];
		std::vector<Viewport *> to_delete;

		for (const auto &vp : proc.viewports) {
			if (UtilityFunctions::is_instance_id_valid(vp.second.vp_id)) {
				if (vp.second.exp_time <= 0) {
					DEV_PRINT_STD("Viewport (%s) timer has expired, it will be deleted from the World3D's container (%s).\n", vp.first->to_string().utf8().get_data(), i == (int)ProcessType::PHYSICS_PROCESS ? "Physics" : "Normal");
					to_delete.push_back(vp.first);
				} else {
					res.insert(vp.first);
				}
			} else {
				to_delete.push_back(vp.first);
			}
		}

		for (const auto &vp : to_delete) {
			proc.viewports.erase(vp);
		}
	}

	return res;
}

void GeometryPool::add_or_update_instance(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg, ConvertableInstanceType _type, const real_t &_exp_time, const ProcessType &p_proc, const Transform3D &_transform, const Color &_col, const SphereBounds &_bounds, const Color *p_custom_col) {
	ZoneScoped;
	add_or_update_instance(cfg, _scoped_config_type_convert(_type, cfg), _exp_time, p_proc, _transform, _col, _bounds, p_custom_col);
}

void GeometryPool::add_or_update_instance(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg, InstanceType _type, const real_t &_exp_time, const ProcessType &p_proc, const Transform3D &_transform, const Color &_col, const SphereBounds &_bounds, const Color *p_custom_col) {
	ZoneScoped;
	auto &proc = pools[(int)p_proc];
	DelayedRendererInstance *inst = proc.instances[(int)_type].get(_exp_time > 0);

	inst->data = GeometryPoolData3DInstance(_transform, _col, p_custom_col ? *p_custom_col : _scoped_config_to_custom(cfg));
	inst->bounds = _bounds;
	inst->expiration_time = _exp_time;
	inst->is_used_one_time = false;
	inst->is_visible = true;

	auto &vp_data = proc.viewports[cfg->viewport];
	vp_data.vp_id = cfg->viewport->get_instance_id();
	if (vp_data.exp_time < _exp_time + 1) {
		vp_data.exp_time = _exp_time + 1;
		vp_data.is_phys_used = false;
	}
}

void GeometryPool::add_or_update_line(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg, const real_t &_exp_time, const ProcessType &p_proc, std::unique_ptr<Vector3[]> _lines, const size_t _line_count, const Color &_col) {
	ZoneScoped;
	auto &proc = pools[(int)p_proc];
	DelayedRendererLine *inst = proc.lines.get(_exp_time > 0);

	inst->lines = std::move(_lines);
	inst->lines_count = _line_count;
	inst->color = _col;
	inst->bounds = MathUtils::calculate_vertex_bounds(inst->lines.get(), _line_count);
	inst->expiration_time = _exp_time;
	inst->is_used_one_time = false;
	inst->is_visible = true;

	auto &vp_data = proc.viewports[cfg->viewport];
	vp_data.vp_id = cfg->viewport->get_instance_id();
	if (vp_data.exp_time < _exp_time + 1) {
		vp_data.exp_time = _exp_time + 1;
		vp_data.is_phys_used = false;
	}
}

GeometryType GeometryPool::_scoped_config_get_geometry_type(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg) {
	ZoneScoped;
	if (cfg->thickness != 0) {
		return GeometryType::Volumetric;
	}
	// TODO solid
	return GeometryType::Wireframe;
}

Color GeometryPool::_scoped_config_to_custom(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg) {
	ZoneScoped;
	if (_scoped_config_get_geometry_type(cfg) == GeometryType::Volumetric)
		return Color(cfg->thickness, cfg->center_brightness, 0, 0);

	return Color();
}

InstanceType GeometryPool::_scoped_config_type_convert(ConvertableInstanceType type, const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg) {
	ZoneScoped;
	switch (_scoped_config_get_geometry_type(cfg)) {
		case GeometryType::Wireframe: {
			switch (type) {
				case ConvertableInstanceType::CUBE:
					return InstanceType::CUBE;
				case ConvertableInstanceType::CUBE_CENTERED:
					return InstanceType::CUBE_CENTERED;
				case ConvertableInstanceType::ARROWHEAD:
					return InstanceType::ARROWHEAD;
				case ConvertableInstanceType::POSITION:
					return InstanceType::POSITION;
				case ConvertableInstanceType::SPHERE:
					if (cfg->hd_sphere) {
						return InstanceType::SPHERE_HD;
					} else {
						return InstanceType::SPHERE;
					}
				case ConvertableInstanceType::CYLINDER:
					return InstanceType::CYLINDER;
				case ConvertableInstanceType::CYLINDER_AB:
					return InstanceType::CYLINDER_AB;
				default:
					break;
			}
			break;
		}
		case GeometryType::Volumetric: {
			switch (type) {
				case ConvertableInstanceType::CUBE:
					return InstanceType::CUBE_VOLUMETRIC;
				case ConvertableInstanceType::CUBE_CENTERED:
					return InstanceType::CUBE_CENTERED_VOLUMETRIC;
				case ConvertableInstanceType::ARROWHEAD:
					return InstanceType::ARROWHEAD_VOLUMETRIC;
				case ConvertableInstanceType::POSITION:
					return InstanceType::POSITION_VOLUMETRIC;
				case ConvertableInstanceType::SPHERE:
					if (cfg->hd_sphere) {
						return InstanceType::SPHERE_HD_VOLUMETRIC;
					} else {
						return InstanceType::SPHERE_VOLUMETRIC;
					}
					return InstanceType::SPHERE_VOLUMETRIC;
				case ConvertableInstanceType::CYLINDER:
					return InstanceType::CYLINDER_VOLUMETRIC;
				case ConvertableInstanceType::CYLINDER_AB:
					return InstanceType::CYLINDER_AB_VOLUMETRIC;
				default:
					break;
			}
			break;
		}
		default:
			break;
	}

	// Crash here...
	return InstanceType(-1);
}

#endif