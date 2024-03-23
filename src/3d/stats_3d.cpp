#include "stats_3d.h"

void DebugDraw3DStats::_bind_methods() {
#define REG_PROPERTY_NO_SET(name, type)                                                         \
	ClassDB::bind_method(D_METHOD(NAMEOF(get_##name)), &DebugDraw3DStats::get_##name);          \
	ClassDB::bind_method(D_METHOD(NAMEOF(set_##name), "value"), &DebugDraw3DStats::set_##name); \
	ADD_PROPERTY(PropertyInfo(type, #name), NAMEOF(set_##name), NAMEOF(get_##name));
#pragma region Parameters

	REG_PROPERTY_NO_SET(instances, Variant::INT);
	REG_PROPERTY_NO_SET(lines, Variant::INT);
	REG_PROPERTY_NO_SET(instances_physics, Variant::INT);
	REG_PROPERTY_NO_SET(lines_physics, Variant::INT);
	REG_PROPERTY_NO_SET(total_geometry, Variant::INT);

	REG_PROPERTY_NO_SET(visible_instances, Variant::INT);
	REG_PROPERTY_NO_SET(visible_lines, Variant::INT);
	REG_PROPERTY_NO_SET(total_visible, Variant::INT);

	REG_PROPERTY_NO_SET(time_filling_buffers_instances_usec, Variant::INT);
	REG_PROPERTY_NO_SET(time_filling_buffers_lines_usec, Variant::INT);
	REG_PROPERTY_NO_SET(time_filling_buffers_instances_physics_usec, Variant::INT);
	REG_PROPERTY_NO_SET(time_filling_buffers_lines_physics_usec, Variant::INT);
	REG_PROPERTY_NO_SET(total_time_filling_buffers_usec, Variant::INT);

	REG_PROPERTY_NO_SET(time_culling_instances_usec, Variant::INT);
	REG_PROPERTY_NO_SET(time_culling_lines_usec, Variant::INT);
	REG_PROPERTY_NO_SET(total_time_culling_usec, Variant::INT);

	REG_PROPERTY_NO_SET(total_time_spent_usec, Variant::INT);

	REG_PROPERTY_NO_SET(created_scoped_configs, Variant::INT);
	REG_PROPERTY_NO_SET(orphan_scoped_configs, Variant::INT);

#undef REG_PROPERTY_NO_SET
#pragma endregion
}

void DebugDraw3DStats::set_scoped_config_stats(const int64_t &t_created_scoped_configs, const int64_t &t_orphan_scoped_configs) {
	created_scoped_configs = t_created_scoped_configs;
	orphan_scoped_configs = t_orphan_scoped_configs;
}

void DebugDraw3DStats::set_render_stats(
		const int64_t &t_instances,
		const int64_t &t_lines,
		const int64_t &t_visible_instances,
		const int64_t &t_visible_lines,

		const int64_t &t_instances_phys,
		const int64_t &t_lines_phys,

		const int64_t &t_time_filling_buffers_instances_usec,
		const int64_t &t_time_filling_buffers_lines_usec,
		const int64_t &t_time_culling_instances_usec,
		const int64_t &t_time_culling_lines_usec) {

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
	total_visible = visible_instances +
					visible_lines;

	time_filling_buffers_instances_usec = t_time_filling_buffers_instances_usec;
	time_filling_buffers_lines_usec = t_time_filling_buffers_lines_usec;
	total_time_filling_buffers_usec = time_filling_buffers_instances_usec +
									  time_filling_buffers_lines_usec;

	time_culling_instances_usec = t_time_culling_instances_usec;
	time_culling_lines_usec = t_time_culling_lines_usec;
	total_time_culling_usec = time_culling_instances_usec +
							  time_culling_lines_usec;

	total_time_spent_usec = total_time_filling_buffers_usec + total_time_culling_usec;
}

void DebugDraw3DStats::combine_with(const Ref<DebugDraw3DStats> other) {
	instances += other->instances;
	lines += other->lines;
	instances_physics += other->instances_physics;
	lines_physics += other->lines_physics;
	total_geometry += other->total_geometry;

	visible_instances += other->visible_instances;
	visible_lines += other->visible_lines;
	total_visible += other->total_visible;

	time_filling_buffers_instances_usec += other->time_filling_buffers_instances_usec;
	time_filling_buffers_lines_usec += other->time_filling_buffers_lines_usec;
	total_time_filling_buffers_usec += other->total_time_filling_buffers_usec;

	time_culling_instances_usec += other->time_culling_instances_usec;
	time_culling_lines_usec += other->time_culling_lines_usec;
	total_time_culling_usec += other->total_time_culling_usec;

	total_time_spent_usec += other->total_time_spent_usec;
}