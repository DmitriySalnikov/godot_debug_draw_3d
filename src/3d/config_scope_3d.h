#pragma once

#include "utils/compiler.h"
#include "utils/native_api_hooks.h"

#include <functional>
#include <memory>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/viewport.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

/**
 * @brief
 * This class is used to override scope parameters for DebugDraw3D.
 *
 * `Scope` means that these overridden parameters will affect the drawn geometry until it exits the current scope.
 *
 * To create it, use DebugDraw3D.new_scoped_config.
 * Immediately after creation, you can change the values and save the reference in a variable.
 *
 * @warning
 * But the main thing is not to save it outside the method or in other objects.
 * After leaving the scope, this object should be deleted.
 *
 * ---
 * @warning
 * Also, you can't use scope config between `await`s unless this object is freed before `await`.
 * So, narrow the scope if you want to use `await` and DebugDraw3DScopeConfig in the same method.
 * Or set the value of the variable to `null` so that the object is cleared due to lack of references.
 * ```python
 *	# Bad example
 *	var _s = DebugDraw3D.new_scoped_config().set_thickness(0.3)
 *	DebugDraw3D.draw_box(Vector3.ZERO, Quaternion.IDENTITY, Vector3.ONE)
 *	await get_tree().process_frame
 *	# your code...
 *
 *	# Good example
 *	if true:
 *		var _s = DebugDraw3D.new_scoped_config().set_thickness(0.3)
 *		DebugDraw3D.draw_box(Vector3.ZERO, Quaternion.IDENTITY, Vector3.ONE)
 *	await get_tree().process_frame
 *	# your code...
 * ```
 *
 * ### Examples:
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
NAPI_CLASS_REF class DebugDraw3DScopeConfig : public RefCounted {
	GDCLASS(DebugDraw3DScopeConfig, RefCounted)

protected:
	/// @private
	static void _bind_methods();

private:
	uint64_t thread_id;
	uint64_t guard_id;

	using unregister_func = std::function<void(const uint64_t &, const uint64_t &)>;
	unregister_func unregister_action;

public:
	/// @private
	struct DebugContainerDependent {
		// TODO replace by instance_id
		Viewport *viewport;
		bool no_depth_test;

		DebugContainerDependent() :
				viewport(nullptr),
				no_depth_test(false) {
		}

		DebugContainerDependent(Viewport *p_viewport, bool p_no_depth_test) :
				viewport(p_viewport),
				no_depth_test(p_no_depth_test) {
		}
	};

	/// @private
	struct Data {
		// Update the constructor if changes are made!
		real_t thickness;
		real_t center_brightness;
		bool hd_sphere;
		real_t plane_size;
		DebugContainerDependent dcd;

		Data();
		Data(const std::shared_ptr<Data> &parent);
	};
	/// @private
	std::shared_ptr<Data> data = nullptr;

	/// @private
	/**
	 * It can be used for example in C#
	 */
	NAPI void _manual_unregister();

	/**
	 * Set the thickness of the volumetric lines. If the value is 0, the standard wireframe rendering will be used.
	 *
	 * ![](docs/images/classes/LineThickness.webp)
	 */
	Ref<DebugDraw3DScopeConfig> set_thickness(real_t _value) const;
	/// @private
	NAPI NSELF_RETURN set_thickness_selfreturn(real_t _value) const;
	NAPI real_t get_thickness() const;

	/**
	 * Set the brightness of the central part of the volumetric lines.
	 *
	 * ![](docs/images/classes/LineCenterBrightness.webp)
	 */
	Ref<DebugDraw3DScopeConfig> set_center_brightness(real_t _value) const;
	/// @private
	NAPI NSELF_RETURN set_center_brightness_selfreturn(real_t _value) const;
	NAPI real_t get_center_brightness() const;

	/**
	 * Set the mesh density of the sphere
	 *
	 * ![](docs/images/classes/SphereDensity.webp)
	 */
	Ref<DebugDraw3DScopeConfig> set_hd_sphere(bool _value) const;
	/// @private
	NAPI NSELF_RETURN set_hd_sphere_selfreturn(bool _value) const;
	NAPI bool is_hd_sphere() const;

	/**
	 * Set the size of the `Plane` in DebugDraw3D.draw_plane. If set to `INF`, the `Far` parameter of the current camera will be used.
	 *
	 * ![](docs/images/classes/PlaneSize.webp)
	 */
	Ref<DebugDraw3DScopeConfig> set_plane_size(real_t _value) const;
	/// @private
	NAPI NSELF_RETURN set_plane_size_selfreturn(real_t _value) const;
	NAPI real_t get_plane_size() const;

	/**
	 * Set which Viewport will be used to get World3D.
	 *
	 * If the World3D of this Viewport has not been used before,
	 * then the owner of this World3D will be found in the current branch of the tree,
	 * and special observer nodes will be added to it.
	 *
	 * @note
	 * Objects created for a specific Viewport will use only one camera related to that Viewport for culling.
	 */
	Ref<DebugDraw3DScopeConfig> set_viewport(godot::Viewport * _value) const;
	/// @private
	NAPI NSELF_RETURN set_viewport_selfreturn(godot::Viewport * _value) const;
	NAPI godot::Viewport *get_viewport() const;

	/**
	 * Set whether the `depth_test_disabled` flag is added or not in the shaders of the debug shapes.
	 *
	 * @note
	 * It may cause artifacts when drawing volumetric objects.
	 *
	 * ![](docs/images/classes/NoDepthTest.webp)
	 */
	Ref<DebugDraw3DScopeConfig> set_no_depth_test(bool _value) const;
	/// @private
	NAPI NSELF_RETURN set_no_depth_test_selfreturn(bool _value) const;
	NAPI bool is_no_depth_test() const;

	/// @private
	DebugDraw3DScopeConfig();

	/// @private
	DebugDraw3DScopeConfig(const uint64_t &p_thread_id, const uint64_t &p_guard_id, const std::shared_ptr<DebugDraw3DScopeConfig::Data> &p_parent, const unregister_func p_unreg);
	~DebugDraw3DScopeConfig();
};
