#ifdef TOOLS_ENABLED

#include "editor_menu_extensions.h"
#include "generate_csharp_bindings.h"
#include "utils/utils.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/classes/os.hpp>
GODOT_WARNING_RESTORE()

void DebugDrawMenuExtensionPlugin::_bind_methods() {
#define REG_CLASS_NAME DebugDrawMenuExtensionPlugin
	REG_METHOD(_on_id_pressed);
#undef REG_CLASS_NAME
}

inline String DebugDrawMenuExtensionPlugin::_get_plugin_name() const {
	return String("Generate C# Binding for DebugDraw");
}

void DebugDrawMenuExtensionPlugin::_enter_tree() {
	DEV_PRINT(NAMEOF(DebugDrawMenuExtensionPlugin) " _enter_tree");

	PopupMenu *menu = memnew(PopupMenu);
	menu->connect("id_pressed", Callable(this, NAMEOF(_on_id_pressed)));
	menu->add_item("Open project site", OPEN_LIBRARY_SITE);
	menu->add_item("Generate C# bindings", GENERATE_CSHARP_BINDING);

	add_tool_submenu_item(menu_item_name, menu);
}

void DebugDrawMenuExtensionPlugin::_exit_tree() {
	// print on exit == crash
	// DEV_PRINT(NAMEOF(DebugDrawMenuExtensionPlugin) " _exit_tree");

	remove_tool_menu_item(menu_item_name);
}

void DebugDrawMenuExtensionPlugin::_on_id_pressed(MenuItemId id) {
	switch (id) {
		case DebugDrawMenuExtensionPlugin::OPEN_LIBRARY_SITE: {
			OS::get_singleton()->shell_open("https://github.com/DmitriySalnikov/godot_debug_draw_3d");
			break;
		}
		case DebugDrawMenuExtensionPlugin::GENERATE_CSHARP_BINDING: {
			GenerateCSharpBindingsPlugin().generate();
			break;
		}
		default:
			PRINT_ERROR("Menu item " + String::num_int64(id) + " not implemented.");
			break;
	}
}

DebugDrawMenuExtensionPlugin::DebugDrawMenuExtensionPlugin() {
	DEV_PRINT(NAMEOF(DebugDrawMenuExtensionPlugin) " constructor");

	menu_item_name = "Debug Draw";
}

#endif