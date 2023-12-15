#include "stats_3d.h"

void DebugDrawStats3D::_bind_methods() {
#define REG_PROPERTY_NO_SET(name, type)                                                         \
	ClassDB::bind_method(D_METHOD(NAMEOF(get_##name)), &DebugDrawStats3D::get_##name);          \
	ClassDB::bind_method(D_METHOD(NAMEOF(set_##name), "value"), &DebugDrawStats3D::set_##name); \
	ADD_PROPERTY(PropertyInfo(type, #name), NAMEOF(set_##name), NAMEOF(get_##name));
#pragma region Parameters

	REG_PROPERTY_NO_SET(instances, Variant::INT);
	REG_PROPERTY_NO_SET(lines, Variant::INT);
	REG_PROPERTY_NO_SET(instances_physics, Variant::INT);
	REG_PROPERTY_NO_SET(lines_physics, Variant::INT);
	REG_PROPERTY_NO_SET(total_geometry, Variant::INT);

	REG_PROPERTY_NO_SET(visible_instances, Variant::INT);
	REG_PROPERTY_NO_SET(visible_lines, Variant::INT);
	REG_PROPERTY_NO_SET(visible_instances_physics, Variant::INT);
	REG_PROPERTY_NO_SET(visible_lines_physics, Variant::INT);
	REG_PROPERTY_NO_SET(total_visible, Variant::INT);

	REG_PROPERTY_NO_SET(time_filling_buffers_instances_usec, Variant::INT);
	REG_PROPERTY_NO_SET(time_filling_buffers_lines_usec, Variant::INT);
	REG_PROPERTY_NO_SET(time_filling_buffers_instances_physics_usec, Variant::INT);
	REG_PROPERTY_NO_SET(time_filling_buffers_lines_physics_usec, Variant::INT);
	REG_PROPERTY_NO_SET(total_time_filling_buffers_usec, Variant::INT);

	REG_PROPERTY_NO_SET(time_culling_instant_usec, Variant::INT);
	REG_PROPERTY_NO_SET(time_culling_delayed_usec, Variant::INT);
	REG_PROPERTY_NO_SET(time_culling_instant_physics_usec, Variant::INT);
	REG_PROPERTY_NO_SET(time_culling_delayed_physics_usec, Variant::INT);
	REG_PROPERTY_NO_SET(total_time_culling_usec, Variant::INT);

	REG_PROPERTY_NO_SET(total_time_spent_usec, Variant::INT);

	REG_PROPERTY_NO_SET(created_scoped_configs, Variant::INT);
	REG_PROPERTY_NO_SET(orphan_scoped_configs, Variant::INT);

#undef REG_PROPERTY_NO_SET
#pragma endregion
}

void DebugDrawStats3D::set_scoped_config_stats(const int64_t &t_created_scoped_configs, const int64_t &t_orphan_scoped_configs) {
	created_scoped_configs = t_created_scoped_configs;
	orphan_scoped_configs = t_orphan_scoped_configs;
}

void DebugDrawStats3D::set_render_stats(
		const int64_t &t_instances,
		const int64_t &t_lines,
		const int64_t &t_visible_instances,
		const int64_t &t_visible_lines,

		const int64_t &t_instances_phys,
		const int64_t &t_lines_phys,
		const int64_t &t_visible_instances_phys,
		const int64_t &t_visible_lines_phys,

		const int64_t &t_time_filling_buffers_instances_usec,
		const int64_t &t_time_filling_buffers_lines_usec,
		const int64_t &t_time_culling_instant_usec,
		const int64_t &t_time_culling_delayed_usec) {

	instances = t_instances;
	lines = t_lines;
	instances_physics = t_instances_phys;
	lines_physics = t_lines_phys;
	total_geometry = instances +
					 lines +
					 instances_physics +
					 lines_physics;

	visible_instances = t_visible_instances;
	visible_lines = t_visible_lines;
	visible_instances_physics = t_visible_instances_phys;
	visible_lines_physics = t_visible_lines_phys;
	total_visible = visible_instances +
					visible_lines +
					visible_instances_physics +
					visible_lines_physics;

	time_filling_buffers_instances_usec = t_time_filling_buffers_instances_usec;
	time_filling_buffers_lines_usec = t_time_filling_buffers_lines_usec;
	total_time_filling_buffers_usec = time_filling_buffers_instances_usec +
									  time_filling_buffers_lines_usec;

	time_culling_instant_usec = t_time_culling_instant_usec;
	time_culling_delayed_usec = t_time_culling_delayed_usec;
	total_time_culling_usec = time_culling_instant_usec +
							  time_culling_delayed_usec;

	total_time_spent_usec = total_time_filling_buffers_usec + total_time_culling_usec;
}
