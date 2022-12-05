#include "editor_plugin.h"

#include "debug_draw.h"
#include "utils.h"

#include <functional>

#include <godot_cpp/classes/config_file.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void DebugDraw3DEditorPlugin::_bind_methods() {
	/* TODO pointers is not available..
	ClassDB::bind_method(D_METHOD(TEXT(_on_scene_changed)), &DebugDraw3DEditorPlugin::_on_scene_changed);
	*/

	ClassDB::bind_method(D_METHOD(TEXT(disable_plugin)), &DebugDraw3DEditorPlugin::disable_plugin);
}

DebugDraw3DEditorPlugin::DebugDraw3DEditorPlugin() {
}

void DebugDraw3DEditorPlugin::_on_scene_changed(Node *node) {
	if (!node) return;

	create_new_node(node);
}

void DebugDraw3DEditorPlugin::remove_prev_node() {
	DebugDraw *dbg3d = DebugDraw::get_singleton();
	if (dbg3d && !dbg3d->is_queued_for_deletion()) {
		memfree(dbg3d);
	}
}

void DebugDraw3DEditorPlugin::create_new_node(Node *parent) {
	remove_prev_node();
	if (!DebugDraw::get_singleton()) {
		find_viewport_control();

		/* DebugDraw *d = memnew(DebugDraw); TODO move to DebugDraw
		parent->add_child(d);

		if (spatial_viewport_container) {
			d->set_custom_editor_viewport(get_custom_viewports());

			auto canvas = get_custom_canvas();
			d->set_custom_canvas(canvas);

			if (canvas) {
				canvas->set_meta("UseParentSize", true);
				canvas->queue_redraw();
			}
		}
		*/
	}
}

void DebugDraw3DEditorPlugin::create_auto_find() {
	remove_prev_node();
	Node *node = Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop())->get_edited_scene_root();
	if (node) {
		create_new_node(node);
	}
}

// HACK For finding canvas and drawing on it
void DebugDraw3DEditorPlugin::find_viewport_control() {
	// Create temp control to get spatial viewport
	Control *ctrl = memnew(Control);
	ctrl->set_name("MY_BEST_CONTROL_NODE!");

	add_control_to_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, ctrl);

	Control *spatial_editor = nullptr;

	int64_t hex_version = (int64_t)(Engine::get_singleton()->get_version_info()["hex"]);

	if (hex_version >= 0x030400) {
		// For Godot 3.4.x
		spatial_editor = Object::cast_to<Control>(ctrl->get_parent()->get_parent()->get_parent()->get_parent());
	} else {
		// For Godot 3.3.x and lower
		spatial_editor = Object::cast_to<Control>(ctrl->get_parent()->get_parent());
	}

	// Remove and destroy temp control
	remove_control_from_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, ctrl);
	memfree(ctrl);

	spatial_viewport_container = nullptr;
	if (spatial_editor->get_class() == "SpatialEditor") {
		// Try to recursively find `SpatialEditorViewport`

		std::function<Control *(Control *, int)> get;
		get = [this, &get](Control *control, int level) -> Control * {
			if (control->get_class() == "SpatialEditorViewport")
				return control;

			// 5 Levels must be enough for 3.5...
			if (level < 5) {
				Array ch = control->get_children();
				for (int i = 0; i < ch.size(); i++) {
					Control *obj = Object::cast_to<Control>(ch[i]);
					if (obj) {
						Control *res = get(obj, level + 1);
						if (res)
							return res;
					}
				}
			}

			return nullptr;
		};

		Control *node = get(spatial_editor, 0);
		if (node) {
			// SpatialEditorViewportContainer	<-SpatialEditorViewport
			// spatial_viewport_container		<-node
			spatial_viewport_container = Object::cast_to<Control>(node->get_parent());
		}
	}
}

std::vector<Viewport *> DebugDraw3DEditorPlugin::get_custom_viewports() {
	Array children = spatial_viewport_container->get_children();
	std::vector<Viewport *> viewports(children.size());
	for (int i = 0; i < children.size(); i++) {
		// SpatialEditorViewportContainer	->SpatialEditorViewport	->ViewportContainer	->Viewport
		// spatial_viewport_container		->one_of_children		->child(0)			->child(0)
		viewports[i] = Object::cast_to<Viewport>(Object::cast_to<Control>(((Object *)children[i]))->get_child(0)->get_child(0));
	}

	return viewports;
}

CanvasItem *DebugDraw3DEditorPlugin::get_custom_canvas() {
	// SpatialEditorViewportContainer	->SpatialEditorViewport	->ViewportContainer
	// spatial_viewport_container		->one_of_children		->child(0)
	return Object::cast_to<CanvasItem>(spatial_viewport_container->get_child(0)->get_child(0));
}

void DebugDraw3DEditorPlugin::_enter_tree() {
	connect("scene_changed", Callable(this, TEXT(_on_scene_changed)));
}

void DebugDraw3DEditorPlugin::_exit_tree() {
	disconnect("scene_changed", Callable(this, TEXT(_on_scene_changed)));

	remove_prev_node();
}

void DebugDraw3DEditorPlugin::disable_plugin() {
	DEBUG_PRINT(TEXT(DebugDraw3DEditorPlugin) " disabled");
}
