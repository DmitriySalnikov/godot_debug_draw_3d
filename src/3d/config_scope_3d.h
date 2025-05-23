#pragma once

#include "utils/compiler.h"

#include <functional>
#include <memory>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/font.hpp>
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
class DebugDraw3DScopeConfig : public RefCounted {
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
		Viewport *viewport = nullptr;
		uint64_t viewport_id = 0;
		bool no_depth_test = false;
	};

	/// @private
	struct Data {
		// Update the constructor if changes are made!
		real_t thickness;
		real_t center_brightness;
		real_t plane_size;
		Transform3D transform;
		Color text_outline_color;
		uint32_t text_outline_color_hash;
		int32_t text_outline_size;
		Ref<Font> text_font;
		DebugContainerDependent dcd;
		bool hd_sphere;
		bool custom_xform;

		Data();
		Data(const Data *parent);
	};
	/// @private
	std::shared_ptr<Data> data = nullptr;

	/// @private
	// It can be used for example in C#
	void _manual_unregister();

	/**
	 * Set the thickness of the volumetric lines. If the value is 0, the standard wireframe rendering will be used.
	 *
	 * ![](docs/images/classes/LineThickness.webp)
	 */
	Ref<DebugDraw3DScopeConfig> set_thickness(real_t _value) const;
	real_t get_thickness() const;

	/**
	 * Set the brightness of the central part of the volumetric lines.
	 *
	 * ![](docs/images/classes/LineCenterBrightness.webp)
	 */
	Ref<DebugDraw3DScopeConfig> set_center_brightness(real_t _value) const;
	real_t get_center_brightness() const;

	/**
	 * Set the mesh density of the sphere
	 *
	 * ![](docs/images/classes/SphereDensity.webp)
	 */
	Ref<DebugDraw3DScopeConfig> set_hd_sphere(bool _value) const;
	bool is_hd_sphere() const;

	/**
	 * Set the size of the `Plane` in DebugDraw3D.draw_plane. If set to `INF`, the `Far` parameter of the current camera will be used.
	 *
	 * ![](docs/images/classes/PlaneSize.webp)
	 */
	Ref<DebugDraw3DScopeConfig> set_plane_size(real_t _value) const;
	real_t get_plane_size() const;

	/**
	 * Set the base/local `transform` relative to which the shapes will be drawn.
	 */
	Ref<DebugDraw3DScopeConfig> set_transform(Transform3D _value) const;
	Transform3D get_transform() const;

	/**
	 * Set the `outline` color in DebugDraw3D.draw_text.
	 *
	 * ![](docs/images/classes/DrawTextOutlineColor.webp)
	 *
	 * @warning
	 * Frequent unsystematic changes to this property can lead to significant performance degradation.
	 */
	Ref<DebugDraw3DScopeConfig> set_text_outline_color(Color _value) const;
	Color get_text_outline_color() const;

	/**
	 * Set the size of the `outline` in DebugDraw3D.draw_text.
	 *
	 * ![](docs/images/classes/DrawTextOutlineSize.webp)
	 *
	 * @warning
	 * Frequent unsystematic changes to this property can lead to significant performance degradation.
	 */
	Ref<DebugDraw3DScopeConfig> set_text_outline_size(int32_t _value) const;
	int32_t get_text_outline_size() const;

	/**
	 * Set the font of the text in DebugDraw3D.draw_text.
	 *
	 * ![](docs/images/classes/DrawTextFont.webp)
	 *
	 * @warning
	 * Frequent unsystematic changes to this property can lead to significant performance degradation.
	 */
	Ref<DebugDraw3DScopeConfig> set_text_font(Ref<Font> _value) const;
	Ref<Font> get_text_font() const;

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
	Ref<DebugDraw3DScopeConfig> set_viewport(Viewport *_value) const;
	Viewport *get_viewport() const;

	/**
	 * Set whether the `depth_test_disabled` flag is added or not in the shaders of the debug shapes.
	 *
	 * @note
	 * It may cause artifacts when drawing volumetric objects.
	 *
	 * ![](docs/images/classes/NoDepthTest.webp)
	 */
	Ref<DebugDraw3DScopeConfig> set_no_depth_test(bool _value) const;
	bool is_no_depth_test() const;

	/// @private
	DebugDraw3DScopeConfig();

	// `DDScopedConfig3D` is passed as Ref to avoid a random unreference
	/// @private
	DebugDraw3DScopeConfig(const uint64_t &p_thread_id, const uint64_t &p_guard_id, const DebugDraw3DScopeConfig::Data *p_parent, const unregister_func p_unreg);
	~DebugDraw3DScopeConfig();
};
