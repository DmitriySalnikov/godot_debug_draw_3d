#pragma once

#include "colors.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#endif

#include <godot_cpp/classes/ref_counted.hpp>

#if defined(_MSC_VER)
#pragma warning(default : 4244)
#endif

using namespace godot;

class DebugDrawConfig3D : public RefCounted {
	GDCLASS(DebugDrawConfig3D, RefCounted)

private:
	/// Layers on which the geometry will be displayed
	int32_t geometry_render_layers = 1;
	/// Freezing 3d debugging instances
	bool freeze_3d_render = false;
	/// Debug for debug...
	bool visible_instance_bounds = false;
	/// Geometry culling based on camera frustum.
	/// It is not recommended to use with the current implementation.
	bool use_frustum_culling = false;
	/// Geometry culling based on the distance to the camera.
	real_t cull_by_distance = -1;
	/// Force use camera placed on edited scene.
	/// Usable for editor.
	bool force_use_camera_from_scene = false;
	/// Default color of line with hit
	Color line_hit_color = Colors::red;
	/// Default color of line after hit
	Color line_after_hit_color = Colors::green;

protected:
	static void _bind_methods();

public:
	void set_freeze_3d_render(const bool &_state);
	bool is_freeze_3d_render() const;

	void set_visible_instance_bounds(const bool &_state);
	bool is_visible_instance_bounds() const;

	void set_use_frustum_culling(const bool &_state);
	bool is_use_frustum_culling() const;

	void set_cull_by_distance(const real_t &_distance);
	real_t get_cull_by_distance() const;

	void set_force_use_camera_from_scene(const bool &_state);
	bool is_force_use_camera_from_scene() const;

	void set_geometry_render_layers(const int32_t &_layers);
	int32_t get_geometry_render_layers() const;

	void set_line_hit_color(const Color &_new_color);
	Color get_line_hit_color() const;

	void set_line_after_hit_color(const Color &_new_color);
	Color get_line_after_hit_color() const;
};
