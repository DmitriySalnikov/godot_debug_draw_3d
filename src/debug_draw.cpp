#include "debug_draw.h"

#include "data_graphs.h"
#include "debug_draw_config_2d.h"
#include "debug_draw_config_3d.h"
#include "debug_geometry_container.h"
#include "draw_stats.h"
#include "grouped_text.h"
#include "utils.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/config_file.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/main_loop.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/window.hpp>
GODOT_WARNING_RESTORE()

#include <limits.h>

#define NEED_LEAVE (!debug_enabled || !is_ready)

DebugDraw *DebugDraw::singleton = nullptr;

void DebugDraw::_bind_methods() {
#define REG_CLASS_NAME DebugDraw

	ClassDB::bind_method(D_METHOD(TEXT(_on_canvas_marked_dirty)), &DebugDraw::_on_canvas_marked_dirty);
	ClassDB::bind_method(D_METHOD(TEXT(_on_canvas_item_draw)), &DebugDraw::_on_canvas_item_draw);
	ClassDB::bind_method(D_METHOD(TEXT(_on_scene_changed)), &DebugDraw::_on_scene_changed);
	ClassDB::bind_method(D_METHOD(TEXT(_scene_tree_found)), &DebugDraw::_scene_tree_found);

#pragma region Parameters

	REG_PROP(empty_color, Variant::COLOR);
	REG_PROP_BOOL(debug_enabled);

	REG_PROP(config_2d, Variant::OBJECT);
	REG_PROP(config_3d, Variant::OBJECT);

	REG_PROP(custom_viewport, Variant::OBJECT);
	REG_PROP(custom_canvas, Variant::OBJECT);

#pragma endregion
#undef REG_CLASS_NAME

#pragma region Draw Functions
	ClassDB::bind_method(D_METHOD(TEXT(clear_3d_objects)), &DebugDraw::clear_3d_objects);
	ClassDB::bind_method(D_METHOD(TEXT(clear_2d_objects)), &DebugDraw::clear_2d_objects);

	ClassDB::bind_method(D_METHOD(TEXT(clear_all)), &DebugDraw::clear_all);

	ClassDB::bind_method(D_METHOD(TEXT(draw_sphere), "position", "radius", "color", "duration"), &DebugDraw::draw_sphere, 0.5f, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_sphere_xf), "transform", "color", "duration"), &DebugDraw::draw_sphere_xf, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(TEXT(draw_sphere_hd), "position", "radius", "color", "duration"), &DebugDraw::draw_sphere_hd, 0.5f, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_sphere_hd_xf), "transform", "color", "duration"), &DebugDraw::draw_sphere_hd_xf, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(TEXT(draw_cylinder), "transform", "color", "duration"), &DebugDraw::draw_cylinder, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(TEXT(draw_box), "position", "size", "color", "is_box_centered", "duration"), &DebugDraw::draw_box, Colors::empty_color, false, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_box_xf), "transform", "color", "is_box_centered", "duration"), &DebugDraw::draw_box_xf, Colors::empty_color, true, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_aabb), "aabb", "color", "duration"), &DebugDraw::draw_aabb, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_aabb_ab), "a", "b", "color", "duration"), &DebugDraw::draw_aabb_ab, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(TEXT(draw_line_hit), "start", "end", "hit", "is_hit", "hit_size", "hit_color", "after_hit_color", "duration"), &DebugDraw::draw_line_hit, 0.25f, Colors::empty_color, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_line_hit_offset), "start", "end", "is_hit", "unit_offset_of_hit", "hit_size", "hit_color", "after_hit_color", "duration"), &DebugDraw::draw_line_hit_offset, 0.5f, 0.25f, Colors::empty_color, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(TEXT(draw_line), "a", "b", "color", "duration"), &DebugDraw::draw_line, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_lines), "lines", "color", "duration"), &DebugDraw::draw_lines, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_ray), "origin", "direction", "length", "color", "duration"), &DebugDraw::draw_ray, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_line_path), "path", "color", "duration"), &DebugDraw::draw_line_path, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(TEXT(draw_arrow), "transform", "color", "duration"), &DebugDraw::draw_arrow, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(TEXT(draw_arrow_line), "a", "b", "color", "arrow_size", "is_absolute_size", "duration"), &DebugDraw::draw_arrow_line, Colors::empty_color, 0.5f, false, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_arrow_ray), "origin", "direction", "length", "color", "arrow_size", "is_absolute_size", "duration"), &DebugDraw::draw_arrow_ray, Colors::empty_color, 0.5f, false, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_arrow_path), "path", "color", "arrow_size", "is_absolute_size", "duration"), &DebugDraw::draw_arrow_path, Colors::empty_color, 0.75f, true, 0);

	ClassDB::bind_method(D_METHOD(TEXT(draw_point_path), "path", "size", "points_color", "lines_color", "duration"), &DebugDraw::draw_point_path, 0.25f, Colors::empty_color, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(TEXT(draw_square), "position", "size", "color", "duration"), &DebugDraw::draw_square, 0.2f, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_points), "points", "size", "color", "duration"), &DebugDraw::draw_points, 0.2f, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(TEXT(draw_camera_frustum), "camera", "color", "duration"), &DebugDraw::draw_camera_frustum, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_camera_frustum_planes), "camera_frustum", "color", "duration"), &DebugDraw::draw_camera_frustum_planes, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(TEXT(draw_position), "transform", "color", "duration"), &DebugDraw::draw_position, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(TEXT(draw_gizmo), "transform", "color", "is_centered", "duration"), &DebugDraw::draw_gizmo, Colors::empty_color, false, 0);

	ClassDB::bind_method(D_METHOD(TEXT(draw_grid), "origin", "x_size", "y_size", "subdivision", "color", "is_centered", "duration"), &DebugDraw::draw_grid, Colors::empty_color, true, 0);
	ClassDB::bind_method(D_METHOD(TEXT(draw_grid_xf), "transform", "subdivision", "color", "is_centered", "duration"), &DebugDraw::draw_grid_xf, Colors::empty_color, true, 0);

	ClassDB::bind_method(D_METHOD(TEXT(begin_text_group), "group_title", "group_priority", "group_color", "show_title", "title_size", "text_size"), &DebugDraw::begin_text_group, 0, Colors::empty_color, true, -1, -1);
	ClassDB::bind_method(D_METHOD(TEXT(end_text_group)), &DebugDraw::end_text_group);
	ClassDB::bind_method(D_METHOD(TEXT(set_text), "key", "value", "priority", "color_of_value", "duration"), &DebugDraw::set_text, Variant(), 0, Colors::empty_color, -1.0); // TODO: -1 must be explicitly double. Need fix for Variant converter

	ClassDB::bind_method(D_METHOD(TEXT(create_graph), "title"), &DebugDraw::create_graph);
	ClassDB::bind_method(D_METHOD(TEXT(create_fps_graph), "title"), &DebugDraw::create_fps_graph);
	ClassDB::bind_method(D_METHOD(TEXT(graph_update_data), "title", "data"), &DebugDraw::graph_update_data);
	ClassDB::bind_method(D_METHOD(TEXT(remove_graph), "title"), &DebugDraw::remove_graph);
	ClassDB::bind_method(D_METHOD(TEXT(clear_graphs)), &DebugDraw::clear_graphs);
	ClassDB::bind_method(D_METHOD(TEXT(get_graph), "title"), &DebugDraw::get_graph);
	ClassDB::bind_method(D_METHOD(TEXT(get_graph_names)), &DebugDraw::get_graph_names);

#pragma endregion // Draw Functions

	ClassDB::bind_method(D_METHOD(TEXT(get_render_stats)), &DebugDraw::get_render_stats);
}

DebugDraw::DebugDraw() {
	if (!singleton) {
		singleton = this;
	} else {
		PRINT_ERROR("Only 1 instance of " TEXT(DebugDraw) " is allowed");
	}

	call_deferred(TEXT(_scene_tree_found));
}

DebugDraw::~DebugDraw() {
	if (singleton && singleton == this) {
		singleton = nullptr;
	} else {
		PRINT_ERROR("More than 1 " TEXT(DebugDraw) " instance was created");
	}
}

Node *DebugDraw::_get_current_scene() {
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

void DebugDraw::_scene_tree_found() {
#ifndef DISABLE_DEBUG_RENDERING
	if (!SCENE_TREE())
		return;
#endif

	set_config_2d(nullptr);
	set_config_3d(nullptr);

	root_node = memnew(DebugDrawSceneManager);
	SCENE_ROOT()->add_child(root_node);
	SCENE_ROOT()->move_child(root_node, 0);
}

void DebugDraw::_connect_scene_changed() {
#ifndef DISABLE_DEBUG_RENDERING
	// Skip when exiting the tree and finish this loop
	if (is_closing) {
		return;
	}

	Node *scene = _get_current_scene();
	if (scene && UtilityFunctions::is_instance_valid(scene)) {
		scene->connect(StringName("tree_exiting"), Callable(this, TEXT(_on_scene_changed)).bindv(Array::make(false)), CONNECT_ONE_SHOT | CONNECT_DEFERRED);
		return;
	}

	SCENE_TREE()->connect(StringName("tree_changed"), Callable(this, TEXT(_on_scene_changed)).bindv(Array::make(true)), CONNECT_ONE_SHOT | CONNECT_DEFERRED);
#endif
}

void DebugDraw::_on_scene_changed(bool _is_scene_null) {
#ifndef DISABLE_DEBUG_RENDERING
	if (!is_current_scene_is_null || is_current_scene_is_null != _is_scene_null) {
		DEV_PRINT("Scene changed! clear_all()");
		clear_all();
	}

	is_current_scene_is_null = _is_scene_null;
	_connect_scene_changed();
#endif
}

void DebugDraw::enter_tree() {
#ifndef DISABLE_DEBUG_RENDERING
	root_node->set_process_priority(INT32_MAX);

	grouped_text = std::make_unique<GroupedText>();
	grouped_text->init_group(this);
	data_graphs = std::make_unique<DataGraphManager>(this);
	dgc = std::make_unique<DebugGeometryContainer>(this);
#endif
}

void DebugDraw::exit_tree() {
	is_closing = true;

#ifndef DISABLE_DEBUG_RENDERING
	_font.unref();

	dgc.reset();
	data_graphs.reset();
	grouped_text.reset();

	root_node->queue_free();
	root_node = nullptr;

	if (!IS_EDITOR_HINT()) {
		_canvas_layer->queue_free();
		default_canvas->queue_free();

		_canvas_layer = nullptr;
		default_canvas = nullptr;
	}

	if (Utils::disconnect_safe(default_canvas, "draw", Callable(this, TEXT(_on_canvas_item_draw))))
		default_canvas->queue_redraw();
	if (Utils::disconnect_safe(custom_canvas, "draw", Callable(this, TEXT(_on_canvas_item_draw))))
		custom_canvas->queue_redraw();
#endif

	config_2d.unref();
	config_3d.unref();
}

void DebugDraw::ready() {
	is_ready = true;

#ifndef DISABLE_DEBUG_RENDERING
	// Useful nodes and names:
	// CanvasItemEditor - probably 2D editor viewport
	// Node3DEditorViewportContainer - base 3D viewport
	// Node3DEditorViewportContainer/Node3DEditorViewport - base of 1 of 4 viewports
	// Node3DEditorViewportContainer/Node3DEditorViewport/SubViewportContainer/SubViewport/Camera3D

	// Funny hack to get default font
	{
		Control *c = memnew(Control);
		SCENE_ROOT()->add_child(c);
		_font = c->get_theme_default_font();
		c->queue_free();
	}

	// Setup default text group
	grouped_text->end_text_group();

	if (IS_EDITOR_HINT()) {
		String editor3d = "Node3DEditorViewportContainer";
		String subviewport = SubViewport::get_class_static();
		Node *res = Utils::find_node_by_class(SCENE_ROOT(), editor3d);

		Node *n = res->get_child(0)->get_child(0);
		n->set_meta("UseParentSize", true);
		default_canvas = (Control *)n;

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
				SubViewport *sub_view = cast_to<SubViewport>(Utils::find_node_by_class(node, subviewport));
				if (sub_view) {
					editor_viewports.push_back(sub_view);
				}
			}
		}
		set_custom_editor_viewport(editor_viewports);

		/*
		// Used to explore the editor tree.
		auto f = FileAccess::open("user://tree.txt", FileAccess::WRITE);
		f->store_string(Utils::get_scene_tree_as_string(res->get_parent()->get_parent()));
		*/
	} else {
		// Create canvas item and canvas layer
		_canvas_layer = memnew(CanvasLayer);
		_canvas_layer->set_layer(64);
		default_canvas = memnew(Control);
		default_canvas->set_name("DebugDrawDefaultCanvas");
		((Control *)default_canvas)->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
		((Control *)default_canvas)->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);

		root_node->add_child(_canvas_layer);
		_canvas_layer->add_child(default_canvas);
		root_node->move_child(_canvas_layer, 0);
	}

	set_custom_canvas(custom_canvas);
	_set_base_world_node(SCENE_ROOT());
	_connect_scene_changed();
#endif
}

void DebugDraw::process(double delta) {
#ifndef DISABLE_DEBUG_RENDERING
	// Clean texts
	grouped_text->cleanup_text(delta);

	// FPS Graph
	data_graphs->auto_update_graphs(delta);

	// Update overlay
	if (_canvas_need_update) {
		if (!UtilityFunctions::is_instance_valid(custom_canvas) && UtilityFunctions::is_instance_valid(default_canvas))
			default_canvas->queue_redraw();
		else if (UtilityFunctions::is_instance_valid(custom_canvas))
			custom_canvas->queue_redraw();

		// reset some values
		_canvas_need_update = false;
		grouped_text->end_text_group();
	}

	// Update 3D debug
	dgc->update_geometry(delta);
#endif

	// Print logs if needed
	log_flush_time += delta;
	if (log_flush_time > 0.25f) {
		log_flush_time -= 0.25f;
		Utils::print_logs();
	}
}

void DebugDraw::_on_canvas_marked_dirty() {
	mark_canvas_dirty();
}

void DebugDraw::_on_canvas_item_draw(Control *ci) {
#ifndef DISABLE_DEBUG_RENDERING
	Vector2 vp_size = ci->has_meta("UseParentSize") ? Object::cast_to<Control>(ci->get_parent())->get_rect().size : ci->get_rect().size;

	grouped_text->draw(ci, _font, vp_size);
	data_graphs->draw(ci, _font, vp_size, ci->get_process_delta_time());
#else
	return;
#endif
}

void DebugDraw::_set_base_world_node(Node *_world_base) {
#ifndef DISABLE_DEBUG_RENDERING
	dgc->set_world(_world_base);
#else
	return;
#endif
}

void DebugDraw::mark_canvas_dirty() {
	_canvas_need_update = true;
}

Node *DebugDraw::get_root_node() {
	return root_node;
}

void DebugDraw::set_custom_editor_viewport(std::vector<SubViewport *> _viewports) {
	custom_editor_viewports = _viewports;
}

std::vector<SubViewport *> DebugDraw::get_custom_editor_viewports() {
	return custom_editor_viewports;
}

void DebugDraw::set_empty_color(const Color &_col) {
}

Color DebugDraw::get_empty_color() const {
	return Colors::empty_color;
}

#pragma region Exposed Parameters

void DebugDraw::set_debug_enabled(const bool &_state) {
	debug_enabled = _state;

	if (!_state) {
		clear_all();
	}
}

bool DebugDraw::is_debug_enabled() const {
	return debug_enabled;
}

void DebugDraw::set_config_2d(Ref<DebugDrawConfig2D> _cfg) {
	if (_cfg.is_valid()) {
#ifndef DISABLE_DEBUG_RENDERING
		Utils::disconnect_safe(config_2d, DebugDrawConfig2D::s_marked_dirty, Callable(this, TEXT(_on_canvas_marked_dirty)));
#endif

		config_2d = _cfg;
	} else {
		config_2d = Ref<DebugDrawConfig2D>();
		config_2d.instantiate();
	}

#ifndef DISABLE_DEBUG_RENDERING
	mark_canvas_dirty();
	Utils::connect_safe(config_2d, DebugDrawConfig2D::s_marked_dirty, Callable(this, TEXT(_on_canvas_marked_dirty)));
#endif
}

Ref<DebugDrawConfig2D> DebugDraw::get_config_2d() const {
	return config_2d;
}

void DebugDraw::set_config_3d(Ref<DebugDrawConfig3D> _cfg) {
	if (_cfg.is_valid()) {
		config_3d = _cfg;
	} else {
		config_3d = Ref<DebugDrawConfig3D>();
		config_3d.instantiate();
	}
}

Ref<DebugDrawConfig3D> DebugDraw::get_config_3d() const {
	return config_3d;
}

void DebugDraw::set_custom_viewport(Viewport *_viewport) {
	custom_viewport = _viewport;
}

Viewport *DebugDraw::get_custom_viewport() const {
	return custom_viewport;
}

void DebugDraw::set_custom_canvas(Control *_canvas) {
#ifndef DISABLE_DEBUG_RENDERING
	if (!_canvas) {
		Utils::connect_safe(default_canvas, "draw", Callable(this, TEXT(_on_canvas_item_draw)).bindv(Array::make(default_canvas)));
		if (Utils::disconnect_safe(custom_canvas, "draw", Callable(this, TEXT(_on_canvas_item_draw))))
			custom_canvas->queue_redraw();
	} else {
		if (Utils::disconnect_safe(default_canvas, "draw", Callable(this, TEXT(_on_canvas_item_draw))))
			default_canvas->queue_redraw();
		Utils::connect_safe(custom_canvas, "draw", Callable(this, TEXT(_on_canvas_item_draw)).bindv(Array::make(_canvas)));
	}
#endif

	custom_canvas = _canvas;
}

Control *DebugDraw::get_custom_canvas() const {
	return custom_canvas;
}

#pragma endregion

#pragma region Draw Functions

Ref<DebugDrawStats> DebugDraw::get_render_stats() {
#ifndef DISABLE_DEBUG_RENDERING
	if (!dgc || !is_ready) return Ref<DebugDrawStats>();
	return dgc->get_render_stats();
#else
	return Ref<DebugDrawStats>();
#endif
}

void DebugDraw::clear_3d_objects() {
#ifndef DISABLE_DEBUG_RENDERING
	if (!dgc || !is_ready) return;
	dgc->clear_3d_objects();
#else
	return;
#endif
}

void DebugDraw::clear_2d_objects() {
#ifndef DISABLE_DEBUG_RENDERING
	if (!grouped_text || !data_graphs || !is_ready) return;
	grouped_text->clear_text();
	data_graphs->clear_graphs();
	mark_canvas_dirty();
#else
	return;
#endif
}

void DebugDraw::clear_all() {
#ifndef DISABLE_DEBUG_RENDERING
	if (!is_ready) return;
	clear_2d_objects();
	clear_3d_objects();
#else
	return;
#endif
}

#pragma region 3D

#ifndef DISABLE_DEBUG_RENDERING
#define CALL_TO_DGC(func, ...)                                          \
	if (!dgc || NEED_LEAVE || config_3d->is_freeze_3d_render()) return; \
	dgc->func(__VA_ARGS__)
#else
#define CALL_TO_DGC(func, ...) return
#endif

#pragma region Spheres

void DebugDraw::draw_sphere(const Vector3 &position, const real_t &radius, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_sphere, position, radius, color, duration);
}

void DebugDraw::draw_sphere_xf(const Transform3D &transform, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_sphere_xf, transform, color, duration);
}

void DebugDraw::draw_sphere_hd(const Vector3 &position, const real_t &radius, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_sphere_hd, position, radius, color, duration);
}

void DebugDraw::draw_sphere_hd_xf(const Transform3D &transform, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_sphere_hd_xf, transform, color, duration);
}

#pragma endregion // Spheres
#pragma region Cylinders

void DebugDraw::draw_cylinder(const Transform3D &transform, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_cylinder, transform, color, duration);
}

#pragma endregion // Cylinders
#pragma region Boxes

void DebugDraw::draw_box(const Vector3 &position, const Vector3 &size, const Color &color, const bool &is_box_centered, const real_t &duration) {
	CALL_TO_DGC(draw_box, position, size, color, is_box_centered, duration);
}

void DebugDraw::draw_box_xf(const Transform3D &transform, const Color &color, const bool &is_box_centered, const real_t &duration) {
	CALL_TO_DGC(draw_box_xf, transform, color, is_box_centered, duration);
}

void DebugDraw::draw_aabb(const AABB &aabb, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_aabb, aabb, color, duration);
}

void DebugDraw::draw_aabb_ab(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_aabb_ab, a, b, color, duration);
}

#pragma endregion // Boxes
#pragma region Lines

void DebugDraw::draw_line_hit(const Vector3 &start, const Vector3 &end, const Vector3 &hit, const bool &is_hit, const real_t &hit_size, const Color &hit_color, const Color &after_hit_color, const real_t &duration) {
	CALL_TO_DGC(draw_line_hit, start, end, hit, is_hit, hit_size, hit_color, after_hit_color, duration);
}

void DebugDraw::draw_line_hit_offset(const Vector3 &start, const Vector3 &end, const bool &is_hit, const real_t &unit_offset_of_hit, const real_t &hit_size, const Color &hit_color, const Color &after_hit_color, const real_t &duration) {
	CALL_TO_DGC(draw_line_hit_offset, start, end, is_hit, unit_offset_of_hit, hit_size, hit_color, after_hit_color, duration);
}

#pragma region Normal

void DebugDraw::draw_line(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_line, a, b, color, duration);
}

void DebugDraw::draw_lines(const PackedVector3Array &lines, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_lines, lines, color, duration);
}

void DebugDraw::draw_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_ray, origin, direction, length, color, duration);
}

void DebugDraw::draw_line_path(const PackedVector3Array &path, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_line_path, path, color, duration);
}

#pragma endregion // Normal
#pragma region Arrows

void DebugDraw::draw_arrow(const Transform3D &transform, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_arrow, transform, color, duration);
}

void DebugDraw::draw_arrow_line(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	CALL_TO_DGC(draw_arrow_line, a, b, color, arrow_size, is_absolute_size, duration);
}

void DebugDraw::draw_arrow_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	CALL_TO_DGC(draw_arrow_ray, origin, direction, length, color, arrow_size, is_absolute_size, duration);
}

void DebugDraw::draw_arrow_path(const PackedVector3Array &path, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	CALL_TO_DGC(draw_arrow_path, path, color, arrow_size, is_absolute_size, duration);
}

#pragma endregion // Arrows
#pragma region Points

void DebugDraw::draw_point_path(const PackedVector3Array &path, const real_t &size, const Color &points_color, const Color &lines_color, const real_t &duration) {
	CALL_TO_DGC(draw_point_path, path, size, points_color, lines_color, duration);
}

#pragma endregion // Points
#pragma endregion // Lines
#pragma region Misc

void DebugDraw::draw_square(const Vector3 &position, const real_t &size, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_square, position, size, color, duration);
}

void DebugDraw::draw_points(const PackedVector3Array &points, const real_t &size, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_points, points, size, color, duration);
}

void DebugDraw::draw_position(const Transform3D &transform, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_position, transform, color, duration);
}

void DebugDraw::draw_gizmo(const Transform3D &transform, const Color &color, const bool &is_centered, const real_t &duration) {
	CALL_TO_DGC(draw_gizmo, transform, color, is_centered, duration);
}

void DebugDraw::draw_grid(const Vector3 &origin, const Vector3 &x_size, const Vector3 &y_size, const Vector2i &subdivision, const Color &color, const bool &is_centered, const real_t &duration) {
	CALL_TO_DGC(draw_grid, origin, x_size, y_size, subdivision, color, is_centered, duration);
}

void DebugDraw::draw_grid_xf(const Transform3D &transform, const Vector2i &_subdivision, const Color &color, const bool &is_centered, const real_t &duration) {
	CALL_TO_DGC(draw_grid_xf, transform, _subdivision, color, is_centered, duration);
}

#pragma region Camera Frustum

void DebugDraw::draw_camera_frustum(const Camera3D *camera, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_camera_frustum, camera, color, duration);
}

void DebugDraw::draw_camera_frustum_planes(const Array &camera_frustum, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_camera_frustum_planes, camera_frustum, color, duration);
}

#pragma endregion // Camera Frustum
#pragma endregion // Misc
#pragma endregion // 3D

#pragma region 2D
#pragma region Text

#ifndef DISABLE_DEBUG_RENDERING
#define CALL_TO_2D(obj, func, ...)  \
	if (!obj || NEED_LEAVE) return; \
	obj->func(__VA_ARGS__)

#define CALL_TO_2D_RET(obj, func, def, ...) \
	if (!obj || NEED_LEAVE) return def;     \
	return obj->func(__VA_ARGS__)
#else
#define CALL_TO_2D(obj, func, ...) return
#define CALL_TO_2D_RET(obj, func, def, ...) return def
#endif

void DebugDraw::begin_text_group(String group_title, int group_priority, Color group_color, bool show_title, int title_size, int text_size) {
	CALL_TO_2D(grouped_text, begin_text_group, group_title, group_priority, group_color, show_title, title_size, text_size);
}

void DebugDraw::end_text_group() {
	CALL_TO_2D(grouped_text, end_text_group);
}

void DebugDraw::set_text(String key, Variant value, int priority, Color color_of_value, real_t duration) {
	CALL_TO_2D(grouped_text, set_text, key, value, priority, color_of_value, duration);
}

#pragma endregion // Text
#pragma region Graphs

Ref<DebugDrawGraph> DebugDraw::create_graph(const StringName &title) {
	CALL_TO_2D_RET(data_graphs, create_graph, Ref<DebugDrawGraph>(), title);
}

Ref<DebugDrawGraph> DebugDraw::create_fps_graph(const StringName &title) {
	CALL_TO_2D_RET(data_graphs, create_fps_graph, Ref<DebugDrawGraph>(), title);
}

void DebugDraw::graph_update_data(const StringName &title, real_t data) {
	CALL_TO_2D(data_graphs, graph_update_data, title, data);
}

void DebugDraw::remove_graph(const StringName &title) {
	CALL_TO_2D(data_graphs, remove_graph, title);
}

void DebugDraw::clear_graphs() {
	CALL_TO_2D(data_graphs, clear_graphs);
}

Ref<DebugDrawGraph> DebugDraw::get_graph(const StringName &title) {
	CALL_TO_2D_RET(data_graphs, get_graph, Ref<DebugDrawGraph>(), title);
}

PackedStringArray DebugDraw::get_graph_names() {
	CALL_TO_2D_RET(data_graphs, get_graph_names, PackedStringArray());
}

#pragma endregion // Graphs
#pragma endregion // 2D

#pragma endregion // Draw Functions

void DebugDrawSceneManager::_notification(int what) {
	switch (what) {
		case NOTIFICATION_PROCESS: {
			DebugDraw::get_singleton()->process(get_process_delta_time());
			break;
		}
		case NOTIFICATION_READY: {
			DebugDraw::get_singleton()->ready();
			set_process(true);
			break;
		}
		case NOTIFICATION_ENTER_TREE: {
			DebugDraw::get_singleton()->enter_tree();
			break;
		}
		case NOTIFICATION_EXIT_TREE: {
			DebugDraw::get_singleton()->exit_tree();
			break;
		}
	}
}