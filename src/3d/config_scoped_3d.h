#pragma once

#include "utils/compiler.h"

#include <functional>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/ref_counted.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

/**
 * This class is used to override scoped parameters for DebugDraw3D.
 * Scoped means that these overridden parameters will affect the displayed geometry until it exits the current scope.
 *
 * To create it, use DebugDraw3D.new_scoped_config. Immediately after creation, you can change the values and save the reference to a variable.
 *
 * But the main thing is not to save it outside the method or in other objects. After leaving the scope, this object should be deleted.
 *
 * ```python
 * var _s = DebugDraw3D.new_scoped_config().set_thickness(0.025).set_center_brightness(0.7)
 * DebugDraw3D.draw_grid_xf(%Grid.global_transform, Vector2i(10,10), Color.LIGHT_GRAY)
 * ```
 *
 * ```cs
 * using (var s = DebugDraw3D.NewScopedConfig().SetThickness(0))
 *     DebugDraw3D.DrawCameraFrustum(dCamera, Colors.DarkOrange);
 * ```
 */
class DebugDraw3DScopedConfig : public RefCounted {
	GDCLASS(DebugDraw3DScopedConfig, RefCounted)

protected:
	/// @private
	static void _bind_methods();

private:
	uint64_t thread_id;
	uint64_t guard_id;

	typedef std::function<void(uint64_t, uint64_t)> unregister_func;
	unregister_func unregister_action;

	// Update constructor!
	real_t thickness;
	real_t center_brightness;

public:
	/// @private
	// It can be used for example in C#
	void _manual_unregister();

	/**
	 * Set the thickness of the volumetric lines. If the value is 0, the standard wireframe rendering will be used.
	 *
	 * ![](docs/images/LineThickness.webp)
	 */
	Ref<DebugDraw3DScopedConfig> set_thickness(real_t value);
	real_t get_thickness();

	/**
	 * Set the brightness of the central part of the volumetric lines.
	 *
	 * ![](docs/images/LineCenterBrightness.webp)
	 */
	Ref<DebugDraw3DScopedConfig> set_center_brightness(real_t value);
	real_t get_center_brightness();

	/// @private
	DebugDraw3DScopedConfig();

	// `DDScopedConfig3D` is passed as Ref to avoid a random unreference
	/// @private
	DebugDraw3DScopedConfig(const uint64_t &p_thread_id, const uint64_t &p_guard_id, const DebugDraw3DScopedConfig *parent, const unregister_func p_unreg);
	~DebugDraw3DScopedConfig();
};
