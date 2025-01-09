/* register_types.cpp */

#include "2d/config_2d.h"
#include "2d/debug_draw_2d.h"
#include "2d/stats_2d.h"
#include "3d/config_3d.h"
#include "3d/config_scope_3d.h"
#include "3d/debug_draw_3d.h"
#include "3d/stats_3d.h"
#include "debug_draw_manager.h"
#include "utils/utils.h"
#include "version.h"

uint64_t debug_draw_manager_id = 0;

#ifndef DISABLE_DEBUG_RENDERING
#ifdef TOOLS_ENABLED
GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/editor_plugin.hpp>
GODOT_WARNING_RESTORE()

#ifdef TELEMETRY_ENABLED
#include "editor/dst_modules/GDExtension/usage_time_reporter.h"
DEFINE_TELEMETRY_OBJECT_ID(dd3d_usage_obj_id);
#endif

#include "editor/editor_menu_extensions.h"

#include "editor/asset_library_update_checker.h"
Ref<_DebugDraw3DAssetLibraryUpdateChecker> upd_checker;
#endif
#endif
using namespace godot;

/** GDExtension Initialize **/
void initialize_debug_draw_3d_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
#if defined(TRACY_DELAYED_INIT) && defined(TRACY_MANUAL_LIFETIME)
		tracy::StartupProfiler();
		tracy::GetProfiler().SetProgramName("libdd3d");
#endif

#ifndef DISABLE_DEBUG_RENDERING
		// TODO register as unexposed
		ClassDB::register_class<_DD3D_PhysicsWatcher>();
		ClassDB::register_class<_DD3D_WorldWatcher>();
#endif

		ClassDB::register_class<DebugDraw2D>();
		ClassDB::register_class<DebugDraw2DStats>();
		ClassDB::register_class<DebugDraw2DConfig>();

		ClassDB::register_class<DebugDraw3D>();
		ClassDB::register_class<DebugDraw3DStats>();
		ClassDB::register_class<DebugDraw3DConfig>();
		ClassDB::register_class<DebugDraw3DScopeConfig>();

		ClassDB::register_class<DebugDrawManager>();

		// Since this manager is a node in the scene tree,
		// it will already be destroyed at the time of cleaning this library.
		DebugDrawManager *debug_draw_manager = memnew(DebugDrawManager);
		debug_draw_manager->init();
		debug_draw_manager_id = debug_draw_manager->get_instance_id();
	}

#ifndef DISABLE_DEBUG_RENDERING
#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		ClassDB::register_class<DebugDrawMenuExtensionPlugin>();
		EditorPlugins::add_by_type<DebugDrawMenuExtensionPlugin>();

		// TODO register as unexposed
		ClassDB::register_class<_DebugDraw3DAssetLibraryUpdateChecker>();
		upd_checker.instantiate();

#ifdef TELEMETRY_ENABLED
		INIT_EDITOR_TELEMETRY_OBJECT(dd3d_usage_obj_id, "Debug Draw 3D", TELEMETRY_APP_ID, DD3D_VERSION_STR, Utils::root_settings_section, TELEMETRY_HOST, "telemetry_dd3d.json");
#endif
	}
#endif
#endif
}

/** GDExtension Uninitialize **/
void uninitialize_debug_draw_3d_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		// If this library is disabled manually before deleting the scene tree (hot-reload),
		// then an attempt is made to delete this node manually.
		DebugDrawManager *debug_draw_manager = Object::cast_to<DebugDrawManager>(ObjectDB::get_instance(debug_draw_manager_id));
		if (debug_draw_manager) {
			memdelete(debug_draw_manager);
		}
		debug_draw_manager_id = 0;

#if defined(TRACY_DELAYED_INIT) && defined(TRACY_MANUAL_LIFETIME)
		tracy::ShutdownProfiler();
#endif
	}

#ifndef DISABLE_DEBUG_RENDERING
#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		upd_checker.unref();
		// EditorPlugins::remove_by_type<DebugDrawMenuExtensionPlugin>();

#ifdef TELEMETRY_ENABLED
		DELETE_EDITOR_TELEMETRY_OBJECT(dd3d_usage_obj_id);
#endif
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

// Orphan StringName: Node (static: 7, total: 8)
// StringName : 1 unclaimed string names at exit.
// TODO https://github.com/godotengine/godot/pull/83619#:~:text=This%20still%20leaves%20one%20reported%20orphan%20string%3A

// TODO godot bug?
// ERROR: Pages in use exist at exit in PagedAllocator: N7Variant5Pools12BucketMediumE
//	at : ~PagedAllocator(./ core / templates / paged_allocator.h : 170)