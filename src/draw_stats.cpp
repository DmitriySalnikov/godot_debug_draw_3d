#include "draw_stats.h"
#include "utils.h"

void DebugDrawStats::_bind_methods() {
#define REG_PROPERTY_NO_SET(name, type)                                              \
	ClassDB::bind_method(D_METHOD(NAMEOF(get_##name)), &DebugDrawStats::get_##name); \
	ClassDB::bind_method(D_METHOD(NAMEOF(set_##name), "value"), &DebugDrawStats::set_##name); \
	ADD_PROPERTY(PropertyInfo(type, #name), NAMEOF(set_##name), NAMEOF(get_##name));
#pragma region Parameters

	REG_PROPERTY_NO_SET(instances, Variant::INT);
	REG_PROPERTY_NO_SET(lines, Variant::INT);
	REG_PROPERTY_NO_SET(total_geometry, Variant::INT);
	REG_PROPERTY_NO_SET(visible_instances, Variant::INT);
	REG_PROPERTY_NO_SET(visible_lines, Variant::INT);
	REG_PROPERTY_NO_SET(total_visible, Variant::INT);
	REG_PROPERTY_NO_SET(time_filling_buffers_instances_usec, Variant::INT);
	REG_PROPERTY_NO_SET(time_filling_buffers_lines_usec, Variant::INT);
	REG_PROPERTY_NO_SET(total_time_filling_buffers_usec, Variant::INT);
	REG_PROPERTY_NO_SET(time_culling_instant_usec, Variant::INT);
	REG_PROPERTY_NO_SET(time_culling_delayed_usec, Variant::INT);
	REG_PROPERTY_NO_SET(total_time_culling_usec, Variant::INT);
	REG_PROPERTY_NO_SET(total_time_spent_usec, Variant::INT);

#pragma endregion
}
