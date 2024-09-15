#include "render_instances.h"

#ifndef DISABLE_DEBUG_RENDERING

#include "stats_3d.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
GODOT_WARNING_RESTORE()

bool DelayedRenderer::update_visibility(const std::shared_ptr<GeometryPoolCullingData> &p_culling_data) {
	is_visible = false;
	for (auto &box : p_culling_data->m_frustum_boxes) {
		if (box.intersects(bounds)) {
			goto frustum;
		}
	}
	return is_visible;
frustum:
	if (p_culling_data->m_frustums.size()) {
		for (auto &frustum : p_culling_data->m_frustums) {
			if (MathUtils::is_bounds_partially_inside_convex_shape(bounds, frustum)) {
				return is_visible = true;
			}
		}
		return is_visible = false;
	} else {
		return is_visible = true;
	}
}

DelayedRendererInstance::DelayedRendererInstance() :
		DelayedRenderer() {
	DEV_PRINT_STD("New %s created\n", NAMEOF(DelayedRendererInstance));
}

DelayedRendererLine::DelayedRendererLine() :
		DelayedRenderer(),
		lines_count(0) {
	DEV_PRINT_STD("New %s created\n", NAMEOF(DelayedRendererLine));
}

void GeometryPool::fill_mesh_data(const std::vector<Ref<MultiMesh> *> &p_meshes, Ref<ArrayMesh> p_ig, std::unordered_map<Viewport *, std::shared_ptr<GeometryPoolCullingData> > &p_culling_data) {
	ZoneScoped;
	fill_instance_data(p_meshes, p_culling_data);
	fill_lines_data(p_ig, p_culling_data);

	process_delta_sum = 0;
	physics_delta_sum = 0;
}

void GeometryPool::fill_instance_data(const std::vector<Ref<MultiMesh> *> &p_meshes, std::unordered_map<Viewport *, std::shared_ptr<GeometryPoolCullingData> > &p_culling_data) {
	ZoneScoped;

	constexpr size_t INSTANCE_DATA_FLOAT_COUNT = ((sizeof(float) * 3 /*3 components*/ * 4 /*4 vectors3*/ + sizeof(godot::Color) /*Instance Color*/ + sizeof(godot::Color) /*Custom Data*/) / sizeof(float));

	// reset timers
	time_spent_to_cull_instances = 0;
	time_spent_to_fill_buffers_of_instances = 0;

	for (int type = 0; type < (int)InstanceType::MAX; type++) {
		ZoneScopedN("Fill iteration");
		ZoneValue(type);
		GODOT_STOPWATCH_ADD(&time_spent_to_fill_buffers_of_instances);

		std::vector<DelayedRendererInstance *> visible_buffer;
		visible_buffer.reserve(prev_buffer_visible_instance_count[type]);

		{
			ZoneScopedN("Update visibility and expiration");

			for (auto &vp_pool : pools) {
				GODOT_STOPWATCH_ADD(&time_spent_to_cull_instances);
				auto &culling_data = p_culling_data[vp_pool.first];

				for (int proc_i = 0; proc_i < (int)ProcessType::MAX; proc_i++) {
					auto &itype = vp_pool.second[proc_i].instances[type];

					auto &inst_arr = itype.instant;
					for (int i = 0; i < itype.used_instant; i++) {
						auto &inst = inst_arr[i];
						if (inst.update_visibility(culling_data)) {
							visible_buffer.push_back(&inst);
						}
					}

					itype.used_delayed = 0;
					if (proc_i == (int)ProcessType::PHYSICS_PROCESS) {
						for (auto &inst : itype.delayed) {
							if (!inst.is_expired()) {
								if (inst.is_used_one_time) {
									inst.expiration_time -= physics_delta_sum;
								}
								inst.is_used_one_time = true;
								itype.used_delayed++;

								if (inst.update_visibility(culling_data)) {
									visible_buffer.push_back(&inst);
								}
							}
						}
					} else {
						for (auto &inst : itype.delayed) {
							if (!inst.is_expired()) {
								inst.expiration_time -= process_delta_sum;
								inst.is_used_one_time = true;
								itype.used_delayed++;

								if (inst.update_visibility(culling_data)) {
									visible_buffer.push_back(&inst);
								}
							}
						}
					}
				}

				stat_visible_instances += visible_buffer.size();
				prev_buffer_visible_instance_count[type] = visible_buffer.size();
			}
		}

		PackedFloat32Array &buffer = temp_instances_buffers[type];
		size_t used_buffer_size = visible_buffer.size() * INSTANCE_DATA_FLOAT_COUNT;

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
			ZoneValue(visible_buffer.size());
			auto w = buffer.ptrw();

			for (auto &inst : visible_buffer) {
				memcpy(w + last_added++ * INSTANCE_DATA_FLOAT_COUNT, reinterpret_cast<const float *>(&inst->data), INSTANCE_DATA_FLOAT_COUNT * sizeof(float));
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
			ZoneScopedN("Set buffer");
			mesh->set_buffer(buffer);
		}
	}

	time_spent_to_fill_buffers_of_instances -= time_spent_to_cull_instances;
}

void GeometryPool::fill_lines_data(Ref<ArrayMesh> p_ig, std::unordered_map<Viewport *, std::shared_ptr<GeometryPoolCullingData> > &p_culling_data) {
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

	size_t used_vertexes = 0;

	PackedVector3Array vertexes;
	PackedColorArray colors;

	std::vector<DelayedRendererLine *> visible_buffer;

	{
		ZoneScopedN("Prepare buffers");
		visible_buffer.reserve(prev_buffer_visible_lines_count);

		{
			ZoneScopedN("Update visibility and expiration");
			GODOT_STOPWATCH(&time_spent_to_cull_lines);

			// pre calculate buffer size

			for (auto &vp_pool : pools) {
				auto &culling_data = p_culling_data[vp_pool.first];

				for (int proc_i = 0; proc_i < (int)ProcessType::MAX; proc_i++) {
					auto &proc = vp_pool.second[proc_i];

					for (size_t i = 0; i < proc.lines.used_instant; i++) {
						auto &o = proc.lines.instant[i];
						if (o.update_visibility(culling_data)) {
							o.is_used_one_time = true;
							used_vertexes += o.lines_count;
							visible_buffer.push_back(&o);
						}
					}

					proc.lines.used_delayed = 0;
					if (proc_i == (int)ProcessType::PHYSICS_PROCESS) {
						for (auto &o : proc.lines.delayed) {
							if (!o.is_expired()) {
								if (o.is_used_one_time) {
									o.expiration_time -= physics_delta_sum;
								}
								o.is_used_one_time = true;
								proc.lines.used_delayed++;

								if (o.update_visibility(culling_data)) {
									used_vertexes += o.lines_count;
									visible_buffer.push_back(&o);
								}
							}
						}
					} else {
						for (auto &o : proc.lines.delayed) {
							if (!o.is_expired()) {
								o.expiration_time -= process_delta_sum;
								o.is_used_one_time = true;
								proc.lines.used_delayed++;

								if (o.update_visibility(culling_data)) {
									used_vertexes += o.lines_count;
									visible_buffer.push_back(&o);
								}
							}
						}
					}
				}
			}
		}

		stat_visible_lines = visible_buffer.size();
		prev_buffer_visible_lines_count = visible_buffer.size();

		ZoneValue(used_vertexes);
		vertexes.resize(used_vertexes);
		colors.resize(used_vertexes);
	}

	size_t prev_pos = 0;
	auto vertexes_write = vertexes.ptrw();
	auto colors_write = colors.ptrw();

	{
		ZoneScopedN("Fill buffers");
		ZoneValue(visible_buffer.size());

		for (const auto &o : visible_buffer) {
			size_t lines_size = o->lines_count;
			memcpy(vertexes_write + prev_pos, o->lines.get(), o->lines_count * sizeof(Vector3));
			std::fill(colors_write + prev_pos, colors_write + prev_pos + lines_size, o->color);
			prev_pos += lines_size;
		}
	}

	if (used_vertexes > 1) {
		ZoneScopedN("Set mesh arrays");

		Array mesh = Array();
		mesh.resize(ArrayMesh::ArrayType::ARRAY_MAX);
		mesh[ArrayMesh::ArrayType::ARRAY_VERTEX] = vertexes;
		mesh[ArrayMesh::ArrayType::ARRAY_COLOR] = colors;

		p_ig->add_surface_from_arrays(Mesh::PrimitiveType::PRIMITIVE_LINES, mesh);
	}

	time_spent_to_fill_buffers_of_lines -= time_spent_to_cull_lines;
}

void GeometryPool::reset_counter(const double &p_delta, const ProcessType &p_proc) {
	ZoneScoped;
	if (p_proc == ProcessType::MAX) {
		for (auto &vp_pool : pools) {
			for (auto &proc : vp_pool.second) {
				for (int i = 0; i < (int)InstanceType::MAX; i++) {
					proc.instances[i].reset_counter(p_delta, i);
				}
				proc.lines.reset_counter(p_delta);
			}
		}
	} else {
		for (auto &vp_pool : pools) {
			auto &proc = vp_pool.second[(int)p_proc];
			for (int i = 0; i < (int)InstanceType::MAX; i++) {
				proc.instances[i].reset_counter(p_delta, i);
			}
			proc.lines.reset_counter(p_delta);
		}
	}
}

void GeometryPool::reset_visible_objects() {
	ZoneScoped;
	stat_visible_instances = 0;
	stat_visible_lines = 0;
}

void GeometryPool::set_stats(Ref<DebugDraw3DStats> &p_stats) const {
	ZoneScoped;

	struct {
		size_t used_instances = 0;
		size_t used_lines = 0;
	} counts[(int)ProcessType::MAX];

	for (auto &vp_pool : pools) {
		for (int proc_i = 0; proc_i < (int)ProcessType::MAX; proc_i++) {
			auto &proc = vp_pool.second[proc_i];
			for (auto &i : proc.instances) {
				counts[proc_i].used_instances += i._prev_used_instant;
				counts[proc_i].used_instances += i.used_delayed;
			}

			counts[proc_i].used_lines += proc.lines._prev_used_instant + proc.lines.used_delayed;
		}
	}

	const int p = (int)ProcessType::PROCESS;
	const int py = (int)ProcessType::PHYSICS_PROCESS;

	p_stats->set_render_stats(
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
	for (auto &vp_pool : pools) {
		for (auto &proc : vp_pool.second) {
			for (auto &i : proc.instances) {
				i.clear_pools();
			}
			proc.lines.clear_pools();
		}
	}
}

void GeometryPool::for_each_instance(const std::function<void(DelayedRendererInstance *)> &p_func) {
	ZoneScoped;
	for (auto &vp_pool : pools) {
		for (auto &proc : vp_pool.second) {
			for (auto &inst : proc.instances) {
				for (size_t i = 0; i < inst.used_instant; i++) {
					p_func(&inst.instant[i]);
				}
				for (size_t i = 0; i < inst.delayed.size(); i++) {
					if (!inst.delayed[i].is_expired())
						p_func(&inst.delayed[i]);
				}
			}
		}
	}
}

void GeometryPool::for_each_line(const std::function<void(DelayedRendererLine *)> &p_func) {
	ZoneScoped;
	for (auto &vp_pool : pools) {
		for (auto &proc : vp_pool.second) {
			for (size_t i = 0; i < proc.lines.used_instant; i++) {
				p_func(&proc.lines.instant[i]);
			}
			for (size_t i = 0; i < proc.lines.delayed.size(); i++) {
				if (!proc.lines.delayed[i].is_expired())
					p_func(&proc.lines.delayed[i]);
			}
		}
	}
}

void GeometryPool::update_expiration_delta(const double &p_delta, const ProcessType &p_proc) {
	ZoneScoped;

	if (p_proc == ProcessType::PHYSICS_PROCESS) {
		physics_delta_sum += p_delta;
	} else {
		process_delta_sum += p_delta;
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

void GeometryPool::set_no_depth_test_info(bool p_no_depth_test) {
	is_no_depth_test = p_no_depth_test;
}

std::vector<Viewport *> GeometryPool::get_and_validate_viewports() {
	ZoneScoped;
	std::vector<Viewport *> res;
	std::vector<Viewport *> to_delete;

	for (const auto &vp : viewport_ids) {
		if (UtilityFunctions::is_instance_id_valid(vp.second)) {
			if (_is_viewport_empty(vp.first)) {
				DEV_PRINT_STD("%s Viewport (%s) did not contain any debug data,\n\tit will be deleted from the World3D's container.\n", is_no_depth_test ? "NoDepth" : "Normal", vp.first->to_string().utf8().get_data());
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

void GeometryPool::add_or_update_instance(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &p_cfg, ConvertableInstanceType p_type, const real_t &p_exp_time, const ProcessType &p_proc, const Transform3D &p_transform, const Color &p_col, const SphereBounds &p_bounds, const Color *p_custom_col) {
	ZoneScoped;
	add_or_update_instance(p_cfg, _scoped_config_type_convert(p_type, p_cfg), p_exp_time, p_proc, p_transform, p_col, p_bounds, p_custom_col);
}

void GeometryPool::add_or_update_instance(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &p_cfg, InstanceType p_type, const real_t &p_exp_time, const ProcessType &p_proc, const Transform3D &p_transform, const Color &p_col, const SphereBounds &p_bounds, const Color *p_custom_col) {
	ZoneScoped;
	auto &proc = pools[p_cfg->dcd.viewport][(int)p_proc];
	DelayedRendererInstance *inst = proc.instances[(int)p_type].get(p_exp_time > 0);
	viewport_ids[p_cfg->dcd.viewport] = p_cfg->dcd.viewport->get_instance_id();

	SphereBounds thick_sphere = p_bounds;
	thick_sphere.radius += p_cfg->thickness * 0.5f;

	inst->data = GeometryPoolData3DInstance(p_transform, p_col, p_custom_col ? *p_custom_col : _scoped_config_to_custom(p_cfg));
	inst->bounds = thick_sphere;
	inst->expiration_time = p_exp_time;
	inst->is_used_one_time = false;
	inst->is_visible = true;
}

void GeometryPool::add_or_update_line(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &p_cfg, const real_t &p_exp_time, const ProcessType &p_proc, std::unique_ptr<Vector3[]> p_lines, const size_t p_line_count, const Color &p_col, const AABB &p_aabb) {
	ZoneScoped;
	auto &proc = pools[p_cfg->dcd.viewport][(int)p_proc];
	DelayedRendererLine *inst = proc.lines.get(p_exp_time > 0);
	viewport_ids[p_cfg->dcd.viewport] = p_cfg->dcd.viewport->get_instance_id();

	inst->lines = std::move(p_lines);
	inst->lines_count = p_line_count;
	inst->color = p_col;
	inst->bounds = p_aabb;
	inst->expiration_time = p_exp_time;
	inst->is_used_one_time = false;
	inst->is_visible = true;
}

GeometryType GeometryPool::_scoped_config_get_geometry_type(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &p_cfg) {
	// ZoneScoped;
	if (p_cfg->thickness != 0) {
		return GeometryType::Volumetric;
	}
	// TODO solid
	return GeometryType::Wireframe;
}

Color GeometryPool::_scoped_config_to_custom(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &p_cfg) {
	// ZoneScoped;
	if (_scoped_config_get_geometry_type(p_cfg) == GeometryType::Volumetric)
		return Color((float)p_cfg->thickness, (float)p_cfg->center_brightness, (float)0, (float)0);

	return Color();
}

InstanceType GeometryPool::_scoped_config_type_convert(ConvertableInstanceType p_type, const std::shared_ptr<DebugDraw3DScopeConfig::Data> &p_cfg) {
	// ZoneScoped;
	switch (_scoped_config_get_geometry_type(p_cfg)) {
		case GeometryType::Wireframe: {
			switch (p_type) {
				case ConvertableInstanceType::CUBE:
					return InstanceType::CUBE;
				case ConvertableInstanceType::CUBE_CENTERED:
					return InstanceType::CUBE_CENTERED;
				case ConvertableInstanceType::ARROWHEAD:
					return InstanceType::ARROWHEAD;
				case ConvertableInstanceType::POSITION:
					return InstanceType::POSITION;
				case ConvertableInstanceType::SPHERE:
					if (p_cfg->hd_sphere) {
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
			switch (p_type) {
				case ConvertableInstanceType::CUBE:
					return InstanceType::CUBE_VOLUMETRIC;
				case ConvertableInstanceType::CUBE_CENTERED:
					return InstanceType::CUBE_CENTERED_VOLUMETRIC;
				case ConvertableInstanceType::ARROWHEAD:
					return InstanceType::ARROWHEAD_VOLUMETRIC;
				case ConvertableInstanceType::POSITION:
					return InstanceType::POSITION_VOLUMETRIC;
				case ConvertableInstanceType::SPHERE:
					if (p_cfg->hd_sphere) {
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