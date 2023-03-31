/* register_types.cpp */

#include "data_graphs.h"
#include "debug_draw.h"
#include "debug_draw_config_2d.h"
#include "debug_draw_config_3d.h"
#include "draw_stats.h"
#include "utils.h"

using namespace godot;

DebugDraw *debug_draw_3d_singleton = nullptr;

/** GDExtension Initialize **/
extern "C" void GDE_EXPORT initialize_debug_draw_3d_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ClassDB::register_class<DebugDraw>();
	ClassDB::register_class<DebugDrawConfig2D>();
	ClassDB::register_class<DebugDrawConfig3D>();
	ClassDB::register_class<DebugDrawGraph>();
	ClassDB::register_class<DebugDrawFPSGraph>();
	ClassDB::register_class<DebugDrawSceneManager>();
	ClassDB::register_class<DebugDrawStats>();

	debug_draw_3d_singleton = memnew(DebugDraw);
	Engine::get_singleton()->register_singleton(TEXT(DebugDraw), debug_draw_3d_singleton);
	Engine::get_singleton()->register_singleton("Dbg3", debug_draw_3d_singleton);

	// TODO: test ability to create EditorPlugin on MODULE_INITIALIZATION_LEVEL_EDITOR 😅
}

/** GDExtension Uninitialize **/
extern "C" void GDE_EXPORT uninitialize_debug_draw_3d_module(ModuleInitializationLevel p_level) {
	if (debug_draw_3d_singleton) {
		Engine::get_singleton()->unregister_singleton(TEXT(DebugDraw));
		Engine::get_singleton()->unregister_singleton("Dbg3");
		memfree(debug_draw_3d_singleton);
		debug_draw_3d_singleton = nullptr;

		UtilityFunctions::push_warning("\n[DebugDraw] The next error is related to https://github.com/godotengine/godot-cpp/issues/914\n"
			"Also, closing the window may be slow due to the crash associated with the unregistration of the GDExtension singleton.\n"
			"I advise you to close the project using the \"Stop\" button in the editor.\n");
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
