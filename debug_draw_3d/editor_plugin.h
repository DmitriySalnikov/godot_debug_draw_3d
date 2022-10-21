#pragma once

#include <vector>

#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/canvas_item.hpp>

using namespace godot;

class DebugDraw3DEditorPlugin : public EditorPlugin {
	GDCLASS(DebugDraw3DEditorPlugin, EditorPlugin)

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
	Control *spatial_viewport_container = nullptr;

protected:
	static void _bind_methods();

public:
	DebugDraw3DEditorPlugin();

	void _on_scene_changed(Node *node);
	void remove_prev_node();
	void create_new_node(Node *parent);
	void create_auto_find();
	void find_viewport_control();

	std::vector<Viewport *> get_custom_viewports();
	CanvasItem *get_custom_canvas();

	void _enter_tree();
	void _exit_tree();
	void disable_plugin();
};
