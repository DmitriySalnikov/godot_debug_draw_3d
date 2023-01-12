/* register_types.cpp */

#ifndef NO_EDITOR
#include "editor_plugin.h"
#endif
#include "data_graphs.h"
#include "debug_draw.h"
#include "utils.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#endif

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/core/class_db.hpp>

#if defined(_MSC_VER)
#pragma warning(default : 4244)
#endif

using namespace godot;

DebugDraw *debug_draw_3d_singleton = nullptr;

/** GDExtension Initialize **/
extern "C" void GDE_EXPORT initialize_debug_draw_3d_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ClassDB::register_class<DebugDraw>();
	ClassDB::register_class<DebugDrawGraph>();
	ClassDB::register_class<DebugDrawFPSGraph>();
	ClassDB::register_class<DebugDrawSceneManager>();

	debug_draw_3d_singleton = memnew(DebugDraw);
	Engine::get_singleton()->register_singleton(TEXT(DebugDraw), debug_draw_3d_singleton);
	Engine::get_singleton()->register_singleton("Dbg3", debug_draw_3d_singleton);

	// TODO: EditorPlugin's is not available in GDExtensions...
	/*
#ifdef TOOLS_ENABLED
	ClassDB::register_class<DebugDraw3DEditorPlugin>();
#endif
	*/
}

/** GDExtension Uninitialize **/
extern "C" void GDE_EXPORT uninitialize_debug_draw_3d_module(ModuleInitializationLevel p_level) {
	if (debug_draw_3d_singleton) {
		Engine::get_singleton()->unregister_singleton(TEXT(DebugDraw3D));
		Engine::get_singleton()->unregister_singleton("Dbg3");
		memfree(debug_draw_3d_singleton);
		debug_draw_3d_singleton = nullptr;
	}
}

/** GDExtension Initialize **/
extern "C" {
GDExtensionBool GDE_EXPORT debug_draw_3d_library_init(const GDExtensionInterface *p_interface, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

	init_obj.register_initializer(initialize_debug_draw_3d_module);
	init_obj.register_terminator(uninitialize_debug_draw_3d_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
