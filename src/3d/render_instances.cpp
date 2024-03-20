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

void DelayedRendererInstance::update(const real_t &_exp_time, const Transform3D &_transform, const Color &_col, const Color &_custom_col, const SphereBounds &_bounds) {
	ZoneScoped;
	_update(_exp_time, true);

	bounds = _bounds;
	data = InstanceTransformAndData3D(_transform, _col, _custom_col);
}

DelayedRendererLine::DelayedRendererLine() :
		DelayedRenderer() {
	DEV_PRINT_STD("New " NAMEOF(DelayedRendererLine) " created\n");
}

void DelayedRendererLine::update(const real_t &_exp_time, std::unique_ptr<Vector3[]> _lines, const size_t _lines_count, const Color &_col) {
	ZoneScoped;
	_update(_exp_time, true);

	lines = std::move(_lines);
	lines_count = _lines_count;
	bounds = calculate_bounds_based_on_lines();
	color = _col;
}

const Vector3 *DelayedRendererLine::get_lines() const {
	return lines.get();
}

const size_t DelayedRendererLine::get_lines_count() const {
	return lines_count;
}

AABB DelayedRendererLine::calculate_bounds_based_on_lines() {
	ZoneScoped;
	if (lines_count > 0) {
		// Using the original Godot expand_to code to avoid creating new AABB instances
		Vector3 begin = lines.get()[0];
		Vector3 end = lines.get()[0] + Vector3_ZERO;

		for (size_t i = 0; i < lines_count; i++) {
			auto &v = lines.get()[i];
			if (v.x < begin.x) {
				begin.x = v.x;
			}
			if (v.y < begin.y) {
				begin.y = v.y;
			}
			if (v.z < begin.z) {
				begin.z = v.z;
			}

			if (v.x > end.x) {
				end.x = v.x;
			}
			if (v.y > end.y) {
				end.y = v.y;
			}
			if (v.z > end.z) {
				end.z = v.z;
			}
		}

		return AABB(begin, end - begin);
	} else {
		return AABB();
	}
}

void GeometryPool::fill_instance_data(const std::array<Ref<MultiMesh> *, (int)InstanceType::MAX> &t_meshes, const double &delta) {
	ZoneScoped;
	thread_local static auto temp_buffer = temp_raw_buffer();
	size_t max_buffer_size = 0;

	GODOT_STOPWATCH(&time_spent_to_fill_buffers_of_instances);

	for (size_t i = 0; i < t_meshes.size(); i++) {
		ZoneScopedN("Fill iteration");
		auto &mesh = *t_meshes[i];

		size_t buf_size = 0;
		PackedFloat32Array a = get_raw_data((InstanceType)i, temp_buffer, buf_size);

		if (buf_size > max_buffer_size) {
			max_buffer_size = buf_size;
		}

		int new_size = (int)(a.size() / INSTANCE_DATA_FLOAT_COUNT);
		if (mesh->get_instance_count() != new_size) {
			mesh->set_instance_count(new_size);
			mesh->set_visible_instance_count(new_size);
		}

		if (a.size()) {
			// TODO PR for native pointer!
			ZoneScopedN("Set buffer");
			mesh->set_buffer(a);
		}
	}

	temp_buffer.update(max_buffer_size, delta);
}

PackedFloat32Array GeometryPool::get_raw_data(InstanceType _type, temp_raw_buffer &buffer, size_t &out_buffer_size) {
	ZoneScoped;
	size_t last_added = 0;
	size_t buffer_size = 0;

	{
		ZoneScopedN("Prepare buffer");
		for (auto &vp_pool : pools) {
			for (auto &proc : vp_pool.second) {
				auto &inst = proc.instances[(int)_type];
				buffer_size += (inst.used_instant + inst.delayed.size()) * INSTANCE_DATA_FLOAT_COUNT;
			}
		}
		buffer.prepare_buffer(buffer_size);

		out_buffer_size = buffer_size;
	}

	if (buffer_size > 0) {
		ZoneScopedN("Fill buffer");
		auto w = buffer.ptrw();

		for (auto &vp_pool : pools) {
			for (auto &proc : vp_pool.second) {
				auto &inst = proc.instances[(int)_type];

				auto write_instance_data = [&last_added, &w](DelayedRendererInstance &o) {
					size_t id = last_added * INSTANCE_DATA_FLOAT_COUNT;
					last_added++;

					// 7500 instances. 1.2-1.3ms with the old approach and 0.8-0.9ms with the current approach
					memcpy(w + id, reinterpret_cast<real_t *>(&o.data), INSTANCE_DATA_FLOAT_COUNT * sizeof(real_t));
				};

				for (size_t i = 0; i < inst.used_instant; i++) {
					auto &o = inst.instant[i];
					o.is_used_one_time = true;
					if (o.is_visible) {
						write_instance_data(o);
					}
				}
				inst._prev_used_instant = inst.used_instant;

				inst.used_delayed = 0;
				for (size_t i = 0; i < inst.delayed.size(); i++) {
					auto &o = inst.delayed[i];
					if (!o.is_expired()) {
						inst.used_delayed++;
						o.is_used_one_time = true;
						if (o.is_visible) {
							write_instance_data(o);
						}
					}
				}
			}
		}
	} else {
		// Force reset stats
		for (auto &vp_pool : pools) {
			for (auto &proc : vp_pool.second) {
				auto &inst = proc.instances[(int)_type];
				inst._prev_used_instant = inst.used_instant;
				inst.used_delayed = 0;
			}
		}
	}

	return buffer.slice(0, last_added * INSTANCE_DATA_FLOAT_COUNT);
}

void GeometryPool::fill_lines_data(Ref<ArrayMesh> _ig, const double &delta) {
	ZoneScoped;

	uint64_t used_lines = 0;
	for (auto &vp_pool : pools) {
		for (auto &proc : vp_pool.second) {
			used_lines += proc.lines.used_instant;
			used_lines += proc.lines.delayed.size();
		}
	}
	if (used_lines == 0) {
		return;
	}

	GODOT_STOPWATCH(&time_spent_to_fill_buffers_of_lines);

	// Avoiding a large number of resizes increased the speed from 1.9-2.0ms to 1.4-1.5ms
	size_t used_vertexes = 0;

	// pre calculate buffer size
	for (auto &vp_pool : pools) {
		for (auto &proc : vp_pool.second) {
			for (size_t i = 0; i < proc.lines.used_instant; i++) {
				auto &o = proc.lines.instant[i];
				if (o.is_visible) {
					used_vertexes += o.get_lines_count();
				}
			}
			for (size_t i = 0; i < proc.lines.delayed.size(); i++) {
				auto &o = proc.lines.delayed[i];
				if (o.is_visible && !o.is_expired()) {
					used_vertexes += o.get_lines_count();
				}
			}
		}
	}

	thread_local static auto verices_buffer = TempBigBuffer<Vector3, PackedVector3Array, 64 * 1024>();
	thread_local static auto colors_buffer = TempBigBuffer<Color, PackedColorArray, 64 * 1024>();
	verices_buffer.prepare_buffer(used_vertexes);
	colors_buffer.prepare_buffer(used_vertexes);
	verices_buffer.update(used_vertexes, delta);
	colors_buffer.update(used_vertexes, delta);

	size_t prev_pos = 0;
	auto vertexes_write = verices_buffer.ptrw();
	auto colors_write = colors_buffer.ptrw();

	auto append_lines = [&vertexes_write, &colors_write, &prev_pos](const DelayedRendererLine &o) {
		size_t lines_size = o.get_lines_count();

		memcpy(vertexes_write + prev_pos, o.get_lines(), o.get_lines_count() * sizeof(Vector3));
		std::fill(colors_write + prev_pos, colors_write + prev_pos + lines_size, o.color);

		prev_pos += lines_size;
	};

	for (auto &vp_pool : pools) {
		for (auto &proc : vp_pool.second) {
			for (size_t i = 0; i < proc.lines.used_instant; i++) {
				auto &o = proc.lines.instant[i];
				if (o.is_visible) {
					append_lines(o);
				}
				o.is_used_one_time = true;
			}
			proc.lines._prev_used_instant = proc.lines.used_instant;

			proc.lines.used_delayed = 0;
			for (size_t i = 0; i < proc.lines.delayed.size(); i++) {
				auto &o = proc.lines.delayed[i];
				if (!o.is_expired()) {
					proc.lines.used_delayed++;
					if (o.is_visible) {
						append_lines(o);
					}
				}
				o.is_used_one_time = true;
			}
		}
	}

	if (used_vertexes > 1) {
		Array mesh = Array();
		mesh.resize(ArrayMesh::ArrayType::ARRAY_MAX);
		mesh[ArrayMesh::ArrayType::ARRAY_VERTEX] = verices_buffer.slice(0, used_vertexes);
		mesh[ArrayMesh::ArrayType::ARRAY_COLOR] = colors_buffer.slice(0, used_vertexes);

		_ig->add_surface_from_arrays(Mesh::PrimitiveType::PRIMITIVE_LINES, mesh);
	}
}

void GeometryPool::reset_counter(const double &_delta, const ProcessType &p_proc) {
	ZoneScoped;
	if (p_proc == ProcessType::MAX) {
		for (auto &vp_pool : pools) {
			for (auto &proc : vp_pool.second) {
				for (int i = 0; i < (int)InstanceType::MAX; i++) {
					proc.instances[i].reset_counter(_delta, i);
				}
				proc.lines.reset_counter(_delta);
			}
		}
	} else {
		for (auto &vp_pool : pools) {
			for (int i = 0; i < (int)InstanceType::MAX; i++) {
				vp_pool.second[(int)p_proc].instances[i].reset_counter(_delta, i);
			}
			vp_pool.second[(int)p_proc].lines.reset_counter(_delta);
		}
	}
}

void GeometryPool::reset_visible_objects() {
	ZoneScoped;
	for (auto &vp_pool : pools) {
		for (auto &proc : vp_pool.second) {
			for (auto &i : proc.instances) {
				i.reset_visible_counter();
			}
			proc.lines.reset_visible_counter();
		}
	}
}

void GeometryPool::set_stats(Ref<DebugDraw3DStats> &stats) const {
	ZoneScoped;

	struct {
		size_t used_instances = 0;
		size_t used_lines = 0;
		size_t visible_instances = 0;
		size_t visible_lines = 0;
	} counts[(int)ProcessType::MAX];

	for (auto &vp_pool : pools) {
		for (int proc_i = 0; proc_i < (int)ProcessType::MAX; proc_i++) {
			auto &proc = vp_pool.second[proc_i];
			for (auto &i : proc.instances) {
				counts[proc_i].used_instances += i._prev_used_instant;
				counts[proc_i].used_instances += i.used_delayed;
			}

			for (auto &i : proc.instances) {
				counts[proc_i].visible_instances += i.visible_objects;
			}

			counts[proc_i].used_lines += proc.lines._prev_used_instant + proc.lines.used_delayed;
			counts[proc_i].visible_lines += proc.lines.visible_objects;
		}
	}

	const int p = (int)ProcessType::PROCESS;
	const int py = (int)ProcessType::PHYSICS_PROCESS;

	stats->set_render_stats(
			/* t_instances */ counts[p].used_instances,
			/* t_lines */ counts[p].used_lines,
			/* t_visible_instances */ counts[p].visible_instances,
			/* t_visible_lines */ counts[p].visible_lines,

			/* t_instances_phys */ counts[py].used_instances,
			/* t_lines_phys */ counts[py].used_lines,
			/* t_visible_instances_phys */ counts[py].visible_instances,
			/* t_visible_lines_phys */ counts[py].visible_lines,

			/* t_time_filling_buffers_instances_usec */ time_spent_to_fill_buffers_of_instances,
			/* t_time_filling_buffers_lines_usec */ time_spent_to_fill_buffers_of_lines,

			/* t_time_culling_instant_usec */ time_spent_to_cull_instant,
			/* t_time_culling_delayed_usec */ time_spent_to_cull_delayed);
}

void GeometryPool::clear_pool() {
	ZoneScoped;
	for (auto &vp_pool : pools) {
		for (auto &proc : vp_pool.second) {
			for (auto &i : proc.instances) {
				i.clear_pools();
			}
			proc.lines.clear_pools();
		}
	}
}

bool GeometryPool::_is_viewport_empty(Viewport *vp) {
	for (auto &proc : pools[vp]) {
		for (auto &i : proc.instances) {
			if (i.instant.size() || i.delayed.size()) {
				return false;
			}
		}
		if (proc.lines.instant.size() || proc.lines.delayed.size()) {
			return false;
		}
	}
	return true;
}

void GeometryPool::for_each_instance(const std::function<void(Viewport *, DelayedRendererInstance *)> &_func) {
	ZoneScoped;
	for (auto &vp_pool : pools) {
		for (auto &proc : vp_pool.second) {
			for (auto &inst : proc.instances) {
				for (size_t i = 0; i < inst.used_instant; i++) {
					_func(vp_pool.first, &inst.instant[i]);
				}
				for (size_t i = 0; i < inst.delayed.size(); i++) {
					if (!inst.delayed[i].is_expired())
						_func(vp_pool.first, &inst.delayed[i]);
				}
			}
		}
	}
}

void GeometryPool::for_each_line(const std::function<void(Viewport *, DelayedRendererLine *)> &_func) {
	ZoneScoped;
	for (auto &vp_pool : pools) {
		for (auto &proc : vp_pool.second) {
			for (size_t i = 0; i < proc.lines.used_instant; i++) {
				_func(vp_pool.first, &proc.lines.instant[i]);
			}
			for (size_t i = 0; i < proc.lines.delayed.size(); i++) {
				if (!proc.lines.delayed[i].is_expired())
					_func(vp_pool.first, &proc.lines.delayed[i]);
			}
		}
	}
}

void GeometryPool::update_visibility(std::unordered_map<const Viewport *, std::shared_ptr<GeometryPoolCullingData> > &p_culling_data) {
	ZoneScoped;

	uint64_t time_spent_i = 0, time_spent_d = 0;
	for (auto &vp_pool : pools) {
		const auto &vp_culling_data = p_culling_data[vp_pool.first];

		for (auto &proc : vp_pool.second) {
			uint64_t instant_time = 0;
			uint64_t delayed_time = 0;
			for (auto &t : proc.instances) { // loop over instance types
				{
					GODOT_STOPWATCH_ADD(&instant_time);
					for (size_t i = 0; i < t.used_instant; i++)
						t.instant[i].update_visibility(vp_culling_data, true);
				}

				{
					GODOT_STOPWATCH_ADD(&delayed_time);
					for (size_t i = 0; i < t.delayed.size(); i++)
						t.delayed[i].update_visibility(vp_culling_data, false);
				}
			}

			// loop over lines
			uint64_t instant_lines_time = 0;
			uint64_t delayed_lines_time = 0;

			{
				GODOT_STOPWATCH(&instant_lines_time);
				for (size_t i = 0; i < proc.lines.used_instant; i++)
					proc.lines.instant[i].update_visibility(vp_culling_data, true);
			}

			{
				GODOT_STOPWATCH(&delayed_lines_time);
				for (size_t i = 0; i < proc.lines.delayed.size(); i++)
					proc.lines.delayed[i].update_visibility(vp_culling_data, false);
			}

			time_spent_i += instant_lines_time + instant_time;
			time_spent_d += delayed_lines_time + delayed_time;
		}
	}
	time_spent_to_cull_instant = time_spent_i;
	time_spent_to_cull_delayed = time_spent_d;
}

void GeometryPool::update_expiration(const double &_delta, const ProcessType &p_proc) {
	ZoneScoped;
	for (auto &vp_pool : pools) {
		auto &proc = vp_pool.second[(int)p_proc];

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
		} else {
			for (auto &t : proc.instances) {
				for (size_t i = 0; i < t.delayed.size(); i++) {
					t.delayed[i].update_expiration(_delta);
				}
			}

			for (size_t i = 0; i < proc.lines.delayed.size(); i++) {
				proc.lines.delayed[i].update_expiration(_delta);
			}
		}
	}
}

void GeometryPool::scan_visible_instances() {
	ZoneScoped;
	reset_visible_objects();

	for (auto &vp_pool : pools) {
		for (auto &proc : vp_pool.second) {
			for (auto &t : proc.instances) {
				for (size_t i = 0; i < t.used_instant; i++)
					if (t.instant[i].is_visible)
						t.visible_objects++;
				for (size_t i = 0; i < t.delayed.size(); i++) {
					auto &o = t.delayed[i];
					if (o.is_visible && !o.is_expired())
						t.visible_objects++;
				}
			}

			for (size_t i = 0; i < proc.lines.used_instant; i++)
				if (proc.lines.instant[i].is_visible)
					proc.lines.visible_objects++;

			for (size_t i = 0; i < proc.lines.delayed.size(); i++) {
				auto &o = proc.lines.delayed[i];
				if (o.is_visible && !o.is_expired())
					proc.lines.visible_objects++;
			}
		}
	}
}

std::vector<Viewport *> GeometryPool::get_and_validate_viewports() {
	std::vector<Viewport *> res;
	std::vector<Viewport *> to_delete;

	for (const auto &vp : viewport_ids) {
		if (UtilityFunctions::is_instance_id_valid(vp.second)) {
			if (_is_viewport_empty(vp.first)) {
				DEV_PRINT_STD("Viewport (%s) did not contain any debug data, it will be deleted from the World3D's container.\n", vp.first->to_string().utf8());
				to_delete.push_back(vp.first);
			} else {
				res.push_back(vp.first);
			}
		} else {
			to_delete.push_back(vp.first);
		}
	}

	for (const auto &vp : to_delete) {
		viewport_ids.erase(vp);
		pools.erase(vp);
	}

	return res;
}

void GeometryPool::add_or_update_instance(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg, ConvertableInstanceType _type, const real_t &_exp_time, const ProcessType &p_proc, const Transform3D &_transform, const Color &_col, const SphereBounds &_bounds, const Color *p_custom_col, const std::function<void(DelayedRendererInstance *)> &_custom_upd) {
	ZoneScoped;
	add_or_update_instance(cfg, _scoped_config_type_convert(_type, cfg), _exp_time, p_proc, _transform, _col, _bounds, p_custom_col, _custom_upd);
}

void GeometryPool::add_or_update_instance(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg, InstanceType _type, const real_t &_exp_time, const ProcessType &p_proc, const Transform3D &_transform, const Color &_col, const SphereBounds &_bounds, const Color *p_custom_col, const std::function<void(DelayedRendererInstance *)> &_custom_upd) {
	ZoneScoped;
	DelayedRendererInstance *inst = pools[cfg->viewport][(int)p_proc].instances[(int)_type].get(_exp_time > 0);
	viewport_ids[cfg->viewport] = cfg->viewport->get_instance_id();

	inst->update(_exp_time, _transform, _col, p_custom_col ? *p_custom_col : _scoped_config_to_custom(cfg), _bounds);
	if (_custom_upd)
		_custom_upd(inst);
}

void GeometryPool::add_or_update_line(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg, const real_t &_exp_time, const ProcessType &p_proc, std::unique_ptr<Vector3[]> _lines, const size_t _line_count, const Color &_col, const std::function<void(DelayedRendererLine *)> _custom_upd) {
	ZoneScoped;
	DelayedRendererLine *inst = pools[cfg->viewport][(int)p_proc].lines.get(_exp_time > 0);
	viewport_ids[cfg->viewport] = cfg->viewport->get_instance_id();

	inst->update(_exp_time, std::move(_lines), _line_count, _col);
	if (_custom_upd)
		_custom_upd(inst);
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