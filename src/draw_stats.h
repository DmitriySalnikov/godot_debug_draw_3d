#pragma once

#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#endif

#include <godot_cpp/classes/ref_counted.hpp>

#if defined(_MSC_VER)
#pragma warning(default : 4244)
#endif
using namespace godot;

class DebugDrawStats : public RefCounted {
	GDCLASS(DebugDrawStats, RefCounted)

protected:
	static void _bind_methods();

#define DEFINE_DEFAULT_PROP(type, name, def) \
private:                                     \
	type name = def;                         \
                                             \
public:                                      \
	type get_##name() const { return name; }

	DEFINE_DEFAULT_PROP(int64_t, instances, 0);
	DEFINE_DEFAULT_PROP(int64_t, lines, 0);
	DEFINE_DEFAULT_PROP(int64_t, total_geometry, 0);

	DEFINE_DEFAULT_PROP(int64_t, visible_instances, 0);
	DEFINE_DEFAULT_PROP(int64_t, visible_lines, 0);
	DEFINE_DEFAULT_PROP(int64_t, total_visible, 0);

	DEFINE_DEFAULT_PROP(int64_t, time_filling_buffers_instances_usec, 0);
	DEFINE_DEFAULT_PROP(int64_t, time_filling_buffers_lines_usec, 0);
	DEFINE_DEFAULT_PROP(int64_t, total_time_filling_buffers_usec, 0);

	DEFINE_DEFAULT_PROP(int64_t, time_culling_instant_usec, 0);
	DEFINE_DEFAULT_PROP(int64_t, time_culling_delayed_usec, 0);
	DEFINE_DEFAULT_PROP(int64_t, total_time_culling_usec, 0);

	DEFINE_DEFAULT_PROP(int64_t, total_time_spent_usec, 0);

#undef DEFINE_DEFAULT_PROP

	void set_not_exposed(int64_t t_val) {}

	DebugDrawStats(){};

	void setup(
			const int64_t &t_instances,
			const int64_t &t_lines,
			const int64_t &t_visible_instances,
			const int64_t &t_visible_lines,
			const int64_t &t_time_filling_buffers_instances_usec,
			const int64_t &t_time_filling_buffers_lines_usec,
			const int64_t &t_time_culling_instant_usec,
			const int64_t &t_time_culling_delayed_usec) {

		instances = t_instances;
		lines = t_lines;
		total_geometry = t_instances + t_lines;

		visible_instances = t_visible_instances;
		visible_lines = t_visible_lines;
		total_visible = t_visible_instances + t_visible_lines;

		time_filling_buffers_instances_usec = t_time_filling_buffers_instances_usec;
		time_filling_buffers_lines_usec = t_time_filling_buffers_lines_usec;
		total_time_filling_buffers_usec = t_time_filling_buffers_instances_usec + t_time_filling_buffers_lines_usec;

		time_culling_instant_usec = t_time_culling_instant_usec;
		time_culling_delayed_usec = t_time_culling_delayed_usec;
		total_time_culling_usec = t_time_culling_instant_usec + t_time_culling_delayed_usec;

		total_time_spent_usec = total_time_filling_buffers_usec + total_time_culling_usec;
	};
};
