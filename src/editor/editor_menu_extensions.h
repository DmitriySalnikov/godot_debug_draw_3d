#pragma once

#ifdef TOOLS_ENABLED
#include "utils/compiler.h"
#include "version.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/editor_plugin.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class DebugDrawMenuExtensionPlugin : public EditorPlugin {
	GDCLASS(DebugDrawMenuExtensionPlugin, EditorPlugin)
public:
	enum MenuItemId {
		MENU_OPEN_ABOUT_WINDOW,
		MENU_OPEN_LIBRARY_SITE,
		MENU_OPEN_REPORT_ISSUE,
		MENU_OPEN_DOCUMENTATION_SITE,
		MENU_GENERATE_CSHARP_BINDING,
	};

	enum AboutAction {
		ABOUT_OPEN_BOOSTY,
		ABOUT_COPY_USDT,
	};

private:
	String menu_item_name;
	class godot::Window *about_window = nullptr;

	constexpr static const char *url_project_page = "https://github.com/DmitriySalnikov/godot_debug_draw_3d";
	constexpr static const char *url_report_issue = "https://github.com/DmitriySalnikov/godot_debug_draw_3d/issues/new/choose";
	constexpr static const char *url_docs_page = "https://dd3d.dmitriysalnikov.ru/docs/" DD3D_VERSION_STR;
	constexpr static const char *url_boosty_page = "https://boosty.to/dmitriysalnikov/donate";
	constexpr static const char *url_asset_library_page = "https://godotengine.org/asset-library/asset/1766";
	constexpr static const char *address_usdt = "TEw934PrsffHsAn5M63SoHYRuZo984EF6v";

protected:
	static void _bind_methods() {};
	void open_about_window();
	void _on_closing_about_window();
	static void _on_copy_clicked(const String copy_string, const uint64_t root_id);

public:
	String _get_plugin_name() const override;
	// Ref<Texture2D> _get_plugin_icon() const override;
	void _enter_tree() override;
	void _exit_tree() override;

	void _on_id_pressed(int p_id);
	void _on_gui_input(const class Ref<godot::InputEvent> ie, int action_id, uint64_t root_id);

	DebugDrawMenuExtensionPlugin();
	~DebugDrawMenuExtensionPlugin();
};

#endif
