#pragma once

#include "utils/compiler.h"
#include "utils/native_api_hooks.h"

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
NAPI_CLASS_REF class DebugDraw3DStats : public RefCounted {
	GDCLASS(DebugDraw3DStats, RefCounted)

protected:
	/// @private
	static void _bind_methods();

private:
	int64_t instances = 0;
	int64_t lines = 0;
	int64_t instances_physics = 0;
	int64_t lines_physics = 0;
	int64_t total_geometry = 0;

	int64_t visible_instances = 0;
	int64_t visible_lines = 0;
	int64_t total_visible = 0;

	int64_t time_filling_buffers_instances_usec = 0;
	int64_t time_filling_buffers_lines_usec = 0;
	int64_t time_filling_buffers_instances_physics_usec = 0;
	int64_t time_filling_buffers_lines_physics_usec = 0;
	int64_t total_time_filling_buffers_usec = 0;

	int64_t time_culling_instances_usec = 0;
	int64_t time_culling_lines_usec = 0;
	int64_t total_time_culling_usec = 0;

	int64_t total_time_spent_usec = 0;

	int64_t created_scoped_configs = 0;
	int64_t orphan_scoped_configs = 0;

public:
	NAPI int64_t get_instances() const { return instances; }
	NAPI void set_instances(int64_t val) {};
	NAPI int64_t get_lines() const { return lines; }
	NAPI void set_lines(int64_t val) {};
	NAPI int64_t get_instances_physics() const { return instances_physics; }
	NAPI void set_instances_physics(int64_t val) {};
	NAPI int64_t get_lines_physics() const { return lines_physics; }
	NAPI void set_lines_physics(int64_t val) {};
	NAPI int64_t get_total_geometry() const { return total_geometry; }
	NAPI void set_total_geometry(int64_t val) {};

	NAPI int64_t get_visible_instances() const { return visible_instances; }
	NAPI void set_visible_instances(int64_t val) {};
	NAPI int64_t get_visible_lines() const { return visible_lines; }
	NAPI void set_visible_lines(int64_t val) {};
	NAPI int64_t get_total_visible() const { return total_visible; }
	NAPI void set_total_visible(int64_t val) {};

	NAPI int64_t get_time_filling_buffers_instances_usec() const { return time_filling_buffers_instances_usec; }
	NAPI void set_time_filling_buffers_instances_usec(int64_t val) {};
	NAPI int64_t get_time_filling_buffers_lines_usec() const { return time_filling_buffers_lines_usec; }
	NAPI void set_time_filling_buffers_lines_usec(int64_t val) {};
	NAPI int64_t get_time_filling_buffers_instances_physics_usec() const { return time_filling_buffers_instances_physics_usec; }
	NAPI void set_time_filling_buffers_instances_physics_usec(int64_t val) {};
	NAPI int64_t get_time_filling_buffers_lines_physics_usec() const { return time_filling_buffers_lines_physics_usec; }
	NAPI void set_time_filling_buffers_lines_physics_usec(int64_t val) {};
	NAPI int64_t get_total_time_filling_buffers_usec() const { return total_time_filling_buffers_usec; }
	NAPI void set_total_time_filling_buffers_usec(int64_t val) {};

	NAPI int64_t get_time_culling_instances_usec() const { return time_culling_instances_usec; }
	NAPI void set_time_culling_instances_usec(int64_t val) {};
	NAPI int64_t get_time_culling_lines_usec() const { return time_culling_lines_usec; }
	NAPI void set_time_culling_lines_usec(int64_t val) {};
	NAPI int64_t get_total_time_culling_usec() const { return total_time_culling_usec; }
	NAPI void set_total_time_culling_usec(int64_t val) {};

	NAPI int64_t get_total_time_spent_usec() const { return total_time_spent_usec; }
	NAPI void set_total_time_spent_usec(int64_t val) {};

	NAPI int64_t get_created_scoped_configs() const { return created_scoped_configs; }
	NAPI void set_created_scoped_configs(int64_t val) {};
	NAPI int64_t get_orphan_scoped_configs() const { return orphan_scoped_configs; }
	NAPI void set_orphan_scoped_configs(int64_t val) {};

#undef DEFINE_DEFAULT_PROP

	DebugDraw3DStats() {};

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
