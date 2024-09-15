#pragma once

#include "utils/compiler.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/ref_counted.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

/**
 * @brief
 * You can get statistics about 3D rendering from this class.
 *
 * All names try to reflect what they mean.
 *
 * To get an instance of this class with current statistics, use DebugDraw3D.get_render_stats.
 *
 * `instances` lets you know how many instances have been created.
 *
 * `instances_physics` reports how many instances were created inside `_physics_process`.
 *
 * `total_time_spent_usec` reports the time in microseconds spent to process everything and display the geometry on the screen.
 */
NAPI_CLASS_REF DebugDraw3DStats : public RefCounted {
	GDCLASS(DebugDraw3DStats, RefCounted)

protected:
	/// @private
	static void _bind_methods();

#define DEFINE_DEFAULT_PROP(name, type, def) \
private:                                     \
	type name = def;                         \
                                             \
public:                                      \
	type get_##name() const {                \
		return name;                         \
	}                                        \
	void set_##name(int64_t val) {}

	// TODO gen API
	DEFINE_DEFAULT_PROP(instances, int64_t, 0);
	DEFINE_DEFAULT_PROP(lines, int64_t, 0);
	DEFINE_DEFAULT_PROP(instances_physics, int64_t, 0);
	DEFINE_DEFAULT_PROP(lines_physics, int64_t, 0);
	DEFINE_DEFAULT_PROP(total_geometry, int64_t, 0);

	DEFINE_DEFAULT_PROP(visible_instances, int64_t, 0);
	DEFINE_DEFAULT_PROP(visible_lines, int64_t, 0);
	DEFINE_DEFAULT_PROP(total_visible, int64_t, 0);

	DEFINE_DEFAULT_PROP(time_filling_buffers_instances_usec, int64_t, 0);
	DEFINE_DEFAULT_PROP(time_filling_buffers_lines_usec, int64_t, 0);
	DEFINE_DEFAULT_PROP(time_filling_buffers_instances_physics_usec, int64_t, 0);
	DEFINE_DEFAULT_PROP(time_filling_buffers_lines_physics_usec, int64_t, 0);
	DEFINE_DEFAULT_PROP(total_time_filling_buffers_usec, int64_t, 0);

	DEFINE_DEFAULT_PROP(time_culling_instances_usec, int64_t, 0);
	DEFINE_DEFAULT_PROP(time_culling_lines_usec, int64_t, 0);
	DEFINE_DEFAULT_PROP(total_time_culling_usec, int64_t, 0);

	DEFINE_DEFAULT_PROP(total_time_spent_usec, int64_t, 0);

	DEFINE_DEFAULT_PROP(created_scoped_configs, int64_t, 0);
	DEFINE_DEFAULT_PROP(orphan_scoped_configs, int64_t, 0);

#undef DEFINE_DEFAULT_PROP

	DebugDraw3DStats(){};

	/// @private
	void set_scoped_config_stats(
			const int64_t &p_created_scoped_configs,
			const int64_t &p_orphan_scoped_configs);

	/// @private
	void set_render_stats(
			const int64_t &p_instances,
			const int64_t &p_lines,
			const int64_t &p_visible_instances,
			const int64_t &p_visible_lines,

			const int64_t &p_instances_phys,
			const int64_t &p_lines_phys,

			const int64_t &p_time_filling_buffers_instances_usec,
			const int64_t &p_time_filling_buffers_lines_usec,
			const int64_t &p_time_culling_instances_usec,
			const int64_t &p_time_culling_lines_usec);

	///  @private
	void combine_with(const Ref<DebugDraw3DStats> p_other);
};
