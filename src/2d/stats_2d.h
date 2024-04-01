#pragma once

#include "utils/compiler.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/ref_counted.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

/**
 * @brief
 * You can get basic statistics about 2D rendering from this class.
 *
 * All names try to reflect what they mean.
 *
 * To get an instance of this class with current statistics, use DebugDraw2D.get_render_stats.
 */
class DebugDraw2DStats : public RefCounted {
	GDCLASS(DebugDraw2DStats, RefCounted)

protected:
	/// @private
	static void _bind_methods();

#define DEFINE_DEFAULT_PROP(name, type, def) \
private:                                     \
	type name = def;                         \
                                             \
public:                                      \
	type get_##name() const { return name; } \
	void set_##name(int64_t val) {}

	DEFINE_DEFAULT_PROP(overlay_text_groups, int64_t, 0);
	DEFINE_DEFAULT_PROP(overlay_text_lines, int64_t, 0);

	DEFINE_DEFAULT_PROP(overlay_graphs_enabled, int64_t, 0);
	DEFINE_DEFAULT_PROP(overlay_graphs_total, int64_t, 0);

#undef DEFINE_DEFAULT_PROP

	DebugDraw2DStats(){};

	/// @private
	void setup(
			const int64_t &p_overlay_text_groups,
			const int64_t &p_overlay_text_lines,
			const int64_t &p_overlay_graphs_enabled,
			const int64_t &p_overlay_graphs_total);
};
