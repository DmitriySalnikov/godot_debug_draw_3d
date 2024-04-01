#include "stats_2d.h"

void DebugDraw2DStats::_bind_methods() {
#define REG_PROPERTY_NO_SET(name, type)                                                         \
	ClassDB::bind_method(D_METHOD(NAMEOF(get_##name)), &DebugDraw2DStats::get_##name);          \
	ClassDB::bind_method(D_METHOD(NAMEOF(set_##name), "value"), &DebugDraw2DStats::set_##name); \
	ADD_PROPERTY(PropertyInfo(type, #name), NAMEOF(set_##name), NAMEOF(get_##name));
#pragma region Parameters

	REG_PROPERTY_NO_SET(overlay_text_groups, Variant::INT);
	REG_PROPERTY_NO_SET(overlay_text_lines, Variant::INT);

	REG_PROPERTY_NO_SET(overlay_graphs_enabled, Variant::INT);
	REG_PROPERTY_NO_SET(overlay_graphs_total, Variant::INT);

#undef REG_PROPERTY_NO_SET
#pragma endregion
}

void DebugDraw2DStats::setup(
		const int64_t &p_overlay_text_groups,
		const int64_t &p_overlay_text_lines,
		const int64_t &p_overlay_graphs_enabled,
		const int64_t &p_overlay_graphs_total) {

	overlay_text_groups = p_overlay_text_groups;
	overlay_text_lines = p_overlay_text_lines;

	overlay_graphs_enabled = p_overlay_graphs_enabled;
	overlay_graphs_total = p_overlay_graphs_total;
};