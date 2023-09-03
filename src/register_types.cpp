/* register_types.cpp */

#include "2d/config_2d.h"
#include "2d/debug_draw_2d.h"
#include "2d/graphs.h"
#include "2d/stats_2d.h"
#include "3d/config_3d.h"
#include "3d/debug_draw_3d.h"
#include "3d/stats_3d.h"
#include "debug_draw_manager.h"
#include "utils/utils.h"
using namespace godot;

DebugDrawManager *debug_draw_manager = nullptr;

#ifndef DISABLE_DEBUG_RENDERING
#ifdef TOOLS_ENABLED
GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/editor_plugin.hpp>
GODOT_WARNING_RESTORE()

#include "editor/editor_menu_extensions.h"
#include "editor/generate_csharp_bindings.h"

#include "editor/asset_library_update_checker.h"
Ref<AssetLibraryUpdateChecker> upd_checker;
#endif
#endif

/** GDExtension Initialize **/
void initialize_debug_draw_3d_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		ClassDB::register_class<DebugDraw2D>();
		ClassDB::register_class<DebugDrawStats2D>();
		ClassDB::register_class<DebugDrawConfig2D>();
		ClassDB::register_class<DebugDrawGraph>();
		ClassDB::register_class<DebugDrawFPSGraph>();
		ClassDB::register_class<DDTransform2DGuard>();

		ClassDB::register_class<DebugDraw3D>();
		ClassDB::register_class<DebugDrawStats3D>();
		ClassDB::register_class<DebugDrawConfig3D>();

		ClassDB::register_class<DebugDrawManager>();

		// Since this manager is a node in the scene tree,
		// it will already be destroyed at the time of cleaning this library.
		debug_draw_manager = memnew(DebugDrawManager);
		debug_draw_manager->init();
	}

#ifndef DISABLE_DEBUG_RENDERING
#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		ClassDB::register_class<DebugDrawMenuExtensionPlugin>();
		EditorPlugins::add_by_type<DebugDrawMenuExtensionPlugin>();

		ClassDB::register_class<AssetLibraryUpdateChecker>();
		upd_checker.instantiate();
	}
#endif
#endif
}

/** GDExtension Uninitialize **/
void uninitialize_debug_draw_3d_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		// If this library is disabled manually before deleting the scene tree,
		// then an attempt is made to delete this node manually.
		if (Engine::get_singleton()->get_main_loop() && UtilityFunctions::is_instance_valid(debug_draw_manager)) {
			memdelete(debug_draw_manager);
		}
		debug_draw_manager = nullptr;
	}

#ifndef DISABLE_DEBUG_RENDERING
#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		upd_checker.unref();
		// EditorPlugins::remove_by_type<DebugDrawMenuExtensionPlugin>();
	}
#endif
#endif
}

/** GDExtension Initialize **/
extern "C" {
GDExtensionBool GDE_EXPORT debug_draw_3d_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_debug_draw_3d_module);
	init_obj.register_terminator(uninitialize_debug_draw_3d_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
