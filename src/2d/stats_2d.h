#pragma once

#include "utils/compiler.h"
#include "utils/native_api_hooks.h"

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

private:
	int64_t overlay_text_groups = 0;
	int64_t overlay_text_lines = 0;

public:
	NAPI int64_t get_overlay_text_groups() const { return overlay_text_groups; }
	NAPI void set_overlay_text_groups(int64_t val) {};
	NAPI int64_t get_overlay_text_lines() const { return overlay_text_lines; }
	NAPI void set_overlay_text_lines(int64_t val) {};

#undef DEFINE_DEFAULT_PROP

	DebugDraw2DStats() {};

	/// @private
	void setup(
			const int64_t &p_overlay_text_groups,
			const int64_t &p_overlay_text_lines);
};
