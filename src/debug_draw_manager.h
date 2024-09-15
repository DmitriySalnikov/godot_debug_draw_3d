#pragma once

#include "utils/compiler.h"

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
	static void _bind_methods() {};

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
class DebugDrawManager : public CanvasLayer {
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

	Node *_get_current_scene();
	void _connect_scene_changed();
	void _on_scene_changed(bool p_is_scene_null);

	void _integrate_into_engine();

	void _register_singleton_aliases(const TypedArray<StringName> &p_names, Object *p_instance);
	void _unregister_singleton_aliases(const TypedArray<StringName> &p_names);

#ifdef TOOLS_ENABLED
	void _try_to_update_cs_bindings();
#endif

	// TEST C# API GENERATOR
#ifdef DEV_ENABLED
public:
	enum DevTestEnum : int {
		FIRST_VALUE = 0,
		SECOND_VALUE = 1,
	};

private:
	static Object *default_arg_obj;

	// Test regular arguments
	void api_test1(Variant, Object *, bool, int, float, String, StringName, NodePath) {};
	void api_test2(Color, Vector2, Vector2i, Vector3, Vector3i, Vector4, Vector4i, Rect2, Rect2i){};
	void api_test3(Transform2D, Transform3D, Plane, Quaternion, AABB, Basis, Projection){};
	void api_test4(RID, Callable, Signal, Dictionary, Array){};
	void api_test5(PackedByteArray, PackedInt32Array, PackedInt64Array, PackedFloat32Array, PackedFloat64Array, PackedStringArray, PackedVector2Array, PackedVector3Array, PackedColorArray){};
	// Test with default arguments
	Variant api_test6(Object *, Variant, Variant, bool, int, DevTestEnum, float, String, StringName, NodePath) { return "test var"; };
	Color api_test7(Color, Vector2, Vector2i, Vector3, Vector3i, Vector4, Vector4i, Rect2, Rect2i) { return Color(4, 3, 2, 1); };
	DevTestEnum api_test8(Transform2D, Transform3D, Plane, Quaternion, AABB, Basis, Projection) { return (DevTestEnum)1; };
	Object *api_test9(RID, Callable, Signal, Dictionary, Array) { return this; };
	void api_test10(PackedByteArray, PackedInt32Array, PackedInt64Array, PackedFloat32Array, PackedFloat64Array, PackedStringArray, PackedVector2Array, PackedVector3Array, PackedColorArray){};

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
	};

#pragma region Exposed Methods
#ifdef NATIVE_API_ENABLED
	Dictionary _get_native_functions();
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
