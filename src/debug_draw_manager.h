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
	void _define_and_update_addon_root_folder();

	void _register_singleton_aliases(const TypedArray<StringName> &p_names, Object *p_instance);
	void _unregister_singleton_aliases(const TypedArray<StringName> &p_names);

	// TEST C# API GENERATOR
#ifdef DEV_ENABLED
public:
	NAPI_ENUM enum DevTestEnum : uint32_t {
		FIRST_VALUE = 0, // Test comment
		SECOND_VALUE = 10,
	};

	static Object *default_arg_obj;

	// Test regular arguments
	// TODO:
	void api_test1(godot::Variant arg1, godot::Object *arg2, bool arg3, int arg4, float arg5, godot::String arg6, godot::StringName arg7, godot::NodePath arg8) {}
	NAPI void api_test2(godot::Color arg1, godot::Vector2 arg2, godot::Vector2i arg3, godot::Vector3 arg4, godot::Vector3i arg5, godot::Vector4 arg6, godot::Vector4i arg7, godot::Rect2 arg8, godot::Rect2i arg9) {}
	NAPI void api_test3(godot::Transform2D arg1, godot::Transform3D arg2, godot::Plane arg3, godot::Quaternion arg4, godot::AABB arg5, godot::Basis arg6, godot::Projection arg7) {}
	// TODO:
	void api_test4(godot::RID arg1, godot::Callable arg2, godot::Signal arg3, godot::Dictionary arg4, godot::Array arg5) {}
	// TODO:
	void api_test5(godot::PackedByteArray arg1, godot::PackedInt32Array arg2, godot::PackedInt64Array arg3, godot::PackedFloat32Array arg4, godot::PackedFloat64Array arg5, godot::PackedStringArray arg6, godot::PackedVector2Array arg7, godot::PackedVector3Array arg8, godot::PackedColorArray arg9) {}
	// Test with default arguments
	// TODO:
	godot::Variant api_test6(godot::Object *arg1, godot::Variant arg2, bool arg3, int arg4, DebugDrawManager::DevTestEnum arg5, float arg6, godot::String arg7, godot::StringName arg8, godot::NodePath arg9) { return "test var"; }
	NAPI godot::Color api_test7(godot::Color arg1, godot::Vector2 arg2, godot::Vector2i arg3, godot::Vector3 arg4, godot::Vector3i arg5, godot::Vector4 arg6, godot::Vector4i arg7, godot::Rect2 arg8, godot::Rect2i arg9) { return godot::Color(4, 3, 2, 1); }
	NAPI DebugDrawManager::DevTestEnum api_test8(godot::Transform2D arg1, godot::Transform3D arg2, godot::Plane arg3, godot::Quaternion arg4, godot::AABB arg5, godot::Basis arg6, godot::Projection arg7) { return (DevTestEnum)1; }
	// TODO:
	godot::Object *api_test9(godot::RID arg1, godot::Callable arg2, godot::Signal arg3, godot::Dictionary arg4, godot::Array arg5) { return this; }
	// TODO:
	void api_test10(godot::PackedByteArray arg1, godot::PackedInt32Array arg2, godot::PackedInt64Array arg3, godot::PackedFloat32Array arg4, godot::PackedFloat64Array arg5, godot::PackedStringArray arg6, godot::PackedVector2Array arg7, godot::PackedVector3Array arg8, godot::PackedColorArray arg9) {}

public:
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
	Dictionary _get_native_functions();
	bool _get_native_functions_is_double();
	int64_t _get_native_functions_hash();
#endif

	/**
	 * Clear all 2D and 3D geometry
	 */
	void clear_all();
	/**
	 * Set whether to display 2D and 3D debug graphics
	 */
	void set_debug_enabled(bool value);
	/**
	 * Whether debug 2D and 3D graphics are disabled
	 */
	bool is_debug_enabled() const;
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

#ifdef DEV_ENABLED
VARIANT_ENUM_CAST(DebugDrawManager::DevTestEnum);
#endif
