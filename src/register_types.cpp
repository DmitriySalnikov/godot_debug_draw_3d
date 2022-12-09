/* register_types.cpp */

#ifndef NO_EDITOR
#include "editor_plugin.h"
#endif
#include "data_graphs.h"
#include "debug_draw.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/editor_interface.hpp>

using namespace godot;

DebugDraw *debug_draw_3d_singleton = nullptr;

/** GDNative Initialize **/
extern "C" void GDN_EXPORT initialize_debug_draw_3d_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ClassDB::register_class<DebugDraw>();
	ClassDB::register_class<GraphParameters>();
	ClassDB::register_class<DebugDraw3DEditorPlugin>();

	debug_draw_3d_singleton = memnew(DebugDraw);
	Engine::get_singleton()->register_singleton(TEXT(DebugDraw), debug_draw_3d_singleton);
	Engine::get_singleton()->register_singleton("Dbg3", debug_draw_3d_singleton);
	
	// TODO EditorPlugin's is not available in GDExtensions...
	/*
#ifdef TOOLS_ENABLED
	ClassDB::register_class<DebugDraw3DEditorPlugin>();
#endif
	*/
}

/** GDNative Uninitialize **/
extern "C" void GDN_EXPORT uninitialize_debug_draw_3d_module(ModuleInitializationLevel p_level) {
	if (debug_draw_3d_singleton) {
		Engine::get_singleton()->unregister_singleton(TEXT(DebugDraw3D));
		Engine::get_singleton()->unregister_singleton("Dbg3");
		memfree(debug_draw_3d_singleton);
		debug_draw_3d_singleton = nullptr;
	}
}

/** GDNative Initialize **/
extern "C" {
GDNativeBool GDN_EXPORT debug_draw_3d_library_init(const GDNativeInterface *p_interface, const GDNativeExtensionClassLibraryPtr p_library, GDNativeInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

	init_obj.register_initializer(initialize_debug_draw_3d_module);
	init_obj.register_terminator(uninitialize_debug_draw_3d_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
