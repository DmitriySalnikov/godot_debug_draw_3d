#include "debug_draw_manager.h"

#include "2d/debug_draw_2d.h"
#include "2d/grouped_text.h"
#include "3d/debug_draw_3d.h"
#include "native_api/c_api.h"
#include "utils/utils.h"
#include "version.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

#ifndef DISABLE_DEBUG_RENDERING
void _DD3D_PhysicsWatcher::init(DebugDrawManager *p_root) {
	root_node = p_root;
	set_process_priority(INT32_MIN);
	set_physics_process_priority(INT32_MIN);
}

void _DD3D_PhysicsWatcher::_process(double p_delta) {
	root_node->_process_start(p_delta);
}

void _DD3D_PhysicsWatcher::_physics_process(double p_delta) {
	root_node->_physics_process_start(p_delta);
}
#endif

DebugDrawManager *DebugDrawManager::singleton = nullptr;

void DebugDrawManager::_bind_methods() {
#define REG_CLASS_NAME DebugDrawManager

#ifdef NATIVE_API_ENABLED
	ClassDB::bind_method(D_METHOD(NAMEOF(_get_native_classes)), &DebugDrawManager::_get_native_classes);
	ClassDB::bind_method(D_METHOD(NAMEOF(_get_native_functions)), &DebugDrawManager::_get_native_functions);
	ClassDB::bind_method(D_METHOD(NAMEOF(_get_native_functions_is_double)), &DebugDrawManager::_get_native_functions_is_double);
	ClassDB::bind_method(D_METHOD(NAMEOF(_get_native_functions_hash)), &DebugDrawManager::_get_native_functions_hash);
#endif

	ClassDB::bind_method(D_METHOD(NAMEOF(clear_all)), &DebugDrawManager::clear_all);
	ClassDB::bind_method(D_METHOD(NAMEOF(get_addon_version)), &DebugDrawManager::get_addon_version);
	ClassDB::bind_method(D_METHOD(NAMEOF(get_addon_version_str)), &DebugDrawManager::get_addon_version_str);

	REG_PROP_BOOL(debug_enabled);

	ADD_SIGNAL(MethodInfo(s_extension_unloading));

#undef REG_CLASS_NAME
}

Node *DebugDrawManager::get_current_scene() {
#ifndef DISABLE_DEBUG_RENDERING
	auto ST = SCENE_TREE();
	if (IS_EDITOR_HINT()) {
		return ST->get_edited_scene_root();
	}
	return ST->get_current_scene();
#else
	return nullptr;
#endif
}

void DebugDrawManager::_connect_scene_changed() {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	// Skip when exiting the tree and finish this loop
	if (is_closing) {
		return;
	}

	Node *scene = get_current_scene();
	if (scene) {
		scene->connect(StringName("tree_exiting"), callable_mp(this, &DebugDrawManager::_on_scene_changed).bind(false), CONNECT_ONE_SHOT | CONNECT_DEFERRED);
		return;
	}

	SCENE_TREE()->connect(StringName("tree_changed"), callable_mp(this, &DebugDrawManager::_on_scene_changed).bind(true), CONNECT_ONE_SHOT | CONNECT_DEFERRED);
#endif
}

void DebugDrawManager::_on_scene_changed(bool p_is_scene_null) {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	if (!is_current_scene_is_null || is_current_scene_is_null != p_is_scene_null) {
		DEV_PRINT(NAMEOF(DebugDrawManager) ": Scene changed! clear_all()");
		debug_draw_3d_singleton->clear_all();
		debug_draw_2d_singleton->_clear_all_internal();
	}

	is_current_scene_is_null = p_is_scene_null;
	_connect_scene_changed();
#endif
}

DebugDrawManager::DebugDrawManager() {
}

DebugDrawManager::~DebugDrawManager() {
	UNASSIGN_SINGLETON(DebugDrawManager);
	deinit();

#ifdef NATIVE_API_ENABLED
	NATIVE_API::clear_orphaned_refs();
#endif
}

#ifdef NATIVE_API_ENABLED
Dictionary DebugDrawManager::_get_native_classes() {
	return NATIVE_API::get_functions().get("classes", Dictionary());
}

Dictionary DebugDrawManager::_get_native_functions() {
	return NATIVE_API::get_functions().get("functions", Dictionary());
}

bool DebugDrawManager::_get_native_functions_is_double() {
#ifdef REAL_T_IS_DOUBLE
	return true;
#else
	return false;
#endif
}

int64_t DebugDrawManager::_get_native_functions_hash() {
	return NATIVE_API::get_functions().get("hash", 0);
}
#endif

void DebugDrawManager::clear_all() {
	ZoneScoped;
	if (debug_draw_2d_singleton)
		debug_draw_2d_singleton->clear_all();
	if (debug_draw_3d_singleton)
		debug_draw_3d_singleton->clear_all();
}

void DebugDrawManager::set_debug_enabled(bool value) {
	debug_enabled = value;
	if (!value) {
		clear_all();
	}
}

bool DebugDrawManager::is_debug_enabled() const {
	return debug_enabled;
}

godot::Dictionary DebugDrawManager::get_addon_version() const {
	return Utils::make_dict("major", DD3D_MAJOR, "minor", DD3D_MINOR, "patch", DD3D_PATCH);
}

godot::String DebugDrawManager::get_addon_version_str() const {
	return DD3D_VERSION_STR;
}

String DebugDrawManager::_define_and_update_addon_root_folder() {
	DEFINE_SETTING_AND_GET_HINT(String root_folder, String(Utils::root_settings_section) + Utils::addon_root_folder, "", Variant::STRING, PropertyHint::PROPERTY_HINT_DIR, "");

	if (IS_EDITOR_HINT()) {
		if (root_folder.is_empty()) {
			root_folder = Utils::search_file("res://", "debug_draw_3d.gdextension");

			if (root_folder.is_empty()) {
				root_folder = "res://addons/debug_draw_3d";
				PRINT_WARNING("DD3D: 'debug_draw_3d.gdextension' was not found. The default path will be used: {0}", root_folder);
			} else {
				root_folder = root_folder.get_base_dir();
				PRINT("DD3D: 'debug_draw_3d.gdextension' is found in the folder '{0}'. This folder will be used later.", root_folder);
			}

			PS()->set_setting(String(Utils::root_settings_section) + Utils::addon_root_folder, root_folder);
		}
	}

	return root_folder;
}

void DebugDrawManager::_remove_old_bindings(String addon_folder) const {
#ifdef TOOLS_ENABLED
	if (IS_EDITOR_HINT() && !addon_folder.is_empty()) {
		String dir = addon_folder.path_join("gen/csharp");

		std::vector<String> files = {
			dir.path_join("DebugDrawGeneratedAPI.cs"),
			dir.path_join("DebugDrawGeneratedAPI.cs") + ".uid",
			dir.path_join("DebugDrawGeneratedAPI.generated.cs"),
			dir.path_join("DebugDrawGeneratedAPI.generated.cs") + ".uid",
			dir.path_join("log.txt"),
		};

		for (auto &f : files) {
			if (FileAccess::file_exists(f)) {
				PRINT_WARNING("DD3D: Deletes the file previously generated by this addon: {0}.", f);
				DirAccess::remove_absolute(f);
			}
		}
	}
#endif
}

void DebugDrawManager::init() {
	ZoneScoped;
	ASSIGN_SINGLETON(DebugDrawManager);

	DEFINE_SETTING_AND_GET(bool initial_debug_state, String(Utils::root_settings_section) + s_initial_state, true, Variant::BOOL);
	set_debug_enabled(initial_debug_state);

	String addon_folder = _define_and_update_addon_root_folder();
	_remove_old_bindings(addon_folder);

	// manager_aliases.push_back(StringName("Dmgr"));
	// dd2d_aliases.push_back(StringName("Dbg2"));
	// dd3d_aliases.push_back(StringName("Dbg3"));

	root_settings_section = String(Utils::root_settings_section) + "common/";
	DEFINE_SETTING_AND_GET_HINT(Variant mgr_a, root_settings_section + s_manager_aliases, manager_aliases, Variant::ARRAY, PropertyHint::PROPERTY_HINT_TYPE_STRING, FMT_STR("{0}:", Variant::STRING_NAME));
	DEFINE_SETTING_AND_GET_HINT(Variant dd2d_a, root_settings_section + s_dd2d_aliases, dd2d_aliases, Variant::ARRAY, PropertyHint::PROPERTY_HINT_TYPE_STRING, FMT_STR("{0}:", Variant::STRING_NAME));
	DEFINE_SETTING_AND_GET_HINT(Variant dd3d_a, root_settings_section + s_dd3d_aliases, dd3d_aliases, Variant::ARRAY, PropertyHint::PROPERTY_HINT_TYPE_STRING, FMT_STR("{0}:", Variant::STRING_NAME));

	manager_aliases = TypedArray<StringName>(mgr_a);
	dd2d_aliases = TypedArray<StringName>(dd2d_a);
	dd3d_aliases = TypedArray<StringName>(dd3d_a);

	Engine::get_singleton()->register_singleton(NAMEOF(DebugDrawManager), this);
	_register_singleton_aliases(manager_aliases, this);

	debug_draw_2d_singleton = memnew(DebugDraw2D);
	Engine::get_singleton()->register_singleton(NAMEOF(DebugDraw2D), debug_draw_2d_singleton);
	_register_singleton_aliases(dd2d_aliases, debug_draw_2d_singleton);

	debug_draw_3d_singleton = memnew(DebugDraw3D);
	Engine::get_singleton()->register_singleton(NAMEOF(DebugDraw3D), debug_draw_3d_singleton);
	_register_singleton_aliases(dd3d_aliases, debug_draw_3d_singleton);

	debug_draw_2d_singleton->init(this);
	debug_draw_3d_singleton->init(this);

	callable_mp(this, &DebugDrawManager::_integrate_into_engine).call_deferred();
}

void DebugDrawManager::deinit() {
	ZoneScoped;
	is_closing = true;

	if (Engine::get_singleton()->has_singleton(NAMEOF(DebugDrawManager))) {
		Engine::get_singleton()->unregister_singleton(NAMEOF(DebugDrawManager));
		_unregister_singleton_aliases(manager_aliases);
	}

	if (debug_draw_2d_singleton) {
		Engine::get_singleton()->unregister_singleton(NAMEOF(DebugDraw2D));
		_unregister_singleton_aliases(dd2d_aliases);
		memdelete(debug_draw_2d_singleton);
		debug_draw_2d_singleton = nullptr;
	}

	if (debug_draw_3d_singleton) {
		Engine::get_singleton()->unregister_singleton(NAMEOF(DebugDraw3D));
		_unregister_singleton_aliases(dd3d_aliases);
		memdelete(debug_draw_3d_singleton);
		debug_draw_3d_singleton = nullptr;
	}

	emit_signal(s_extension_unloading);
}

void DebugDrawManager::_register_singleton_aliases(const TypedArray<StringName> &p_names, Object *p_instance) {
	for (int i = 0; i < p_names.size(); i++) {
		if (!p_names[i].operator godot::StringName().is_empty())
			Engine::get_singleton()->register_singleton(p_names[i], p_instance);
	}
}

void DebugDrawManager::_unregister_singleton_aliases(const TypedArray<StringName> &p_names) {
	for (int i = 0; i < p_names.size(); i++) {
		if (!p_names[i].operator godot::StringName().is_empty())
			Engine::get_singleton()->unregister_singleton(p_names[i]);
	}
}

void DebugDrawManager::_integrate_into_engine() {
	ZoneScoped;

	if (!SCENE_TREE()) {
		// Running via the console with a custom `MainLoop` or with `--check-only`
		set_debug_enabled(false);
		return;
	}

	// disable by default for headless
	{
		if (Engine::get_singleton()->has_singleton("DisplayServer")) {
			Object *display_server = Engine::get_singleton()->get_singleton("DisplayServer");
			if (display_server) {
				bool is_enabled = display_server->call("window_can_draw", 0);
				set_debug_enabled(is_enabled);
			}
		} else {
			set_debug_enabled(false);
		}
	}

	// Draw everything after calls from scripts to avoid lagging
	set_process_priority(INT32_MAX);
	set_physics_process_priority(INT32_MAX);

	if (IS_EDITOR_HINT()) {
		// Moving the manager to position 0 in the editor makes it impossible to change the editor's theme.
		// https://github.com/godotengine/godot/blob/d33da79d3f8fe84be2521d25b9ba8e440cf25a88/editor/editor_settings.cpp#L2052-L2057
		if (Node *res = Utils::find_node_by_class(SCENE_ROOT(), "EditorNode"); res) {
			res->add_child(this);
		} else {
			SCENE_ROOT()->add_child(this);
		}
	} else {
		SCENE_ROOT()->add_child(this);
		SCENE_ROOT()->move_child(this, 0);
	}

#ifndef DISABLE_DEBUG_RENDERING
	{
		// will be auto deleted as child
		_DD3D_PhysicsWatcher *physics_watcher = memnew(_DD3D_PhysicsWatcher);
		add_child(physics_watcher);
		physics_watcher->init(this);
	}

	// Useful nodes and names:
	// CanvasItemEditor - probably 2D editor viewport
	// Node3DEditorViewportContainer - base 3D viewport
	// Node3DEditorViewportContainer/Node3DEditorViewport - base of 1 of 4 viewports
	// Node3DEditorViewportContainer/Node3DEditorViewport/SubViewportContainer/SubViewport/Camera3D

	// Funny hack to get default font
	{
		Control *c = memnew(Control);
		SCENE_ROOT()->add_child(c);
		debug_draw_2d_singleton->_font = c->get_theme_default_font();
		memdelete(c);
	}

	// Setup default text group
	debug_draw_2d_singleton->grouped_text->end_text_group();

	if (IS_EDITOR_HINT()) {
		Node *res = Utils::find_node_by_class(SCENE_ROOT(), "Node3DEditorViewportContainer");

		Node *n = res->get_child(0)->get_child(0);
		n->set_meta("UseParentSize", true);
		debug_draw_2d_singleton->default_control_id = n->get_instance_id();

		// actual tree for godot 4.0 beta 14
		//
		//	0. VSplitContainer
		//		0. Node3DEditorViewportContainer >>> res
		//			0. Node3DEditorViewport
		//				0. SubViewportContainer >>> res->get_child(0)->get_child(0)
		//					0. SubViewport
		//						0. Camera3D
		//				1. ...
		//			1. Node3DEditorViewport
		//				0. SubViewportContainer
		//					0. SubViewport
		//  ......

		std::vector<SubViewport *> editor_viewports;
		TypedArray<Node> viewports_root = res->get_children();
		for (int i = 0; i < viewports_root.size(); i++) {
			Node *node = cast_to<Node>(viewports_root[i]);
			if (node) {
				SubViewport *sub_view = cast_to<SubViewport>(Utils::find_node_by_class(node, SubViewport::get_class_static()));
				if (sub_view) {
					editor_viewports.push_back(sub_view);
				}
			}
		}
		debug_draw_3d_singleton->set_custom_editor_viewport(editor_viewports);

		if (editor_viewports.size()) {
			debug_draw_3d_singleton->scoped_config()->set_viewport(editor_viewports[0]);
		}

		/*
		// Used to explore the editor tree.
		auto f = FileAccess::open("user://tree.txt", FileAccess::WRITE);
		f->store_string(Utils::get_scene_tree_as_string(res->get_parent()->get_parent()));
		*/
	} else {
		set_layer(64);

		auto default_control = memnew(Control);
		default_control->set_name("DebugDrawDefaultCanvas");
		((Control *)default_control)->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
		((Control *)default_control)->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
		debug_draw_2d_singleton->default_control_id = default_control->get_instance_id();

		// will be auto deleted as child
		add_child(default_control);

		debug_draw_3d_singleton->scoped_config()->set_viewport(SCENE_ROOT());
	}

	debug_draw_2d_singleton->set_custom_canvas(nullptr);
	_connect_scene_changed();
#endif
}

void DebugDrawManager::_process_start(double p_delta) {
	ZoneScoped;
	FrameMark;

	if (debug_enabled) {
		debug_draw_3d_singleton->process_start(p_delta);
		debug_draw_2d_singleton->process_start(p_delta);
	}
}

void DebugDrawManager::_process(double p_delta) {
	// To discover what causes the constant look here:
	// https://github.com/godotengine/godot/blob/baf6b4634d08bc3e193a38b86e96945052002f64/servers/rendering/rendering_server_default.h#L104
	// Replace _changes_changed's body with:
	// __declspec(noinline) void RenderingServerDefault::_changes_changed() {
	//	#if 0
	//		auto *asd = memnew(Object);
	//		memdelete(asd);
	//	#endif
	//	}
#ifndef DISABLE_DEBUG_RENDERING
	if (debug_enabled) {
		debug_draw_3d_singleton->process_end(p_delta);
		debug_draw_2d_singleton->process_end(p_delta);
	}
#endif

	log_flush_time += p_delta;
	if (log_flush_time > 0.25f) {
		log_flush_time -= 0.25f;
		Utils::print_logs();
	}
}

void DebugDrawManager::_physics_process_start(double p_delta) {
	if (debug_enabled) {
		debug_draw_3d_singleton->physics_process_start(p_delta);
		debug_draw_2d_singleton->physics_process_start(p_delta);
	}
}

void DebugDrawManager::_physics_process(double p_delta) {
#ifndef DISABLE_DEBUG_RENDERING
	if (debug_enabled) {
		debug_draw_3d_singleton->physics_process_end(p_delta);
		debug_draw_2d_singleton->physics_process_end(p_delta);
	}
#endif
}
