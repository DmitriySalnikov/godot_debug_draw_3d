/* register_types.cpp */

#include "test_api_node.h"

#include "../src/utils/compiler.h"

GODOT_WARNING_DISABLE()
//#include <godot_cpp/classes/editor_plugin.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

/** GDExtension Initialize **/
void initialize_dd3d_test_cpp_api_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		ClassDB::register_class<DD3DTestCppApiNode>();
	}
}

/** GDExtension Uninitialize **/
void uninitialize_dd3d_test_cpp_api_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
	}
}

/** GDExtension Initialize **/
extern "C" {
GDExtensionBool GDE_EXPORT dd3d_test_cpp_api_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_dd3d_test_cpp_api_module);
	init_obj.register_terminator(uninitialize_dd3d_test_cpp_api_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
