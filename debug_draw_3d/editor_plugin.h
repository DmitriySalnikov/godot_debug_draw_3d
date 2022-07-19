#pragma once

#include <EditorPlugin.hpp>
#include <Godot.hpp>
#include <ViewportContainer.hpp>

using namespace godot;

class DebugDraw3DEditorPlugin : public EditorPlugin {
	GODOT_CLASS(DebugDraw3DEditorPlugin, EditorPlugin)

	ViewportContainer* spatial_viewport = nullptr;

public:
	static void _register_methods();
	void _init();

	void on_scene_changed(Node *node);
	void remove_prev_node();
	void create_new_node(Node *parent);
	void create_auto_find();
	void find_viewport_control();

	void _enter_tree();
	void _exit_tree();
	void disable_plugin();
};
