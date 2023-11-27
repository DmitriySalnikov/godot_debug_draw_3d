#pragma once

#ifdef TOOLS_ENABLED
#include "utils/compiler.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/editor_plugin.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class DebugDrawMenuExtensionPlugin : public EditorPlugin {
	GDCLASS(DebugDrawMenuExtensionPlugin, EditorPlugin)
public:
	enum MenuItemId {
		OPEN_LIBRARY_SITE = 1,
		GENERATE_CSHARP_BINDING = 2,
	};

private:
	String menu_item_name;

protected:
	static void _bind_methods();

public:
	String _get_plugin_name() const override;
	// Ref<Texture2D> _get_plugin_icon() const override;
	void _enter_tree() override;
	void _exit_tree() override;

	void _on_id_pressed(MenuItemId id);

	DebugDrawMenuExtensionPlugin();
	~DebugDrawMenuExtensionPlugin();
};

// Register but not expose to GDScript
VARIANT_ENUM_CAST(DebugDrawMenuExtensionPlugin::MenuItemId);

#endif
