#include "debug_draw.h"
#include "utils.h"

#include <godot_cpp/classes/config_file.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/window.hpp>

#include <algorithm>
#include <chrono>
#include <functional>

#define RECALL_TO_SINGLETON(func, ...)                    \
	if (recall_to_singleton && this != get_singleton()) { \
		if (get_singleton()) {                            \
			get_singleton()->func(__VA_ARGS__);           \
			return;                                       \
		} else {                                          \
			return;                                       \
		}                                                 \
	}

#define RECALL_TO_SINGLETON_RET(func, def, ...)           \
	if (recall_to_singleton && this != get_singleton()) { \
		if (get_singleton()) {                            \
			return get_singleton()->func(__VA_ARGS__);    \
		} else {                                          \
			return def;                                   \
		}                                                 \
	}

#define NEED_LEAVE (!debug_enabled || !is_ready)

DebugDraw3D *DebugDraw3D::singleton = nullptr;
int DebugDraw3D::instance_counter = 0;

void DebugDraw3D::_bind_methods() {
#define REG_CLASS_NAME DebugDraw3D

	ClassDB::bind_method(D_METHOD(TEXT(get_singleton)), &DebugDraw3D::get_singleton_gdscript);

	REG_METHOD(_on_canvas_item_draw);

#pragma region Constants

	// TODO actually must be BIND_ENUM_CONSTANT
	BIND_CONSTANT(BlockPosition::LeftTop);
	BIND_CONSTANT(BlockPosition::RightTop);
	BIND_CONSTANT(BlockPosition::LeftBottom);
	BIND_CONSTANT(BlockPosition::RightBottom);

	BIND_CONSTANT(GraphTextFlags::None);
	BIND_CONSTANT(GraphTextFlags::Current);
	BIND_CONSTANT(GraphTextFlags::Avarage);
	BIND_CONSTANT(GraphTextFlags::Max);
	BIND_CONSTANT(GraphTextFlags::Min);
	BIND_CONSTANT(GraphTextFlags::All);

#pragma region Parameters

	REG_PROP_BOOL(recall_to_singleton);
	REG_PROP_BOOL(debug_enabled);
	REG_PROP_BOOL(freeze_3d_render);
	REG_PROP_BOOL(visible_instance_bounds);
	REG_PROP_BOOL(use_frustum_culling);
	REG_PROP_BOOL(force_use_camera_from_scene);
	REG_PROP(graphs_base_offset, Variant::VECTOR2);
	REG_PROP(geometry_render_layers, Variant::INT);
	REG_PROP(text_block_position, Variant::INT);
	REG_PROP(text_block_offset, Variant::VECTOR2);
	REG_PROP(text_padding, Variant::VECTOR2);
	REG_PROP(text_default_duration, Variant::FLOAT);
	REG_PROP(text_foreground_color, Variant::COLOR);
	REG_PROP(text_background_color, Variant::COLOR);
	REG_PROP(text_custom_font, Variant::OBJECT);
	REG_PROP(line_hit_color, Variant::COLOR);
	REG_PROP(line_after_hit_color, Variant::COLOR);

	/* TODO pointers is not available..
	REG_PROP(custom_viewport, Variant::OBJECT);
	REG_PROP(custom_canvas, Variant::OBJECT);
	*/

#pragma endregion

#pragma region Draw Functions
	REG_METHOD(clear_3d_objects);
	REG_METHOD(clear_2d_objects);

	REG_METHOD(clear_all);

	REG_METHOD_ARGS(draw_sphere, "position", "radius", "color", "duration");
	REG_METHOD_ARGS(draw_sphere_xf, "transform", "color", "duration");

	REG_METHOD_ARGS(draw_sphere_hd, "position", "radius", "color", "duration");
	REG_METHOD_ARGS(draw_sphere_hd_xf, "transform", "color", "duration");

	REG_METHOD_ARGS(draw_cylinder, "transform", "color", "duration");

	REG_METHOD_ARGS(draw_box, "position", "size", "color", "is_box_centered", "duration");
	REG_METHOD_ARGS(draw_box_xf, "transform", "color", "is_box_centered", "duration");
	REG_METHOD_ARGS(draw_aabb, "aabb", "color", "duration");
	REG_METHOD_ARGS(draw_aabb_ab, "a", "b", "color", "duration");

	REG_METHOD_ARGS(draw_line_hit, "start", "end", "hit", "is_hit", "hit_size", "hit_color", "after_hit_color", "duration");
	REG_METHOD_ARGS(draw_line_hit_offset, "start", "end", "is_hit", "unit_offset_of_hit", "hit_size", "hit_color", "after_hit_color", "duration");

	REG_METHOD_ARGS(draw_line, "a", "b", "color", "duration");
	REG_METHOD_ARGS(draw_lines, "lines", "color", "duration");
	REG_METHOD_ARGS(draw_ray, "origin", "direction", "length", "color", "duration");
	REG_METHOD_ARGS(draw_line_path, "path", "color", "duration");

	REG_METHOD_ARGS(draw_arrow, "transform", "color", "duration");

	REG_METHOD_ARGS(draw_arrow_line, "a", "b", "color", "arrow_size", "is_absolute_size", "duration");
	REG_METHOD_ARGS(draw_arrow_ray, "origin", "direction", "length", "color", "arrow_size", "is_absolute_size", "duration");
	REG_METHOD_ARGS(draw_arrow_path, "path", "color", "arrow_size", "is_absolute_size", "duration");

	REG_METHOD_ARGS(draw_point_path, "path", "size", "points_color", "lines_color", "duration");

	REG_METHOD_ARGS(draw_square, "position", "size", "color", "duration");
	REG_METHOD_ARGS(draw_points, "points", "size", "color", "duration");

	/* TODO pointers is not available..
	REG_METHOD_ARGS(draw_camera_frustum, "camera", "color", "duration");
	*/
	REG_METHOD_ARGS(draw_camera_frustum_planes, "camera_frustum", "color", "duration");

	REG_METHOD_ARGS(draw_position, "transform", "color", "duration");

	REG_METHOD_ARGS(draw_gizmo, "transform", "color", "is_centered", "duration");

	REG_METHOD_ARGS(draw_grid, "origin", "x_size", "y_size", "subdivision", "color", "is_centered", "duration");
	REG_METHOD_ARGS(draw_grid_xf, "transform", "subdivision", "color", "is_centered", "duration");

	REG_METHOD_ARGS(begin_text_group, "group_title", "group_priority", "group_color", "show_title");
	REG_METHOD(end_text_group);
	REG_METHOD_ARGS(set_text, "key", "value", "priority", "color_of_value", "duration");

	REG_METHOD_ARGS(create_graph, "title");
	REG_METHOD_ARGS(create_fps_graph, "title");
	REG_METHOD_ARGS(graph_update_data, "title", "data");
	REG_METHOD_ARGS(remove_graph, "title");
	REG_METHOD(clear_graphs);
	REG_METHOD_ARGS(get_graph_config, "title");
	REG_METHOD(get_graph_names);

#pragma endregion // Draw Functions

	REG_METHOD(get_render_stats);
}

DebugDraw3D::DebugDraw3D() {
}

void DebugDraw3D::_enter_tree() {
	instance_counter++;

	if (instance_counter > (IS_EDITOR_HINT() ? 2 : 1)) {
		PRINT_WARNING("Too many " TEXT(DebugDraw3D) " instances entered tree: " + String::num_int64(instance_counter));
	}

	if (IS_EDITOR_HINT() && get_parent() == get_tree()->get_root()) {
		PRINT_WARNING("Ignore this instance. It may be created as an Autoload script inside the editor");
		return;
	}

	if (!singleton) {
		singleton = this;
		recall_to_singleton = false;
	} else {
		PRINT_ERROR("Only 1 instance of " TEXT(DebugDraw3D) " is allowed");
	}

	if (IS_EDITOR_HINT()) {
		text_block_position = BlockPosition::LeftBottom;
		text_block_offset = Vector2(24, 24);
		graphs_base_offset = Vector2(12, 72);
	}

	set_process_priority(INT32_MAX);

	grouped_text = std::make_unique<GroupedText>(this);
	data_graphs = std::make_unique<DataGraphManager>(this);
	dgc = std::make_unique<DebugGeometryContainer>(this);
}

void DebugDraw3D::_exit_tree() {
	instance_counter--;

	if (instance_counter < 0) {
		PRINT_WARNING("Too many " TEXT(DebugDraw3D) " instances exited tree: " + String::num_int64(instance_counter));
	}

	if (singleton && singleton == this) {
		singleton = nullptr;
	}

	_font.unref();

	if (default_canvas && default_canvas->is_connected("draw", Callable(this, TEXT(_on_canvas_item_draw)))) {
		default_canvas->disconnect("draw", Callable(this, TEXT(_on_canvas_item_draw)));
		current_draw_canvas = nullptr;
	}
	if (custom_canvas && custom_canvas->is_connected("draw", Callable(this, TEXT(_on_canvas_item_draw)))) {
		custom_canvas->disconnect("draw", Callable(this, TEXT(_on_canvas_item_draw)));
		current_draw_canvas = nullptr;
	}

	// Clear editor canvas
	if (custom_canvas)
		custom_canvas->queue_redraw();
}

void DebugDraw3D::_ready() {
	is_ready = true;

	// Funny hack to get default font
	{
		Control *c = memnew(Control);
		add_child(c);
		_font = c->get_theme_default_font();
		c->queue_free();
	}

	// Setup default text group
	grouped_text->end_text_group();

	// Create canvas item and canvas layer
	_canvasLayer = memnew(CanvasLayer);
	_canvasLayer->set_layer(64);
	default_canvas = memnew(Node2D);

	if (!custom_canvas) { // && godot_is_instance_valid(custom_canvas))
		default_canvas->connect("draw", Callable(this, TEXT(_on_canvas_item_draw))); // TODO use bind()
		current_draw_canvas = default_canvas;
	}

	add_child(_canvasLayer);
	_canvasLayer->add_child(default_canvas);
}

void DebugDraw3D::_process(real_t delta) {
	// Clean texts
	grouped_text->cleanup_text(delta);

	// FPS Graph
	data_graphs->_update_fps(delta);

	// Update overlay
	if (_canvasNeedUpdate) {
		if (!custom_canvas) // && godot_is_instance_valid(custom_canvas))
			default_canvas->queue_redraw();
		else
			custom_canvas->queue_redraw();

		// reset some values
		_canvasNeedUpdate = false;
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

// TODO use version with pointer
// void DebugDraw3D::_on_canvas_item_draw(CanvasItem *ci) {
void DebugDraw3D::_on_canvas_item_draw() {
	RECALL_TO_SINGLETON(_on_canvas_item_draw);

	// TODO remove it
	CanvasItem *ci = current_draw_canvas;

	Vector2 vp_size = ci->has_meta("UseParentSize") ? Object::cast_to<Control>(ci->get_parent())->get_rect().size : ci->get_viewport_rect().size;

	grouped_text->draw(ci, _font, vp_size);
	data_graphs->draw(ci, _font, vp_size);
}

void DebugDraw3D::mark_canvas_needs_update() {
	_canvasNeedUpdate = true;
}

void DebugDraw3D::set_custom_editor_viewport(std::vector<Viewport *> viewports) {
	custom_editor_viewports = viewports;
}

std::vector<Viewport *> DebugDraw3D::get_custom_editor_viewport() {
	return custom_editor_viewports;
}

#pragma region Exposed Parameters

#define RECALL_TO_SINGLETON_SETTER(setter)                \
	if (recall_to_singleton && this != get_singleton()) { \
		if (get_singleton()) {                            \
			get_singleton()->setter;                      \
			return;                                       \
		} else {                                          \
			return;                                       \
		}                                                 \
	}                                                     \
	setter

#define RECALL_TO_SINGLETON_GETTER(getter, def)           \
	if (recall_to_singleton && this != get_singleton()) { \
		if (get_singleton()) {                            \
			return get_singleton()->getter;               \
		} else {                                          \
			return def;                                   \
		}                                                 \
	}                                                     \
	return getter

void DebugDraw3D::set_recall_to_singleton(bool state) {
	RECALL_TO_SINGLETON_SETTER(recall_to_singleton = state);
}

bool DebugDraw3D::is_recall_to_singleton() {
	RECALL_TO_SINGLETON_GETTER(recall_to_singleton, true);
}

void DebugDraw3D::set_debug_enabled(bool state) {
	RECALL_TO_SINGLETON(set_debug_enabled, state);

	debug_enabled = state;

	if (!state) {
		clear_all();
	}
}

bool DebugDraw3D::is_debug_enabled() {
	RECALL_TO_SINGLETON_GETTER(debug_enabled, false);
	return debug_enabled;
}

void DebugDraw3D::set_freeze_3d_render(bool state) {
	RECALL_TO_SINGLETON_SETTER(freeze_3d_render = state);
	freeze_3d_render = state;
}

bool DebugDraw3D::is_freeze_3d_render() {
	RECALL_TO_SINGLETON_GETTER(freeze_3d_render, false);
}

void DebugDraw3D::set_visible_instance_bounds(bool state) {
	RECALL_TO_SINGLETON_SETTER(visible_instance_bounds = state);
	visible_instance_bounds = state;
}

bool DebugDraw3D::is_visible_instance_bounds() {
	RECALL_TO_SINGLETON_GETTER(visible_instance_bounds, false);
}

void DebugDraw3D::set_use_frustum_culling(bool state) {
	RECALL_TO_SINGLETON_SETTER(use_frustum_culling = state);
}

bool DebugDraw3D::is_use_frustum_culling() {
	RECALL_TO_SINGLETON_GETTER(use_frustum_culling, false);
}

void DebugDraw3D::set_force_use_camera_from_scene(bool state) {
	RECALL_TO_SINGLETON_SETTER(force_use_camera_from_scene = state);
}

bool DebugDraw3D::is_force_use_camera_from_scene() {
	RECALL_TO_SINGLETON_GETTER(force_use_camera_from_scene, false);
}

void DebugDraw3D::set_graphs_base_offset(Vector2 offset) {
	RECALL_TO_SINGLETON_SETTER(graphs_base_offset = offset);
}

Vector2 DebugDraw3D::get_graphs_base_offset() {
	RECALL_TO_SINGLETON_GETTER(graphs_base_offset, Vector2());
}

void DebugDraw3D::set_geometry_render_layers(int64_t layers) {
	RECALL_TO_SINGLETON(set_geometry_render_layers, layers);

	if (geometry_render_layers != layers) {
		dgc->set_render_layer_mask(layers);
		geometry_render_layers = layers;
	}
}

int64_t DebugDraw3D::get_geometry_render_layers() {
	RECALL_TO_SINGLETON_GETTER(geometry_render_layers, 0);
}

void DebugDraw3D::set_text_block_position(int position) {
	RECALL_TO_SINGLETON_SETTER(text_block_position = (BlockPosition)position);
}

int DebugDraw3D::get_text_block_position() {
	RECALL_TO_SINGLETON_GETTER(text_block_position, 0);
}

void DebugDraw3D::set_text_block_offset(Vector2 offset) {
	RECALL_TO_SINGLETON_SETTER(text_block_offset = offset);
}

Vector2 DebugDraw3D::get_text_block_offset() {
	RECALL_TO_SINGLETON_GETTER(text_block_offset, Vector2());
}

void DebugDraw3D::set_text_padding(Vector2 padding) {
	RECALL_TO_SINGLETON_SETTER(text_padding = padding);
}

Vector2 DebugDraw3D::get_text_padding() {
	RECALL_TO_SINGLETON_GETTER(text_padding, Vector2());
}

void DebugDraw3D::set_text_default_duration(real_t duration) {
	RECALL_TO_SINGLETON_SETTER(text_default_duration = duration);
}

real_t DebugDraw3D::get_text_default_duration() {
	RECALL_TO_SINGLETON_GETTER(text_default_duration, 0);
}

void DebugDraw3D::set_text_foreground_color(Color new_color) {
	RECALL_TO_SINGLETON_SETTER(text_foreground_color = new_color);
}

Color DebugDraw3D::get_text_foreground_color() {
	RECALL_TO_SINGLETON_GETTER(text_foreground_color, Color());
}

void DebugDraw3D::set_text_background_color(Color new_color) {
	RECALL_TO_SINGLETON_SETTER(text_background_color = new_color);
}

Color DebugDraw3D::get_text_background_color() {
	RECALL_TO_SINGLETON_GETTER(text_background_color, Color());
}

void DebugDraw3D::set_text_custom_font(Ref<Font> custom_font) {
	RECALL_TO_SINGLETON_SETTER(text_custom_font = custom_font);
}

Ref<Font> DebugDraw3D::get_text_custom_font() {
	RECALL_TO_SINGLETON_GETTER(text_custom_font, nullptr);
}

void DebugDraw3D::set_line_hit_color(Color new_color) {
	RECALL_TO_SINGLETON_SETTER(line_hit_color = new_color);
}

Color DebugDraw3D::get_line_hit_color() {
	RECALL_TO_SINGLETON_GETTER(line_hit_color, Color());
}

void DebugDraw3D::set_line_after_hit_color(Color new_color) {
	RECALL_TO_SINGLETON_SETTER(line_after_hit_color = new_color);
}

Color DebugDraw3D::get_line_after_hit_color() {
	RECALL_TO_SINGLETON_GETTER(line_after_hit_color, Color());
}

void DebugDraw3D::set_custom_viewport(Viewport *viewport) {
	RECALL_TO_SINGLETON_SETTER(custom_viewport = viewport);
}

Viewport *DebugDraw3D::get_custom_viewport() {
	RECALL_TO_SINGLETON_GETTER(custom_viewport, nullptr);
}

void DebugDraw3D::set_custom_canvas(CanvasItem *canvas) {
	RECALL_TO_SINGLETON(set_custom_canvas, canvas);

	bool connected_internal = default_canvas && default_canvas->is_connected("draw", Callable(this, TEXT(_on_canvas_item_draw)));
	bool connected_custom = custom_canvas && custom_canvas->is_connected("draw", Callable(this, TEXT(_on_canvas_item_draw)));

	if (!canvas) {
		if (!connected_internal) {
			default_canvas->connect("draw", Callable(this, TEXT(_on_canvas_item_draw))); // TODO use bind()
			current_draw_canvas = default_canvas;
		}
		if (connected_custom) {
			custom_canvas->disconnect("draw", Callable(this, TEXT(_on_canvas_item_draw)));
			current_draw_canvas = nullptr;
		}
	} else {
		if (connected_internal) {
			default_canvas->disconnect("draw", Callable(this, TEXT(_on_canvas_item_draw)));
			current_draw_canvas = nullptr;
		}
		if (!connected_custom) {
			canvas->connect("draw", Callable(this, TEXT(_on_canvas_item_draw))); // TODO use bind()
			current_draw_canvas = canvas;
		}
	}

	custom_canvas = canvas;
}

CanvasItem *DebugDraw3D::get_custom_canvas() {
	RECALL_TO_SINGLETON_GETTER(custom_canvas, nullptr);
}

#pragma endregion

#pragma region Draw Functions

Dictionary DebugDraw3D::get_render_stats() {
	RECALL_TO_SINGLETON_RET(get_render_stats, Dictionary());
	if (!is_ready) return Dictionary();
	return dgc->get_render_stats();
}

void DebugDraw3D::clear_3d_objects() {
	RECALL_TO_SINGLETON(clear_3d_objects);
	if (!is_ready) return;
	dgc->clear_3d_objects();
}

void DebugDraw3D::clear_2d_objects() {
	RECALL_TO_SINGLETON(clear_2d_objects);
	if (!is_ready) return;
	grouped_text->clear_text();
	data_graphs->clear_graphs();
	mark_canvas_needs_update();
}

void DebugDraw3D::clear_all() {
	RECALL_TO_SINGLETON(clear_all);
	if (!is_ready) return;
	clear_2d_objects();
	clear_3d_objects();
}

#pragma region 3D

#define RECALL_TO_SINGLETON_CALL_DGC(func, ...) \
	RECALL_TO_SINGLETON(func, __VA_ARGS__);     \
	if (NEED_LEAVE || freeze_3d_render) return; \
	dgc->func(__VA_ARGS__)

#pragma region Spheres

void DebugDraw3D::draw_sphere(Vector3 position, real_t radius, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_sphere, position, radius, color, duration);
}

void DebugDraw3D::draw_sphere_xf(Transform3D transform, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_sphere_xf, transform, color, duration);
}

void DebugDraw3D::draw_sphere_hd(Vector3 position, real_t radius, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_sphere_hd, position, radius, color, duration);
}

void DebugDraw3D::draw_sphere_hd_xf(Transform3D transform, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_sphere_hd_xf, transform, color, duration);
}

#pragma endregion // Spheres
#pragma region Cylinders

void DebugDraw3D::draw_cylinder(Transform3D transform, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_cylinder, transform, color, duration);
}

#pragma endregion // Cylinders
#pragma region Boxes

void DebugDraw3D::draw_box(Vector3 position, Vector3 size, Color color, bool is_box_centered, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_box, position, size, color, is_box_centered, duration);
}

void DebugDraw3D::draw_box_xf(Transform3D transform, Color color, bool is_box_centered, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_box_xf, transform, color, is_box_centered, duration);
}

void DebugDraw3D::draw_aabb(AABB aabb, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_aabb, aabb, color, duration);
}

void DebugDraw3D::draw_aabb_ab(Vector3 a, Vector3 b, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_aabb_ab, a, b, color, duration);
}

#pragma endregion // Boxes
#pragma region Lines

void DebugDraw3D::draw_line_hit(Vector3 start, Vector3 end, Vector3 hit, bool is_hit, real_t hit_size, Color hit_color, Color after_hit_color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_line_hit, start, end, hit, is_hit, hit_size, hit_color, after_hit_color, duration);
}

void DebugDraw3D::draw_line_hit_offset(Vector3 start, Vector3 end, bool is_hit, real_t unit_offset_of_hit, real_t hit_size, Color hit_color, Color after_hit_color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_line_hit_offset, start, end, is_hit, unit_offset_of_hit, hit_size, hit_color, after_hit_color, duration);
}

#pragma region Normal

void DebugDraw3D::draw_line(Vector3 a, Vector3 b, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_line, a, b, color, duration);
}

void DebugDraw3D::draw_lines(PackedVector3Array lines, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_lines, lines, color, duration);
}

void DebugDraw3D::draw_ray(Vector3 origin, Vector3 direction, real_t length, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_ray, origin, direction, length, color, duration);
}

void DebugDraw3D::draw_line_path(PackedVector3Array path, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_line_path, path, color, duration);
}

#pragma endregion // Normal
#pragma region Arrows

void DebugDraw3D::draw_arrow(Transform3D transform, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_arrow, transform, color, duration);
}

void DebugDraw3D::draw_arrow_line(Vector3 a, Vector3 b, Color color, real_t arrow_size, bool absolute_size, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_arrow_line, a, b, color, arrow_size, absolute_size, duration);
}

void DebugDraw3D::draw_arrow_ray(Vector3 origin, Vector3 direction, real_t length, Color color, real_t arrow_size, bool absolute_size, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_arrow_ray, origin, direction, length, color, arrow_size, absolute_size, duration);
}

void DebugDraw3D::draw_arrow_path(PackedVector3Array path, Color color, real_t arrow_size, bool absolute_size, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_arrow_path, path, color, arrow_size, absolute_size, duration);
}

#pragma endregion // Arrows
#pragma region Points

void DebugDraw3D::draw_point_path(PackedVector3Array path, real_t size, Color points_color, Color lines_color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_point_path, path, size, points_color, lines_color, duration);
}

#pragma endregion // Points
#pragma endregion // Lines
#pragma region Misc

void DebugDraw3D::draw_square(Vector3 position, real_t size, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_square, position, size, color, duration);
}

void DebugDraw3D::draw_points(PackedVector3Array points, real_t size, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_points, points, size, color, duration);
}

void DebugDraw3D::draw_position(Transform3D transform, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_position, transform, color, duration);
}

void DebugDraw3D::draw_gizmo(Transform3D transform, Color color, bool is_centered, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_gizmo, transform, color, is_centered, duration);
}

void DebugDraw3D::draw_grid(Vector3 origin, Vector3 x_size, Vector3 y_size, Vector2i subdivision, Color color, bool is_centered, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_grid, origin, x_size, y_size, subdivision, color, is_centered, duration);
}

void DebugDraw3D::draw_grid_xf(Transform3D transform, Vector2i subdivision, Color color, bool is_centered, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_grid_xf, transform, subdivision, color, is_centered, duration);
}

#pragma region Camera Frustum

void DebugDraw3D::draw_camera_frustum(class Camera3D *camera, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_camera_frustum, camera, color, duration);
}

void DebugDraw3D::draw_camera_frustum_planes(Array cameraFrustum, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_camera_frustum_planes, cameraFrustum, color, duration);
}

#pragma endregion // Camera Frustum
#pragma endregion // Misc
#pragma endregion // 3D

#pragma region 2D
#pragma region Text

#define RECALL_TO_SINGLETON_CALL_2D(obj, func, ...) \
	RECALL_TO_SINGLETON(func, __VA_ARGS__);         \
	if (NEED_LEAVE) return;                         \
	obj->func(__VA_ARGS__)

#define RECALL_TO_SINGLETON_CALL_2D_RET(obj, func, def, ...) \
	RECALL_TO_SINGLETON_RET(func, def, __VA_ARGS__);         \
	if (NEED_LEAVE) return def;                              \
	return obj->func(__VA_ARGS__)

void DebugDraw3D::begin_text_group(String group_title, int group_priority, Color group_color, bool show_title) {
	RECALL_TO_SINGLETON_CALL_2D(grouped_text, begin_text_group, group_title, group_priority, group_color, show_title);
}

void DebugDraw3D::end_text_group() {
	RECALL_TO_SINGLETON_CALL_2D(grouped_text, end_text_group);
}

void DebugDraw3D::set_text(String key, Variant value, int priority, Color color_of_value, real_t duration) {
	RECALL_TO_SINGLETON_CALL_2D(grouped_text, set_text, key, value, priority, color_of_value, duration);
}

#pragma endregion // Text
#pragma region Graphs

Ref<GraphParameters> DebugDraw3D::create_graph(String title) {
	RECALL_TO_SINGLETON_CALL_2D_RET(data_graphs, create_graph, Ref<GraphParameters>(), title);
}

Ref<GraphParameters> DebugDraw3D::create_fps_graph(String title) {
	RECALL_TO_SINGLETON_CALL_2D_RET(data_graphs, create_fps_graph, Ref<GraphParameters>(), title);
}

void DebugDraw3D::graph_update_data(String title, real_t data) {
	RECALL_TO_SINGLETON_CALL_2D(data_graphs, graph_update_data, title, data);
}

void DebugDraw3D::remove_graph(String title) {
	RECALL_TO_SINGLETON_CALL_2D(data_graphs, remove_graph, title);
}

void DebugDraw3D::clear_graphs() {
	RECALL_TO_SINGLETON_CALL_2D(data_graphs, clear_graphs);
}

Ref<GraphParameters> DebugDraw3D::get_graph_config(String title) {
	RECALL_TO_SINGLETON_CALL_2D_RET(data_graphs, get_graph_config, Ref<GraphParameters>(), title);
}

PackedStringArray DebugDraw3D::get_graph_names() {
	RECALL_TO_SINGLETON_CALL_2D_RET(data_graphs, get_graph_names, PackedStringArray());
}

#pragma endregion // Graphs
#pragma endregion // 2D

#pragma endregion // Draw Functions
