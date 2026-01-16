#pragma once

#include "common/colors.h"
#include "utils/compiler.h"
#include "utils/native_api_hooks.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/ref_counted.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

/**
 * @brief
 * This is a class for storing part of the DebugDraw3D configuration.
 *
 * You can use DebugDraw3D.get_config to get the current instance of the configuration.
 */
NAPI_CLASS_REF class DebugDraw3DConfig : public RefCounted {
	GDCLASS(DebugDraw3DConfig, RefCounted)

public:
	/**
	 * Frustum culling mode. You can disable it completely or select the culling accuracy.
	 */
	NAPI_ENUM enum CullingMode : uint32_t {
		FRUSTUM_DISABLED,
		FRUSTUM_ROUGH,
		FRUSTUM_PRECISE,
	};

private:
	int32_t geometry_render_layers = 1;
	bool freeze_3d_render = false;
	bool visible_instance_bounds = false;
	CullingMode frustum_culling_mode = CullingMode::FRUSTUM_PRECISE;
	real_t frustum_length_scale = 1;
	bool force_use_camera_from_scene = false;
	Color line_hit_color = Colors::red;
	Color line_after_hit_color = Colors::green;

protected:
	/// @private
	static void _bind_methods();

public:
	/**
	 * Set whether debug 3D graphics rendering is frozen.
	 * This means that previously created geometry will not be updated until set to false or until DebugDraw3D.clear_all is called.
	 */
	NAPI void set_freeze_3d_render(const bool &_state);
	NAPI bool is_freeze_3d_render() const;

	/**
	 * Set whether the boundaries of instances are displayed.
	 * Based on these boundaries, instances are culled if set_use_frustum_culling is activated.
	 */
	NAPI void set_visible_instance_bounds(const bool &_state);
	NAPI bool is_visible_instance_bounds() const;

	/**
	 * @deprecated
	 * Set whether frustum culling is used.
	 * This is a wrapper over DebugDraw3DConfig.set_frustum_culling_mode and exists for compatibility with older versions.
	 *
	 * @note
	 * Enabling or disabling this option does not affect the rough culling based on the camera's AABB of frustum.
	 * This option enables more accurate culling based on the camera's frustum planes.
	 */
	NAPI void set_use_frustum_culling(const bool &_state);
	NAPI bool is_use_frustum_culling() const;

	/**
	 * Set frustum culling mode.
	 */
	NAPI void set_frustum_culling_mode(const DebugDraw3DConfig::CullingMode _mode);
	NAPI DebugDraw3DConfig::CullingMode get_frustum_culling_mode() const;

	/**
	 * Change the distance between the Far and Near Planes of the Viewport's Camera3D.
	 */
	NAPI void set_frustum_length_scale(const real_t &_distance);
	NAPI real_t get_frustum_length_scale() const;

	/**
	 * Set the forced use of the scene camera instead of the editor camera.
	 */
	NAPI void set_force_use_camera_from_scene(const bool &_state);
	NAPI bool is_force_use_camera_from_scene() const;

	/**
	 * Set the visibility layer on which the 3D geometry will be drawn.
	 * Similar to using VisualInstance3D.layers.
	 */
	NAPI void set_geometry_render_layers(const int32_t &_layers);
	NAPI int32_t get_geometry_render_layers() const;

	/**
	 * Set the default color for the collision point of DebugDraw3D.draw_line_hit.
	 */
	NAPI void set_line_hit_color(const godot::Color &_new_color);
	NAPI godot::Color get_line_hit_color() const;

	/**
	 * Set the default color for the line after the collision point of DebugDraw3D.draw_line_hit.
	 */
	NAPI void set_line_after_hit_color(const godot::Color &_new_color);
	NAPI godot::Color get_line_after_hit_color() const;
};

VARIANT_ENUM_CAST(DebugDraw3DConfig::CullingMode);
