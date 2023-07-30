/* register_types.cpp */

#include "data_graphs.h"
#include "debug_draw_2d.h"
#include "debug_draw_3d.h"
#include "debug_draw_config_2d.h"
#include "debug_draw_config_3d.h"
#include "debug_draw_manager.h"
#include "draw_stats.h"
#include "utils.h"

using namespace godot;

DebugDrawManager *debug_draw_manager = nullptr;

#ifndef DISABLE_DEBUG_RENDERING
#ifdef DEBUG_ENABLED
#include "asset_library_update_checker.h"
Ref<AssetLibraryUpdateChecker> upd_checker;
#endif
#endif

/** GDExtension Initialize **/
void initialize_debug_draw_3d_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		ClassDB::register_class<DebugDraw2D>();
		ClassDB::register_class<DebugDraw3D>();
		ClassDB::register_class<DebugDrawConfig2D>();
		ClassDB::register_class<DebugDrawConfig3D>();
		ClassDB::register_class<DebugDrawGraph>();
		ClassDB::register_class<DebugDrawFPSGraph>();
		ClassDB::register_class<DebugDrawManager>();
		ClassDB::register_class<DebugDrawStats>();

		// Since this manager is a node in the scene tree,
		// it will already be destroyed at the time of cleaning this library.
		debug_draw_manager = memnew(DebugDrawManager);
		debug_draw_manager->init();
	}

#ifndef DISABLE_DEBUG_RENDERING
#ifdef DEBUG_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
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
		if (UtilityFunctions::is_instance_valid(debug_draw_manager)) {
			memdelete(debug_draw_manager);
		}
		debug_draw_manager = nullptr;
	}

#ifndef DISABLE_DEBUG_RENDERING
#ifdef DEBUG_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		upd_checker.unref();
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
