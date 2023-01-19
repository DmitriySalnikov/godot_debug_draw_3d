#include "debug_draw.h"
#include "utils.h"

#include "data_graphs.h"
#include "debug_geometry_container.h"
#include "grouped_text.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#endif

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

#if defined(_MSC_VER)
#pragma warning(default : 4244)
#endif

#include <limits.h>

#define NEED_LEAVE (!debug_enabled || !is_ready)

DebugDraw *DebugDraw::singleton = nullptr;

void DebugDraw::_bind_methods() {
#define REG_CLASS_NAME DebugDraw

	ClassDB::bind_method(D_METHOD(TEXT(get_singleton)), &DebugDraw::get_singleton_gdscript);

	ClassDB::bind_method(D_METHOD(TEXT(_on_canvas_item_draw)), &DebugDraw::_on_canvas_item_draw);
	ClassDB::bind_method(D_METHOD(TEXT(_scene_tree_found)), &DebugDraw::_scene_tree_found);

#pragma region Constants

	BIND_ENUM_CONSTANT(POSITION_LEFT_TOP);
	BIND_ENUM_CONSTANT(POSITION_RIGHT_TOP);
	BIND_ENUM_CONSTANT(POSITION_LEFT_BOTTOM);
	BIND_ENUM_CONSTANT(POSITION_RIGHT_BOTTOM);

#pragma region Parameters

	REG_PROP(empty_color, Variant::COLOR);
	REG_PROP_BOOL(debug_enabled);
	REG_PROP_BOOL(freeze_3d_render);
	REG_PROP_BOOL(visible_instance_bounds);
	REG_PROP_BOOL(use_frustum_culling);
	REG_PROP_BOOL(force_use_camera_from_scene);
	REG_PROP(graphs_base_offset, Variant::VECTOR2I);
	REG_PROP(geometry_render_layers, Variant::INT);
	REG_PROP(text_block_position, Variant::INT);
	REG_PROP(text_block_offset, Variant::VECTOR2I);
	REG_PROP(text_padding, Variant::VECTOR2I);
	REG_PROP(text_default_duration, Variant::FLOAT);
	REG_PROP(text_default_size, Variant::INT);
	REG_PROP(text_foreground_color, Variant::COLOR);
	REG_PROP(text_background_color, Variant::COLOR);
	REG_PROP(text_custom_font, Variant::OBJECT);
	REG_PROP(line_hit_color, Variant::COLOR);
	REG_PROP(line_after_hit_color, Variant::COLOR);

	REG_PROP(custom_viewport, Variant::OBJECT);
	REG_PROP(custom_canvas, Variant::OBJECT);

#pragma endregion

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
	ClassDB::bind_method(D_METHOD(TEXT(set_text), "key", "value", "priority", "color_of_value", "duration"), &DebugDraw::set_text, "", 0, Colors::empty_color, -1.0); // TODO: -1 must be explicitly double. Need fix for Variant converter

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

// TODO: clear geometry on scene switch
//
// TODO: restore animation for lines in example scene

void DebugDraw::_scene_tree_found() {
	base_node = memnew(DebugDrawSceneManager);
	SCENE_ROOT()->add_child(base_node);
	SCENE_ROOT()->move_child(base_node, 0);
}

void DebugDraw::enter_tree() {
	if (IS_EDITOR_HINT()) {
		text_block_position = BlockPosition::POSITION_LEFT_BOTTOM;
		text_block_offset = Vector2(24, 24);
		graphs_base_offset = Vector2(12, 72);
	}

	base_node->set_process_priority(INT32_MAX);

	grouped_text = std::make_unique<GroupedText>();
	grouped_text->init_group(this);
	data_graphs = std::make_unique<DataGraphManager>(this);
	dgc = std::make_unique<DebugGeometryContainer>(this);
}

void DebugDraw::exit_tree() {
	_font.unref();

	if (UtilityFunctions::is_instance_valid(default_canvas) && default_canvas->is_connected("draw", Callable(this, TEXT(_on_canvas_item_draw)))) {
		default_canvas->disconnect("draw", Callable(this, TEXT(_on_canvas_item_draw)));
		default_canvas->queue_redraw();
	}
	if (UtilityFunctions::is_instance_valid(custom_canvas) && custom_canvas->is_connected("draw", Callable(this, TEXT(_on_canvas_item_draw)))) {
		custom_canvas->disconnect("draw", Callable(this, TEXT(_on_canvas_item_draw)));
		custom_canvas->queue_redraw();
	}
}

void DebugDraw::ready() {
	is_ready = true;

	// Usefull nodes and names:
	// CanvasItemEditor - probobly 2D editor viewport
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
		Node *res = Utils::find_node_by_class(SCENE_ROOT(), &editor3d);
		// Node *res = SCENE_ROOT()->find_child("MainScreen", true, false);

		Node *n = res->get_child(0)->get_child(0);
		n->set_meta("UseParentSize", true);
		default_canvas = (Control *)n;
		// if (res) {
		//	res->add_child(_canvasLayer);
		//	_canvasLayer->add_child(default_canvas);
		// } else {
		//	ERR_FAIL_MSG("\"Node3DEditorViewportContainer\" not found in editor tree!");
		// }

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

		base_node->add_child(_canvas_layer);
		_canvas_layer->add_child(default_canvas);
		base_node->move_child(_canvas_layer, 0);
	}
	set_custom_canvas(custom_canvas);
	_set_base_world_node(SCENE_ROOT());
}

void DebugDraw::process(double delta) {
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

	// Print logs if needed
	log_flush_time += delta;
	if (log_flush_time > 0.25f) {
		log_flush_time -= 0.25f;
		Utils::print_logs();
	}
}

void DebugDraw::_on_canvas_item_draw(Control *ci) {
	Vector2 vp_size = ci->has_meta("UseParentSize") ? Object::cast_to<Control>(ci->get_parent())->get_rect().size : ci->get_rect().size;

	grouped_text->draw(ci, _font, vp_size);
	data_graphs->draw(ci, _font, vp_size, ci->get_process_delta_time());
}

void DebugDraw::_set_base_world_node(Node *_world_base) {
	dgc->set_world(_world_base);
}

// TODO: mark dirty if changed fonts or offsets
void DebugDraw::mark_canvas_dirty() {
	_canvas_need_update = true;
}

void DebugDraw::set_custom_editor_viewport(std::vector<Viewport *> _viewports) {
	custom_editor_viewports = _viewports;
}

std::vector<Viewport *> DebugDraw::get_custom_editor_viewport() {
	return custom_editor_viewports;
}

void DebugDraw::set_empty_color(Color _col) {
}

Color DebugDraw::get_empty_color() {
	return Colors::empty_color;
}

#pragma region Exposed Parameters

void DebugDraw::set_debug_enabled(bool _state) {
	debug_enabled = _state;

	if (!_state) {
		clear_all();
	}
}

bool DebugDraw::is_debug_enabled() {
	return debug_enabled;
}

void DebugDraw::set_freeze_3d_render(bool _state) {
	freeze_3d_render = _state;
}

bool DebugDraw::is_freeze_3d_render() {
	return freeze_3d_render;
}

void DebugDraw::set_visible_instance_bounds(bool _state) {
	visible_instance_bounds = _state;
}

bool DebugDraw::is_visible_instance_bounds() {
	return visible_instance_bounds;
}

void DebugDraw::set_use_frustum_culling(bool _state) {
	use_frustum_culling = _state;
}

bool DebugDraw::is_use_frustum_culling() {
	return use_frustum_culling;
}

void DebugDraw::set_force_use_camera_from_scene(bool _state) {
	force_use_camera_from_scene = _state;
}

bool DebugDraw::is_force_use_camera_from_scene() {
	return force_use_camera_from_scene;
}

void DebugDraw::set_graphs_base_offset(Vector2i _offset) {
	graphs_base_offset = _offset;
}

Vector2i DebugDraw::get_graphs_base_offset() {
	return graphs_base_offset;
}

void DebugDraw::set_geometry_render_layers(int32_t _layers) {
	if (geometry_render_layers != _layers) {
		dgc->set_render_layer_mask(_layers);
		geometry_render_layers = _layers;
	}
}

int32_t DebugDraw::get_geometry_render_layers() {
	return geometry_render_layers;
}

void DebugDraw::set_text_block_position(BlockPosition _position) {
	text_block_position = (BlockPosition)_position;
}

DebugDraw::BlockPosition DebugDraw::get_text_block_position() {
	return (BlockPosition)text_block_position;
}

void DebugDraw::set_text_block_offset(Vector2i _offset) {
	text_block_offset = _offset;
}

Vector2i DebugDraw::get_text_block_offset() {
	return text_block_offset;
}

void DebugDraw::set_text_padding(Vector2i _padding) {
	text_padding = _padding;
	text_padding.x = Math::clamp(text_padding.x, 0, INT_MAX);
	text_padding.y = Math::clamp(text_padding.y, 0, INT_MAX);
}

Vector2i DebugDraw::get_text_padding() {
	return text_padding;
}

void DebugDraw::set_text_default_duration(real_t _duration) {
	text_default_duration = _duration;
}

real_t DebugDraw::get_text_default_duration() {
	return text_default_duration;
}

void DebugDraw::set_text_default_size(int _size) {
	text_default_size = Math::clamp(_size, 1, INT_MAX);
}

int DebugDraw::get_text_default_size() {
	return text_default_size;
}

void DebugDraw::set_text_foreground_color(Color _new_color) {
	text_foreground_color = _new_color;
}

Color DebugDraw::get_text_foreground_color() {
	return text_foreground_color;
}

void DebugDraw::set_text_background_color(Color _new_color) {
	text_background_color = _new_color;
}

Color DebugDraw::get_text_background_color() {
	return text_background_color;
}

void DebugDraw::set_text_custom_font(Ref<Font> _custom_font) {
	text_custom_font = _custom_font;
}

Ref<Font> DebugDraw::get_text_custom_font() {
	return text_custom_font;
}

void DebugDraw::set_line_hit_color(Color _new_color) {
	line_hit_color = _new_color;
}

Color DebugDraw::get_line_hit_color() {
	return line_hit_color;
}

void DebugDraw::set_line_after_hit_color(Color _new_color) {
	line_after_hit_color = _new_color;
}

Color DebugDraw::get_line_after_hit_color() {
	return line_after_hit_color;
}

void DebugDraw::set_custom_viewport(Viewport *_viewport) {
	custom_viewport = _viewport;
}

Viewport *DebugDraw::get_custom_viewport() {
	return custom_viewport;
}

void DebugDraw::set_custom_canvas(Control *_canvas) {
	bool connected_internal = UtilityFunctions::is_instance_valid(default_canvas) && default_canvas->is_connected("draw", Callable(this, TEXT(_on_canvas_item_draw)));
	bool connected_custom = UtilityFunctions::is_instance_valid(custom_canvas) && custom_canvas->is_connected("draw", Callable(this, TEXT(_on_canvas_item_draw)));

	if (!_canvas) {
		if (!connected_internal && UtilityFunctions::is_instance_valid(default_canvas)) {
			default_canvas->connect("draw", Callable(this, TEXT(_on_canvas_item_draw)).bindv(Array::make(default_canvas)));
		}
		if (connected_custom) {
			custom_canvas->disconnect("draw", Callable(this, TEXT(_on_canvas_item_draw)));
			custom_canvas->queue_redraw();
		}
	} else {
		if (connected_internal) {
			default_canvas->disconnect("draw", Callable(this, TEXT(_on_canvas_item_draw)));
			default_canvas->queue_redraw();
		}
		if (!connected_custom) {
			_canvas->connect("draw", Callable(this, TEXT(_on_canvas_item_draw)).bindv(Array::make(_canvas)));
		}
	}

	custom_canvas = _canvas;
}

Control *DebugDraw::get_custom_canvas() {
	return custom_canvas;
}

#pragma endregion

#pragma region Draw Functions

Dictionary DebugDraw::get_render_stats() {
	if (!dgc || !is_ready) return Dictionary();
	return dgc->get_render_stats();
}

void DebugDraw::clear_3d_objects() {
	if (!dgc || !is_ready) return;
	dgc->clear_3d_objects();
}

void DebugDraw::clear_2d_objects() {
	if (!grouped_text || !data_graphs || !is_ready) return;
	grouped_text->clear_text();
	data_graphs->clear_graphs();
	mark_canvas_dirty();
}

void DebugDraw::clear_all() {
	if (!is_ready) return;
	clear_2d_objects();
	clear_3d_objects();
}

#pragma region 3D

#define CALL_TO_DGC(func, ...)                          \
	if (!dgc || NEED_LEAVE || freeze_3d_render) return; \
	dgc->func(__VA_ARGS__)

#pragma region Spheres

void DebugDraw::draw_sphere(Vector3 position, real_t radius, Color color, real_t duration) {
	CALL_TO_DGC(draw_sphere, position, radius, color, duration);
}

void DebugDraw::draw_sphere_xf(Transform3D transform, Color color, real_t duration) {
	CALL_TO_DGC(draw_sphere_xf, transform, color, duration);
}

void DebugDraw::draw_sphere_hd(Vector3 position, real_t radius, Color color, real_t duration) {
	CALL_TO_DGC(draw_sphere_hd, position, radius, color, duration);
}

void DebugDraw::draw_sphere_hd_xf(Transform3D transform, Color color, real_t duration) {
	CALL_TO_DGC(draw_sphere_hd_xf, transform, color, duration);
}

#pragma endregion // Spheres
#pragma region Cylinders

void DebugDraw::draw_cylinder(Transform3D transform, Color color, real_t duration) {
	CALL_TO_DGC(draw_cylinder, transform, color, duration);
}

#pragma endregion // Cylinders
#pragma region Boxes

void DebugDraw::draw_box(Vector3 position, Vector3 size, Color color, bool is_box_centered, real_t duration) {
	CALL_TO_DGC(draw_box, position, size, color, is_box_centered, duration);
}

void DebugDraw::draw_box_xf(Transform3D transform, Color color, bool is_box_centered, real_t duration) {
	CALL_TO_DGC(draw_box_xf, transform, color, is_box_centered, duration);
}

void DebugDraw::draw_aabb(AABB aabb, Color color, real_t duration) {
	CALL_TO_DGC(draw_aabb, aabb, color, duration);
}

void DebugDraw::draw_aabb_ab(Vector3 a, Vector3 b, Color color, real_t duration) {
	CALL_TO_DGC(draw_aabb_ab, a, b, color, duration);
}

#pragma endregion // Boxes
#pragma region Lines

void DebugDraw::draw_line_hit(Vector3 start, Vector3 end, Vector3 hit, bool is_hit, real_t hit_size, Color hit_color, Color after_hit_color, real_t duration) {
	CALL_TO_DGC(draw_line_hit, start, end, hit, is_hit, hit_size, hit_color, after_hit_color, duration);
}

void DebugDraw::draw_line_hit_offset(Vector3 start, Vector3 end, bool is_hit, real_t unit_offset_of_hit, real_t hit_size, Color hit_color, Color after_hit_color, real_t duration) {
	CALL_TO_DGC(draw_line_hit_offset, start, end, is_hit, unit_offset_of_hit, hit_size, hit_color, after_hit_color, duration);
}

#pragma region Normal

void DebugDraw::draw_line(Vector3 a, Vector3 b, Color color, real_t duration) {
	CALL_TO_DGC(draw_line, a, b, color, duration);
}

void DebugDraw::draw_lines(PackedVector3Array lines, Color color, real_t duration) {
	CALL_TO_DGC(draw_lines, lines, color, duration);
}

void DebugDraw::draw_ray(Vector3 origin, Vector3 direction, real_t length, Color color, real_t duration) {
	CALL_TO_DGC(draw_ray, origin, direction, length, color, duration);
}

void DebugDraw::draw_line_path(PackedVector3Array path, Color color, real_t duration) {
	CALL_TO_DGC(draw_line_path, path, color, duration);
}

#pragma endregion // Normal
#pragma region Arrows

void DebugDraw::draw_arrow(Transform3D transform, Color color, real_t duration) {
	CALL_TO_DGC(draw_arrow, transform, color, duration);
}

void DebugDraw::draw_arrow_line(Vector3 a, Vector3 b, Color color, real_t arrow_size, bool absolute_size, real_t duration) {
	CALL_TO_DGC(draw_arrow_line, a, b, color, arrow_size, absolute_size, duration);
}

void DebugDraw::draw_arrow_ray(Vector3 origin, Vector3 direction, real_t length, Color color, real_t arrow_size, bool absolute_size, real_t duration) {
	CALL_TO_DGC(draw_arrow_ray, origin, direction, length, color, arrow_size, absolute_size, duration);
}

void DebugDraw::draw_arrow_path(PackedVector3Array path, Color color, real_t arrow_size, bool absolute_size, real_t duration) {
	CALL_TO_DGC(draw_arrow_path, path, color, arrow_size, absolute_size, duration);
}

#pragma endregion // Arrows
#pragma region Points

void DebugDraw::draw_point_path(PackedVector3Array path, real_t size, Color points_color, Color lines_color, real_t duration) {
	CALL_TO_DGC(draw_point_path, path, size, points_color, lines_color, duration);
}

#pragma endregion // Points
#pragma endregion // Lines
#pragma region Misc

void DebugDraw::draw_square(Vector3 position, real_t size, Color color, real_t duration) {
	CALL_TO_DGC(draw_square, position, size, color, duration);
}

void DebugDraw::draw_points(PackedVector3Array points, real_t size, Color color, real_t duration) {
	CALL_TO_DGC(draw_points, points, size, color, duration);
}

void DebugDraw::draw_position(Transform3D transform, Color color, real_t duration) {
	CALL_TO_DGC(draw_position, transform, color, duration);
}

void DebugDraw::draw_gizmo(Transform3D transform, Color color, bool is_centered, real_t duration) {
	CALL_TO_DGC(draw_gizmo, transform, color, is_centered, duration);
}

void DebugDraw::draw_grid(Vector3 origin, Vector3 x_size, Vector3 y_size, Vector2i subdivision, Color color, bool is_centered, real_t duration) {
	CALL_TO_DGC(draw_grid, origin, x_size, y_size, subdivision, color, is_centered, duration);
}

void DebugDraw::draw_grid_xf(Transform3D transform, Vector2i subdivision, Color color, bool is_centered, real_t duration) {
	CALL_TO_DGC(draw_grid_xf, transform, subdivision, color, is_centered, duration);
}

#pragma region Camera Frustum

void DebugDraw::draw_camera_frustum(class Camera3D *camera, Color color, real_t duration) {
	CALL_TO_DGC(draw_camera_frustum, camera, color, duration);
}

void DebugDraw::draw_camera_frustum_planes(Array cameraFrustum, Color color, real_t duration) {
	CALL_TO_DGC(draw_camera_frustum_planes, cameraFrustum, color, duration);
}

#pragma endregion // Camera Frustum
#pragma endregion // Misc
#pragma endregion // 3D

#pragma region 2D
#pragma region Text

#define CALL_TO_2D(obj, func, ...)  \
	if (!obj || NEED_LEAVE) return; \
	obj->func(__VA_ARGS__)

#define CALL_TO_2D_RET(obj, func, def, ...) \
	if (!obj || NEED_LEAVE) return def;     \
	return obj->func(__VA_ARGS__)

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
