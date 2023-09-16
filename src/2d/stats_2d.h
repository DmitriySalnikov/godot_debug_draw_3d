#pragma once

#include "utils/compiler.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/ref_counted.hpp>
GODOT_WARNING_RESTORE()

using namespace godot;

class DebugDrawStats2D : public RefCounted {
	GDCLASS(DebugDrawStats2D, RefCounted)

protected:
	static void _bind_methods();

#define DEFINE_DEFAULT_PROP(type, name, def) \
private:                                     \
	type name = def;                         \
                                             \
public:                                      \
	type get_##name() const { return name; } \
	void set_##name(int64_t val) {}

	DEFINE_DEFAULT_PROP(int64_t, overlay_text_groups, 0);
	DEFINE_DEFAULT_PROP(int64_t, overlay_text_lines, 0);

	DEFINE_DEFAULT_PROP(int64_t, overlay_graphs_enabled, 0);
	DEFINE_DEFAULT_PROP(int64_t, overlay_graphs_total, 0);

#undef DEFINE_DEFAULT_PROP

	DebugDrawStats2D(){};

	void setup(
			const int64_t &t_overlay_text_groups,
			const int64_t &t_overlay_text_lines,
			const int64_t &t_overlay_graphs_enabled,
			const int64_t &t_overlay_graphs_total) {

		overlay_text_groups = t_overlay_text_groups;
		overlay_text_lines = t_overlay_text_lines;

		overlay_graphs_enabled = t_overlay_graphs_enabled;
		overlay_graphs_total = t_overlay_graphs_total;
	};
};
