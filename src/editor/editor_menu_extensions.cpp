#ifdef TOOLS_ENABLED

#include "editor_menu_extensions.h"
#include "generate_csharp_bindings.h"
#include "utils/utils.h"
#include "version.h"

GODOT_WARNING_DISABLE()
#include "gen/editor_resources.gen.h"
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
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
	ZoneScoped;
	DEV_PRINT_STD("%s _enter_tree\n", NAMEOF(DebugDrawMenuExtensionPlugin));

	PopupMenu *menu = memnew(PopupMenu);
	menu->connect("id_pressed", Callable(this, NAMEOF(_on_id_pressed)));
	menu->add_item("Open the project page", OPEN_LIBRARY_SITE);
	menu->add_item("Open the documentation", OPEN_DOCUMENTATION_SITE);
	menu->add_item("Generate C# bindings", GENERATE_CSHARP_BINDING);

	add_tool_submenu_item(menu_item_name, menu);

	// HACK to change item icon
	PopupMenu *parent = cast_to<PopupMenu>(menu->get_parent());

	Ref<Image> img;
	img.instantiate();
	img->load_png_from_buffer(Utils::convert_to_packed_array<PackedByteArray>(DD3DEditorResources::images_icon_3d_32_png));
	int icon_size = parent->get_theme_constant("class_icon_size", "Editor");
	parent->add_theme_constant_override("icon_max_width", icon_size ? icon_size : 16);
	parent->set_item_icon(parent->get_item_count() - 1, ImageTexture::create_from_image(img));
}

void DebugDrawMenuExtensionPlugin::_exit_tree() {
	// print on exit == crash
	// DEV_PRINT(NAMEOF(DebugDrawMenuExtensionPlugin) " _exit_tree");

	remove_tool_menu_item(menu_item_name);
}

void DebugDrawMenuExtensionPlugin::_on_id_pressed(MenuItemId p_id) {
	ZoneScoped;
	switch (p_id) {
		case DebugDrawMenuExtensionPlugin::OPEN_LIBRARY_SITE: {
			OS::get_singleton()->shell_open("https://github.com/DmitriySalnikov/godot_debug_draw_3d");
			break;
		}
		case DebugDrawMenuExtensionPlugin::OPEN_DOCUMENTATION_SITE: {
			OS::get_singleton()->shell_open("https://dd3d.dmitriysalnikov.ru/docs/" DD3D_VERSION_STR);
			break;
		}
		case DebugDrawMenuExtensionPlugin::GENERATE_CSHARP_BINDING: {
			GenerateCSharpBindingsPlugin().generate();
			break;
		}
		default:
			PRINT_ERROR("Menu item " + String::num_int64(p_id) + " not implemented.");
			break;
	}
}

DebugDrawMenuExtensionPlugin::DebugDrawMenuExtensionPlugin() {
	DEV_PRINT_STD("%s constructor\n", NAMEOF(DebugDrawMenuExtensionPlugin));

	menu_item_name = "Debug Draw";
}

DebugDrawMenuExtensionPlugin::~DebugDrawMenuExtensionPlugin() {
	// DEV_PRINT_STD(NAMEOF(DebugDrawMenuExtensionPlugin) " deconstructor\n");
	// remove_tool_menu_item(menu_item_name);
}

#endif