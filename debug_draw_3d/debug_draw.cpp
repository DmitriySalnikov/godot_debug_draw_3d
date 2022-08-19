#include "debug_draw.h"
#include "utils.h"

#include <ConfigFile.hpp>
#include <Directory.hpp>
#include <Engine.hpp>
#include <File.hpp>
#include <GlobalConstants.hpp>
#include <Label.hpp>
#include <Node2D.hpp>
#include <ProjectSettings.hpp>
#include <SceneTree.hpp>
#include <SpatialMaterial.hpp>
#include <Viewport.hpp>

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

#define RECALL_TO_SINGLETON_NA(func)                      \
	if (recall_to_singleton && this != get_singleton()) { \
		if (get_singleton()) {                            \
			get_singleton()->func();                      \
			return;                                       \
		} else {                                          \
			return;                                       \
		}                                                 \
	}

#define RECALL_TO_SINGLETON_NA_RET(func, def)             \
	if (recall_to_singleton && this != get_singleton()) { \
		if (get_singleton()) {                            \
			return get_singleton()->func();               \
		} else {                                          \
			return def;                                   \
		}                                                 \
	}

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

#define NEED_LEAVE (!debug_enabled || !is_ready)

DebugDraw3D *DebugDraw3D::singleton = nullptr;
int DebugDraw3D::instance_counter = 0;

void DebugDraw3D::_register_methods() {
	register_method("get_singleton", &DebugDraw3D::get_singleton_gdscript);

#define REG_METHOD(name) register_method(#name, &DebugDraw3D::name)

	REG_METHOD(_enter_tree);
	REG_METHOD(_exit_tree);
	REG_METHOD(_ready);
	REG_METHOD(_process);

	REG_METHOD(on_canva_item_draw);

#pragma region Constants
#define CONST_REG(_enum, _const) \
	register_property(TEXT(_enum##_##_const), &DebugDraw3D::fake_set_const, &DebugDraw3D::get_##_enum##_##_const, (int64_t)_enum::_const)

	CONST_REG(BlockPosition, LeftTop);
	CONST_REG(BlockPosition, RightTop);
	CONST_REG(BlockPosition, LeftBottom);
	CONST_REG(BlockPosition, RightBottom);

	CONST_REG(GraphTextFlags, None);
	CONST_REG(GraphTextFlags, Current);
	CONST_REG(GraphTextFlags, Avarage);
	CONST_REG(GraphTextFlags, Max);
	CONST_REG(GraphTextFlags, Min);
	CONST_REG(GraphTextFlags, All);

#undef CONST_REG
#pragma endregion

#pragma region Parameters

#define REG_PROP(name, def) register_property(#name, &DebugDraw3D::set_##name, &DebugDraw3D::get_##name, def);
#define REG_PROP_BOOL(name, def) register_property(#name, &DebugDraw3D::set_##name, &DebugDraw3D::is_##name, def);

	REG_PROP_BOOL(recall_to_singleton, true);
	REG_PROP_BOOL(debug_enabled, true);
	REG_PROP_BOOL(freeze_3d_render, false);
	REG_PROP_BOOL(draw_instance_bounds, false);
	REG_PROP_BOOL(use_frustum_culling, true);
	REG_PROP_BOOL(force_use_camera_from_scene, false);
	REG_PROP(graphs_base_offset, Vector2(8, 8));
	REG_PROP(geometry_render_layers, (int64_t)1);
	REG_PROP(text_block_position, (int)BlockPosition::LeftTop);
	REG_PROP(text_block_offset, Vector2(8, 8));
	REG_PROP(text_padding, Vector2(2, 1));
	REG_PROP(text_default_duration, (real_t)0.5f);
	REG_PROP(text_foreground_color, Color(1, 1, 1));
	REG_PROP(text_background_color, Color(0.3f, 0.3f, 0.3f, 0.8f));
	REG_PROP(text_custom_font, Ref<Font>());
	REG_PROP(line_hit_color, Color(1, 0, 0, 1));
	REG_PROP(line_after_hit_color, Color(0, 1, 0, 1));
	REG_PROP(custom_viewport, (Viewport *)nullptr);
	REG_PROP(custom_canvas, (CanvasItem *)nullptr);

#undef REG_PROP
#undef REG_PROP_BOOL

#pragma endregion

#pragma region Draw Functions
	REG_METHOD(clear_3d_objects);
	REG_METHOD(clear_2d_objects);

	REG_METHOD(clear_all);

	REG_METHOD(draw_sphere);
	REG_METHOD(draw_sphere_xf);

	REG_METHOD(draw_sphere_hd);
	REG_METHOD(draw_sphere_hd_xf);

	REG_METHOD(draw_cylinder);

	REG_METHOD(draw_box);
	REG_METHOD(draw_aabb);
	REG_METHOD(draw_aabb_ab);

	REG_METHOD(draw_line_hit);
	REG_METHOD(draw_line_hit_offset);

	REG_METHOD(draw_line);
	REG_METHOD(draw_lines);
	REG_METHOD(draw_ray);
	REG_METHOD(draw_line_path);

	REG_METHOD(draw_arrow_line);
	REG_METHOD(draw_arrow_ray);
	REG_METHOD(draw_arrow_path);

	REG_METHOD(draw_billboard_square);

	REG_METHOD(draw_camera_frustum);
	REG_METHOD(draw_camera_frustum_planes);

	REG_METHOD(draw_position);

	REG_METHOD(draw_gizmo);

	REG_METHOD(draw_grid);
	REG_METHOD(draw_grid_xf);

	REG_METHOD(begin_text_group);
	REG_METHOD(end_text_group);
	REG_METHOD(set_text);

	REG_METHOD(create_graph);
	REG_METHOD(create_fps_graph);
	REG_METHOD(graph_update_data);
	REG_METHOD(remove_graph);
	REG_METHOD(clear_graphs);
	REG_METHOD(get_graph_config);
	REG_METHOD(get_graph_names);

#pragma endregion // Draw Functions

	REG_METHOD(get_rendered_primitives_count);

#undef REG_METHOD
}

void DebugDraw3D::_init() {
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

	if (default_canvas && default_canvas->is_connected("draw", this, TEXT(on_canva_item_draw)))
		default_canvas->disconnect("draw", this, TEXT(on_canva_item_draw));
	if (custom_canvas && custom_canvas->is_connected("draw", this, TEXT(on_canva_item_draw)))
		custom_canvas->disconnect("draw", this, TEXT(on_canva_item_draw));

	// Clear editor canvas
	if (custom_canvas)
		custom_canvas->update();
}

void DebugDraw3D::_ready() {
	is_ready = true;

	// Funny hack to get default font
	{
		Control *c = Control::_new();
		add_child(c);
		_font = c->get_font("font");
		c->queue_free();
	}

	// Setup default text group
	grouped_text->end_text_group();

	// Create canvas item and canvas layer
	_canvasLayer = CanvasLayer::_new();
	_canvasLayer->set_layer(64);
	default_canvas = Node2D::_new();

	if (!custom_canvas) // && godot_is_instance_valid(custom_canvas))
		default_canvas->connect("draw", this, TEXT(on_canva_item_draw), Array::make(default_canvas));

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
			default_canvas->update();
		else
			custom_canvas->update();

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

void DebugDraw3D::on_canva_item_draw(CanvasItem *ci) {
	RECALL_TO_SINGLETON(on_canva_item_draw, ci);

	Vector2 vp_size = ci->has_meta("UseParentSize") ? cast_to<Control>(ci->get_parent())->get_rect().size : ci->get_viewport_rect().size;

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

void DebugDraw3D::set_draw_instance_bounds(bool state) {
	RECALL_TO_SINGLETON_SETTER(draw_instance_bounds = state);
	draw_instance_bounds = state;
}

bool DebugDraw3D::is_draw_instance_bounds() {
	RECALL_TO_SINGLETON_GETTER(draw_instance_bounds, false);
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

	bool connected_internal = default_canvas && default_canvas->is_connected("draw", this, TEXT(on_canva_item_draw));
	bool connected_custom = custom_canvas && custom_canvas->is_connected("draw", this, TEXT(on_canva_item_draw));

	if (!canvas) {
		if (!connected_internal)
			default_canvas->connect("draw", this, TEXT(on_canva_item_draw), Array::make(default_canvas));
		if (connected_custom)
			custom_canvas->disconnect("draw", this, TEXT(on_canva_item_draw));
	} else {
		if (connected_internal)
			default_canvas->disconnect("draw", this, TEXT(on_canva_item_draw));
		if (!connected_custom)
			canvas->connect("draw", this, TEXT(on_canva_item_draw), Array::make(canvas));
	}

	custom_canvas = canvas;
}

CanvasItem *DebugDraw3D::get_custom_canvas() {
	RECALL_TO_SINGLETON_GETTER(custom_canvas, nullptr);
}

#pragma endregion

#pragma region Draw Functions

Dictionary DebugDraw3D::get_rendered_primitives_count() {
	RECALL_TO_SINGLETON_NA_RET(get_rendered_primitives_count, Dictionary());
	if (!is_ready) return Dictionary();
	return dgc->get_rendered_primitives_count();
}

void DebugDraw3D::clear_3d_objects() {
	RECALL_TO_SINGLETON_NA(clear_3d_objects);
	if (!is_ready) return;
	dgc->clear_3d_objects();
}

void DebugDraw3D::clear_2d_objects() {
	RECALL_TO_SINGLETON_NA(clear_2d_objects);
	if (!is_ready) return;
	grouped_text->clear_text();
	data_graphs->clear_graphs();
	mark_canvas_needs_update();
}

void DebugDraw3D::clear_all() {
	RECALL_TO_SINGLETON_NA(clear_all);
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

void DebugDraw3D::draw_sphere_xf(Transform transform, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_sphere_xf, transform, color, duration);
}

void DebugDraw3D::draw_sphere_hd(Vector3 position, real_t radius, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_sphere_hd, position, radius, color, duration);
}

void DebugDraw3D::draw_sphere_hd_xf(Transform transform, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_sphere_hd_xf, transform, color, duration);
}

#pragma endregion // Spheres
#pragma region Cylinders

void DebugDraw3D::draw_cylinder(Transform transform, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_cylinder, transform, color, duration);
}

#pragma endregion // Cylinders
#pragma region Boxes

void DebugDraw3D::draw_box(Transform transform, Color color, bool is_box_centered, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_box, transform, color, is_box_centered, duration);
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

void DebugDraw3D::draw_lines(PoolVector3Array lines, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_lines, lines, color, duration);
}

void DebugDraw3D::draw_ray(Vector3 origin, Vector3 direction, real_t length, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_ray, origin, direction, length, color, duration);
}

void DebugDraw3D::draw_line_path(PoolVector3Array path, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_line_path, path, color, duration);
}

#pragma endregion // Normal
#pragma region Arrows

void DebugDraw3D::draw_arrow_line(Vector3 a, Vector3 b, Color color, real_t arrow_size, bool absolute_size, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_arrow_line, a, b, color, arrow_size, absolute_size, duration);
}

void DebugDraw3D::draw_arrow_ray(Vector3 origin, Vector3 direction, real_t length, Color color, real_t arrow_size, bool absolute_size, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_arrow_ray, origin, direction, length, color, arrow_size, absolute_size, duration);
}

void DebugDraw3D::draw_arrow_path(PoolVector3Array path, Color color, real_t arrow_size, bool absolute_size, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_arrow_path, path, color, arrow_size, absolute_size, duration);
}

#pragma endregion // Arrows
#pragma endregion // Lines
#pragma region Misc

void DebugDraw3D::draw_billboard_square(Vector3 position, real_t size, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_billboard_square, position, size, color, duration);
}

#pragma region Camera Frustum

void DebugDraw3D::draw_camera_frustum(class Camera *camera, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_camera_frustum, camera, color, duration);
}

void DebugDraw3D::draw_camera_frustum_planes(Array cameraFrustum, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_camera_frustum_planes, cameraFrustum, color, duration);
}

#pragma endregion // Camera Frustum

void DebugDraw3D::draw_position(Transform transform, Color color, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_position, transform, color, duration);
}

void DebugDraw3D::draw_gizmo(Transform transform, bool is_centered, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_gizmo, transform, is_centered, duration);
}

void DebugDraw3D::draw_grid(Vector3 origin, Vector3 x_size, Vector3 y_size, Vector2 subdivision, Color color, bool is_centered, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_grid, origin, x_size, y_size, subdivision, color, is_centered, duration);
}

void DebugDraw3D::draw_grid_xf(Transform transform, Vector2 subdivision, Color color, bool is_centered, real_t duration) {
	RECALL_TO_SINGLETON_CALL_DGC(draw_grid_xf, transform, subdivision, color, is_centered, duration);
}

#pragma endregion // Misc
#pragma endregion // 3D

#undef RECALL_TO_SINGLETON_CALL_DGC

#pragma region 2D
#pragma region Text

void DebugDraw3D::begin_text_group(String group_title, int group_priority, Color group_color, bool show_title) {
	RECALL_TO_SINGLETON(begin_text_group, group_title, group_priority, group_color, show_title);
	if (NEED_LEAVE) return;
	grouped_text->begin_text_group(group_title, group_priority, group_color, show_title);
}

void DebugDraw3D::end_text_group() {
	RECALL_TO_SINGLETON_NA(end_text_group);
	if (NEED_LEAVE) return;
	grouped_text->end_text_group();
}

void DebugDraw3D::set_text(String key, Variant value, int priority, Color color_of_value, real_t duration) {
	RECALL_TO_SINGLETON(set_text, key, value, priority, color_of_value, duration);
	if (NEED_LEAVE) return;
	grouped_text->set_text(key, value, priority, color_of_value, duration);
}

#pragma endregion // Text
#pragma region Graphs

Ref<GraphParameters> DebugDraw3D::create_graph(String title) {
	RECALL_TO_SINGLETON_RET(create_graph, Ref<GraphParameters>(), title);
	if (NEED_LEAVE) return Ref<GraphParameters>();
	return data_graphs->create_graph(title);
}

Ref<GraphParameters> DebugDraw3D::create_fps_graph(String title) {
	RECALL_TO_SINGLETON_RET(create_fps_graph, Ref<GraphParameters>(), title);
	if (NEED_LEAVE) return Ref<GraphParameters>();
	return data_graphs->create_fps_graph(title);
}

void DebugDraw3D::graph_update_data(String title, real_t data) {
	RECALL_TO_SINGLETON(graph_update_data, title, data);
	if (NEED_LEAVE) return;
	data_graphs->update(title, data);
}

void DebugDraw3D::remove_graph(String title) {
	RECALL_TO_SINGLETON(remove_graph, title);
	if (NEED_LEAVE) return;
	data_graphs->remove_graph(title);
}

void DebugDraw3D::clear_graphs() {
	RECALL_TO_SINGLETON_NA(clear_graphs);
	if (NEED_LEAVE) return;
	data_graphs->clear_graphs();
}

Ref<GraphParameters> DebugDraw3D::get_graph_config(String title) {
	RECALL_TO_SINGLETON_RET(get_graph_config, Ref<GraphParameters>(), title);
	if (NEED_LEAVE) return Ref<GraphParameters>();
	return data_graphs->get_graph_config(title);
}

PoolStringArray DebugDraw3D::get_graph_names() {
	RECALL_TO_SINGLETON_NA_RET(get_graph_names, PoolStringArray());
	if (NEED_LEAVE) return PoolStringArray();
	return data_graphs->get_graph_names();
}

#pragma endregion // Graphs
#pragma endregion // 2D

#pragma endregion // Draw Functions
