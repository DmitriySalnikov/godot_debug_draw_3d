#include "stats_2d.h"

void DebugDrawStats2D::_bind_methods() {
#define REG_PROPERTY_NO_SET(name, type)                                                         \
	ClassDB::bind_method(D_METHOD(NAMEOF(get_##name)), &DebugDrawStats2D::get_##name);          \
	ClassDB::bind_method(D_METHOD(NAMEOF(set_##name), "value"), &DebugDrawStats2D::set_##name); \
	ADD_PROPERTY(PropertyInfo(type, #name), NAMEOF(set_##name), NAMEOF(get_##name));
#pragma region Parameters

	REG_PROPERTY_NO_SET(overlay_text_groups, Variant::INT);
	REG_PROPERTY_NO_SET(overlay_text_lines, Variant::INT);

	REG_PROPERTY_NO_SET(overlay_graphs_enabled, Variant::INT);
	REG_PROPERTY_NO_SET(overlay_graphs_total, Variant::INT);

#undef REG_PROPERTY_NO_SET
#pragma endregion
}
