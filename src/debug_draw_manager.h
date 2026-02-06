#pragma once

#include "utils/compiler.h"
#include "utils/native_api_hooks.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/canvas_layer.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class DebugDraw2D;
class DebugDraw3D;
class DebugDrawManager;

#ifndef DISABLE_DEBUG_RENDERING
/// @private
class _DD3D_PhysicsWatcher : public Node {
	GDCLASS(_DD3D_PhysicsWatcher, Node)
protected:
	DebugDrawManager *root_node;
	static void _bind_methods() {}

public:
	void init(DebugDrawManager *p_root);

	virtual void _process(double p_delta) override;
	virtual void _physics_process(double p_delta) override;
};
#endif

/**
 * @brief
 * The main singleton class that handles DebugDraw2D and DebugDraw3D.
 *
 * Several additional settings can be found in the project settings.
 *
 * @note The following settings require a restart.
 *
 * `debug_draw_3d/settings/initial_debug_state` sets the initial debugging state.
 *
 * `debug_draw_3d/settings/common/DebugDrawManager_singleton_aliases` sets aliases for DebugDrawManager to be registered as additional singletons.
 *
 * `debug_draw_3d/settings/common/DebugDraw2D_singleton_aliases` sets aliases for DebugDraw2D to be registered as additional singletons.
 *
 * `debug_draw_3d/settings/common/DebugDraw3D_singleton_aliases` sets aliases for DebugDraw3D to be registered as additional singletons.
 *
 * Using these aliases you can reference singletons with shorter words:
 *
 * ```python
 * var _s = Dbg3.new_scoped_config().set_thickness(0.025).set_center_brightness(0.7)
 * Dbg3.draw_grid_xf(%Grid.global_transform, Vector2i(10,10), Color.LIGHT_GRAY)
 * Dbg2.set_text("Frametime", delta)
 * ```
 */
NAPI_CLASS_SINGLETON class DebugDrawManager : public CanvasLayer {
	GDCLASS(DebugDrawManager, CanvasLayer)
protected:
	/// @private
	static void _bind_methods();

private:
#ifndef DISABLE_DEBUG_RENDERING
	friend _DD3D_PhysicsWatcher;
#endif

	static DebugDrawManager *singleton;

	String root_settings_section;
	static constexpr const char *s_initial_state = "initial_debug_state";
	static constexpr const char *s_manager_aliases = NAMEOF(DebugDrawManager) "_singleton_aliases ";
	static constexpr const char *s_dd2d_aliases = NAMEOF(DebugDraw2D) "_singleton_aliases";
	static constexpr const char *s_dd3d_aliases = NAMEOF(DebugDraw3D) "_singleton_aliases";

	double log_flush_time = 0;
	bool debug_enabled = true;
	bool is_closing = false;
	bool is_current_scene_is_null = true;

	DebugDraw2D *debug_draw_2d_singleton = nullptr;
	DebugDraw3D *debug_draw_3d_singleton = nullptr;

	TypedArray<StringName> manager_aliases;
	TypedArray<StringName> dd2d_aliases;
	TypedArray<StringName> dd3d_aliases;

	void _connect_scene_changed();
	void _on_scene_changed(bool p_is_scene_null);

	void _integrate_into_engine();
	String _define_and_update_addon_root_folder();
	void _remove_old_bindings(String addon_folder) const;

	void _register_singleton_aliases(const TypedArray<StringName> &p_names, Object *p_instance);
	void _unregister_singleton_aliases(const TypedArray<StringName> &p_names);

	// TEST C# API GENERATOR
#ifdef DEV_ENABLED
public:
	NAPI_ENUM enum DevTestEnum : uint16_t {
		FIRST_VALUE = 0, // Test comment
		SECOND_VALUE = 10,
	};

	// Test regular arguments
	NAPI void api_test1_c(godot::Object *arg1, bool arg2, int arg3, float arg4, const char *arg5_string, DebugDrawManager::DevTestEnum arg6) {}
	NAPI void api_test2(godot::Color arg1, godot::Vector2 arg2, godot::Vector2i arg3, godot::Vector3 arg4, godot::Vector3i arg5, godot::Vector4 arg6, godot::Vector4i arg7, godot::Rect2 arg8, godot::Rect2i arg9) {}
	NAPI void api_test3(godot::Transform2D arg1, godot::Transform3D arg2, godot::Plane arg3, godot::Quaternion arg4, godot::AABB arg5, godot::Basis arg6, godot::Projection arg7) {}
	void api_test5(uint8_t *arg1_data, uint64_t arg1_size, int32_t *arg2_data, uint64_t arg2_size, int64_t *arg3_data, uint64_t arg3_size, float *arg4_data, uint64_t arg4_size, double *arg5_data, uint64_t arg5_size, godot::Vector2 *arg6_data, uint64_t arg6_size, godot::Vector3 *arg8_data, uint64_t arg8_size, godot::Color *arg9_data, uint64_t arg9_size) {}
	// Test with default arguments
	NAPI bool api_test4_c(godot::Object *arg1 = nullptr, bool arg2 = true, int arg3 = 1, DebugDrawManager::DevTestEnum arg4 = DebugDrawManager::DevTestEnum::SECOND_VALUE, float arg5 = 0.5f, const char *arg6_string = "test_str") { return {}; }
	NAPI DebugDrawManager::DevTestEnum api_test7(godot::Color arg1 = godot::Color(1, 0, 1, 1), godot::Vector2 arg2 = godot::Vector2(1, 0), godot::Vector3 arg3 = godot::Vector3(1, 1, 1)) { return {}; }
	NAPI godot::Quaternion api_test8(real_t angle_deg) { return Quaternion(Vector3_RIGHT, Math::deg_to_rad(angle_deg)); }
#endif

public:
	/// @private
	static constexpr const char *s_extension_unloading = "extension_unloading";

	DebugDrawManager();
	~DebugDrawManager();

	/**
	 * Get singleton. Not available in GDScript.
	 */
	static DebugDrawManager *get_singleton() {
		return singleton;
	}

	/// @private
	Node *get_current_scene();

#pragma region Exposed Methods
#ifdef NATIVE_API_ENABLED
	Dictionary _get_native_classes();
	Dictionary _get_native_functions();
	bool _get_native_functions_is_double();
	int64_t _get_native_functions_hash();
#endif

	/**
	 * Clear all 2D and 3D geometry
	 */
	NAPI void clear_all();
	/**
	 * Set whether to display 2D and 3D debug graphics
	 */
	NAPI void set_debug_enabled(bool value);
	/**
	 * Whether debug 2D and 3D graphics are disabled
	 */
	NAPI bool is_debug_enabled() const;

	/**
	 * Returns a dictionary with the keys "major", "minor" and "patch"
	 */
	godot::Dictionary get_addon_version() const;
	/**
	 * Returns a string with the version in the "major.minor.patch" format
	 */
	godot::String get_addon_version_str() const;
#pragma endregion // Exposed Methods

	/// @private
	void init();
	/// @private
	void deinit();

	/// @private
	void _process_start(double p_delta);
	/// @private
	virtual void _process(double p_delta) override;
	/// @private
	void _physics_process_start(double p_delta);
	/// @private
	virtual void _physics_process(double p_delta) override;
};
