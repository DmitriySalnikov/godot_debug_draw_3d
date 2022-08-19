#include "editor_plugin.h"

#include "debug_draw.h"

#include "utils.h"
#include <ConfigFile.hpp>
#include <Control.hpp>
#include <Directory.hpp>
#include <Engine.hpp>
#include <File.hpp>
#include <GlobalConstants.hpp>
#include <ProjectSettings.hpp>
#include <SceneTree.hpp>
#include <functional>

void DebugDraw3DEditorPlugin::_register_methods() {
	register_method("on_scene_changed", &DebugDraw3DEditorPlugin::on_scene_changed);

	register_method("disable_plugin", &DebugDraw3DEditorPlugin::disable_plugin);
	register_method("_enter_tree", &DebugDraw3DEditorPlugin::_enter_tree);
	register_method("_exit_tree", &DebugDraw3DEditorPlugin::_exit_tree);
}

void DebugDraw3DEditorPlugin::_init() {
}

void DebugDraw3DEditorPlugin::on_scene_changed(Node *node) {
	if (!node) return;

	create_new_node(node);
}

void DebugDraw3DEditorPlugin::remove_prev_node() {
	DebugDraw3D *dbg3d = DebugDraw3D::get_singleton();
	if (dbg3d && !dbg3d->is_queued_for_deletion()) {
		dbg3d->free();
	}
}

void DebugDraw3DEditorPlugin::create_new_node(Node *parent) {
	remove_prev_node();
	if (!DebugDraw3D::get_singleton()) {
		find_viewport_control();

		DebugDraw3D *d = DebugDraw3D::_new();
		parent->add_child(d);

		if (spatial_viewport_container) {
			d->set_custom_editor_viewport(get_custom_viewports());

			auto canvas = get_custom_canvas();
			d->set_custom_canvas(canvas);

			if (canvas) {
				canvas->set_meta("UseParentSize", true);
				canvas->update();
			}
		}
	}
}

void DebugDraw3DEditorPlugin::create_auto_find() {
	remove_prev_node();
	Node *node = cast_to<SceneTree>(Engine::get_singleton()->get_main_loop())->get_edited_scene_root();
	if (node) {
		create_new_node(node);
	}
}

// HACK for finding canvas and drawing on it
void DebugDraw3DEditorPlugin::find_viewport_control() {
	// Create temp control to get spatial viewport
	Control *ctrl = Control::_new();
	ctrl->set_name("MY_BEST_CONTROL_NODE!");

	add_control_to_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, ctrl);

	Control *spatial_editor = nullptr;

	int64_t hex_version = (int64_t)(Engine::get_singleton()->get_version_info()["hex"]);
	if (hex_version >= 0x030400) {
		// For Godot 3.4.x
		spatial_editor = cast_to<Control>(ctrl->get_parent()->get_parent()->get_parent()->get_parent());
	} else {
		// For Godot 3.3.x and lower
		spatial_editor = cast_to<Control>(ctrl->get_parent()->get_parent());
	}

	// Remove and destroy temp control
	remove_control_from_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, ctrl);
	ctrl->free();

	spatial_viewport_container = nullptr;
	if (spatial_editor->get_class() == "SpatialEditor") {
		// Try to recursively find `SpatialEditorViewport`

		std::function<Control *(Control *, int)> get;
		get = [this, &get](Control *control, int level) -> Control * {
			if (control->get_class() == "SpatialEditorViewport")
				return control;

			// 4 Levels must be enough for 3.2.4
			if (level < 4) {
				Array ch = control->get_children();
				for (int i = 0; i < ch.size(); i++) {
					Control *obj = cast_to<Control>(ch[i]);
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
			spatial_viewport_container = cast_to<Control>(node->get_parent());
		}
	}
}

std::vector<Viewport *> DebugDraw3DEditorPlugin::get_custom_viewports() {
	Array children = spatial_viewport_container->get_children();
	std::vector<Viewport *> viewports(children.size());
	for (int i = 0; i < children.size(); i++) {
		// SpatialEditorViewportContainer	->SpatialEditorViewport	->ViewportContainer	->Viewport
		// spatial_viewport_container		->one_of_children		->child(0)			->child(0)
		viewports[i] = cast_to<Viewport>(cast_to<Control>(((Object *)children[i]))->get_child(0)->get_child(0));
	}

	return viewports;
}

CanvasItem *DebugDraw3DEditorPlugin::get_custom_canvas() {
	// SpatialEditorViewportContainer	->SpatialEditorViewport	->ViewportContainer
	// spatial_viewport_container		->one_of_children		->child(0)
	return cast_to<CanvasItem>(spatial_viewport_container->get_child(0)->get_child(0));
}

void DebugDraw3DEditorPlugin::_enter_tree() {
	connect("scene_changed", this, TEXT(on_scene_changed));
}

void DebugDraw3DEditorPlugin::_exit_tree() {
	disconnect("scene_changed", this, TEXT(on_scene_changed));

	remove_prev_node();
}

void DebugDraw3DEditorPlugin::disable_plugin() {
	DEBUG_PRINT(TEXT(DebugDraw3DEditorPlugin) " disabled");
}
