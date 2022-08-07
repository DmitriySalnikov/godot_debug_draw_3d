#pragma once

#include <vector>

#include <EditorPlugin.hpp>
#include <Godot.hpp>
#include <ViewportContainer.hpp>

using namespace godot;

class DebugDraw3DEditorPlugin : public EditorPlugin {
	GODOT_CLASS(DebugDraw3DEditorPlugin, EditorPlugin)

	// SpatialEditorViewportContainer
	//	* SpatialEditorViewport
	//		* ViewportContainer
	//			* Viewport
	//	* SpatialEditorViewport
	//		* same..
	//	* SpatialEditorViewport
	//		* same..
	//	* SpatialEditorViewport
	//		* same..
	Control* spatial_viewport_container = nullptr;

public:
	static void _register_methods();
	void _init();

	void on_scene_changed(Node *node);
	void remove_prev_node();
	void create_new_node(Node *parent);
	void create_auto_find();
	void find_viewport_control();

	std::vector<Viewport*> get_custom_viewports();
	CanvasItem* get_custom_canvas();

	void _enter_tree();
	void _exit_tree();
	void disable_plugin();
};
