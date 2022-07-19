#include "editor_plugin.h"

#include "debug_draw.h"

#include "utils.h"
#include <ConfigFile.hpp>
#include <Control.hpp>
#include <Directory.hpp>
#include <File.hpp>
#include <GlobalConstants.hpp>
#include <SceneTree.hpp>
#include <Engine.hpp>
#include <ProjectSettings.hpp>
#include <functional>

void DebugDraw3DEditorPlugin::_register_methods() {
	register_method("on_scene_changed", &DebugDraw3DEditorPlugin::on_scene_changed);

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

		DebugDraw3D* d = DebugDraw3D::_new();
		parent->add_child(d);
		
		// TODO DebugDraw.CustomViewport = spatial_viewport.GetChild<Viewport>(0);
		// TODO DebugDraw.CustomCanvas = spatial_viewport;
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
// Hardcoded for 3.2.4
void DebugDraw3DEditorPlugin::find_viewport_control() {
	// Create temp control to get spatial viewport
	Control *ctrl = Control::_new();
	add_control_to_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, ctrl);

	// Try to get main viewport node. Must be `SpatialEditor`
	Control *spatial_editor = cast_to<Control>(ctrl->get_parent()->get_parent());

	// Remove and destroy temp control
	remove_control_from_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, ctrl);
	ctrl->free();

	spatial_viewport = nullptr;
	if (spatial_editor->get_class() == "SpatialEditor") {
		// Try to recursively find `SpatialEditorViewport`

		std::function<Control *(Control *, int)> get;
		get = [&](Control *control, int level) -> Control * {
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

		Control *spat_editor = get(spatial_editor, 0);
		if (spatial_editor) {
			spatial_viewport = cast_to<ViewportContainer>(spatial_editor->get_child(0));
		}
	}

	if (spatial_viewport) {
		spatial_viewport->set_meta("UseParentSize", true);
		spatial_viewport->update();
	}
}

void DebugDraw3DEditorPlugin::_enter_tree() {
	connect("scene_changed", this, TEXT(on_scene_changed));
}

void DebugDraw3DEditorPlugin::_exit_tree() {
	disconnect("scene_changed", this, TEXT(on_scene_changed));
	
	remove_prev_node();
}

void DebugDraw3DEditorPlugin::disable_plugin() {
	PRINT_WARNING("hmm");
}
