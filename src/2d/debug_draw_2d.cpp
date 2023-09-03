#include "debug_draw_2d.h"

#include "config_2d.h"
#include "debug_draw_manager.h"
#include "geometry_container_2d.h"
#include "graphs.h"
#include "grouped_text.h"
#include "stats_2d.h"
#include "utils/utils.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/os.hpp>
GODOT_WARNING_RESTORE()

#include <limits.h>

#define NEED_LEAVE (!_is_enabled_override())
std::atomic<uint64_t> DDTransform2DGuard::create_counter = 0;

DDTransform2DGuard::DDTransform2DGuard() {
	ERR_FAIL_MSG(NAMEOF(DDTransform2DGuard) " is not intended to be instantiated directly");
}

DDTransform2DGuard::DDTransform2DGuard(const Transform2D &xform) {
	guard_id = create_counter++;
	thread_id = OS::get_singleton()->get_thread_caller_id();

	DebugDraw2D::get_singleton()->register_transform_guard(guard_id, thread_id, xform);
}

DDTransform2DGuard::~DDTransform2DGuard() {
	DebugDraw2D::get_singleton()->unregister_transform_guard(guard_id, thread_id);
}

DebugDraw2D *DebugDraw2D::singleton = nullptr;

void DebugDraw2D::_bind_methods() {
#define REG_CLASS_NAME DebugDraw2D

	ClassDB::bind_method(D_METHOD(NAMEOF(_on_canvas_marked_dirty)), &DebugDraw2D::_on_canvas_marked_dirty);
	ClassDB::bind_method(D_METHOD(NAMEOF(_on_canvas_item_draw)), &DebugDraw2D::_on_canvas_item_draw);

#pragma region Parameters

	REG_PROP(empty_color, Variant::COLOR);
	REG_PROP_BOOL(debug_enabled);

	REG_PROP(default_transform_2d, Variant::TRANSFORM2D);
	REG_PROP(config, Variant::OBJECT);

	REG_PROP(custom_canvas, Variant::OBJECT);

#pragma endregion
#undef REG_CLASS_NAME

#pragma region Draw Functions
	ClassDB::bind_method(D_METHOD(NAMEOF(clear_all)), &DebugDraw2D::clear_all);

	const int DEFAULT_FONT_SIZE = 16;
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_line), "from", "to", "color", "width", "antialiased"), &DebugDraw2D::draw_line, -1.0, false);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_dashed_line), "from", "to", "color", "width", "dash", "aligned"), &DebugDraw2D::draw_dashed_line, -1.0, 2.0, true);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_polyline), "points", "color", "width", "antialiased"), &DebugDraw2D::draw_polyline, -1.0, false);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_polyline_colors), "points", "colors", "width", "antialiased"), &DebugDraw2D::draw_polyline_colors, -1.0, false);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_arc), "center", "radius", "start_angle", "end_angle", "point_count", "color", "width", "antialiased"), &DebugDraw2D::draw_arc, -1.0, false);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_multiline), "points", "color", "width"), &DebugDraw2D::draw_multiline, -1.0);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_multiline_colors), "points", "colors", "width"), &DebugDraw2D::draw_multiline_colors, -1.0);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_rect), "rect", "color", "filled", "width"), &DebugDraw2D::draw_rect, true, -1.0);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_circle), "position", "radius", "color"), &DebugDraw2D::draw_circle);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_texture), "texture", "position", "modulate"), &DebugDraw2D::draw_texture, Color(1, 1, 1, 1));
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_texture_rect), "texture", "rect", "tile", "modulate", "transpose"), &DebugDraw2D::draw_texture_rect, Color(1, 1, 1, 1), false);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_texture_rect_region), "texture", "rect", "src_rect", "modulate", "transpose", "clip_uv"), &DebugDraw2D::draw_texture_rect_region, Color(1, 1, 1, 1), false, true);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_style_box), "style_box", "rect"), &DebugDraw2D::draw_style_box);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_primitive), "points", "colors", "uvs", "texture"), &DebugDraw2D::draw_primitive, Ref<Texture2D>());
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_polygon), "points", "colors", "uvs", "texture"), &DebugDraw2D::draw_polygon, PackedVector2Array(), Ref<Texture2D>());
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_colored_polygon), "points", "color", "uvs", "texture"), &DebugDraw2D::draw_colored_polygon, PackedVector2Array(), Ref<Texture2D>());
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_string), "font", "pos", "text", "alignment", "width", "font_size", "modulate", "justification_flags", "direction", "orientation"), &DebugDraw2D::draw_string, HORIZONTAL_ALIGNMENT_LEFT, -1, DEFAULT_FONT_SIZE, Color(1.0, 1.0, 1.0), TextServer::JUSTIFICATION_KASHIDA | TextServer::JUSTIFICATION_WORD_BOUND, TextServer::DIRECTION_AUTO, TextServer::ORIENTATION_HORIZONTAL);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_multiline_string), "font", "pos", "text", "alignment", "width", "font_size", "max_lines", "modulate", "brk_flags", "justification_flags", "direction", "orientation"), &DebugDraw2D::draw_multiline_string, HORIZONTAL_ALIGNMENT_LEFT, -1, DEFAULT_FONT_SIZE, -1, Color(1.0, 1.0, 1.0), TextServer::BREAK_MANDATORY | TextServer::BREAK_WORD_BOUND, TextServer::JUSTIFICATION_KASHIDA | TextServer::JUSTIFICATION_WORD_BOUND, TextServer::DIRECTION_AUTO, TextServer::ORIENTATION_HORIZONTAL);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_string_outline), "font", "pos", "text", "alignment", "width", "font_size", "size", "modulate", "justification_flags", "direction", "orientation"), &DebugDraw2D::draw_string_outline, HORIZONTAL_ALIGNMENT_LEFT, -1, DEFAULT_FONT_SIZE, 1, Color(1.0, 1.0, 1.0), TextServer::JUSTIFICATION_KASHIDA | TextServer::JUSTIFICATION_WORD_BOUND, TextServer::DIRECTION_AUTO, TextServer::ORIENTATION_HORIZONTAL);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_multiline_string_outline), "font", "pos", "text", "alignment", "width", "font_size", "max_lines", "size", "modulate", "brk_flags", "justification_flags", "direction", "orientation"), &DebugDraw2D::draw_multiline_string_outline, HORIZONTAL_ALIGNMENT_LEFT, -1, DEFAULT_FONT_SIZE, -1, 1, Color(1.0, 1.0, 1.0), TextServer::BREAK_MANDATORY | TextServer::BREAK_WORD_BOUND, TextServer::JUSTIFICATION_KASHIDA | TextServer::JUSTIFICATION_WORD_BOUND, TextServer::DIRECTION_AUTO, TextServer::ORIENTATION_HORIZONTAL);
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_mesh), "mesh", "texture", "transform", "modulate"), &DebugDraw2D::draw_mesh, Transform2D(), Color(1, 1, 1, 1));
	// ClassDB::bind_method(D_METHOD(NAMEOF(draw_multimesh), "multimesh", "texture"), &DebugDraw2D::draw_multimesh);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_set_transform_2d), "position", "rotation", "scale"), &DebugDraw2D::draw_set_transform_2d, 0.0, Vector2(1.0, 1.0));
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_set_transform_matrix_2d), "xform"), &DebugDraw2D::draw_set_transform_matrix_2d);

	ClassDB::bind_method(D_METHOD(NAMEOF(begin_text_group), "group_title", "group_priority", "group_color", "show_title", "title_size", "text_size"), &DebugDraw2D::begin_text_group, 0, Colors::empty_color, true, -1, -1);
	ClassDB::bind_method(D_METHOD(NAMEOF(end_text_group)), &DebugDraw2D::end_text_group);
	ClassDB::bind_method(D_METHOD(NAMEOF(set_text), "key", "value", "priority", "color_of_value", "duration"), &DebugDraw2D::set_text, Variant(), 0, Colors::empty_color, -1.0);

	ClassDB::bind_method(D_METHOD(NAMEOF(create_graph), "title"), &DebugDraw2D::create_graph);
	ClassDB::bind_method(D_METHOD(NAMEOF(create_fps_graph), "title"), &DebugDraw2D::create_fps_graph);
	ClassDB::bind_method(D_METHOD(NAMEOF(graph_update_data), "title", "data"), &DebugDraw2D::graph_update_data);
	ClassDB::bind_method(D_METHOD(NAMEOF(remove_graph), "title"), &DebugDraw2D::remove_graph);
	ClassDB::bind_method(D_METHOD(NAMEOF(clear_graphs)), &DebugDraw2D::clear_graphs);
	ClassDB::bind_method(D_METHOD(NAMEOF(get_graph), "title"), &DebugDraw2D::get_graph);
	ClassDB::bind_method(D_METHOD(NAMEOF(get_graph_names)), &DebugDraw2D::get_graph_names);

#pragma endregion // Draw Functions

	ClassDB::bind_method(D_METHOD(NAMEOF(get_render_stats)), &DebugDraw2D::get_render_stats);
}

DebugDraw2D::DebugDraw2D() {
	ASSIGN_SINGLETON(DebugDraw2D);
}

void DebugDraw2D::init(DebugDrawManager *root) {
	root_node = root;
	set_config(nullptr);

#ifndef DISABLE_DEBUG_RENDERING
	grouped_text = std::make_unique<GroupedText>();
	grouped_text->init_group(this);
	data_graphs = std::make_unique<DataGraphManager>(this);
#endif
}

DebugDraw2D::~DebugDraw2D() {
	UNASSIGN_SINGLETON(DebugDraw2D);

#ifndef DISABLE_DEBUG_RENDERING
	data_graphs.reset();
	grouped_text.reset();

	if (Utils::disconnect_safe(default_canvas, "draw", Callable(this, NAMEOF(_on_canvas_item_draw))))
		default_canvas->queue_redraw();
	if (Utils::disconnect_safe(custom_canvas, "draw", Callable(this, NAMEOF(_on_canvas_item_draw))))
		custom_canvas->queue_redraw();

	if (!IS_EDITOR_HINT()) {
		if (UtilityFunctions::is_instance_valid(default_canvas))
			default_canvas->queue_free();

		default_canvas = nullptr;
	}
#endif

	root_node = nullptr;
	clear_transform_guards();
}

void DebugDraw2D::process(double delta) {
#ifndef DISABLE_DEBUG_RENDERING
	// Clean texts
	grouped_text->cleanup_text(delta);

	// FPS Graph
	data_graphs->auto_update_graphs(delta);

	// Update overlay
	_finish_frame_and_update();
#endif
}

#ifndef DISABLE_DEBUG_RENDERING
void DebugDraw2D::_finish_frame_and_update() {
	if (_canvas_need_update) {
		if (!UtilityFunctions::is_instance_valid(custom_canvas) && UtilityFunctions::is_instance_valid(default_canvas)) {
			default_canvas->queue_redraw();
		} else if (UtilityFunctions::is_instance_valid(custom_canvas)) {
			custom_canvas->queue_redraw();
		} else {
			clear_transform_guards();
		}

		// reset some values
		_canvas_need_update = false;
		grouped_text->end_text_group();
	}
}
#endif

void DebugDraw2D::_on_canvas_marked_dirty() {
	mark_canvas_dirty();
}

void DebugDraw2D::_on_canvas_item_draw(Control *ci) {
#ifndef DISABLE_DEBUG_RENDERING
	Vector2 vp_size = ci->has_meta("UseParentSize") ? Object::cast_to<Control>(ci->get_parent())->get_rect().size : ci->get_rect().size;

	_draw_2d_geometry(ci);
	grouped_text->draw(ci, _font, vp_size);
	data_graphs->draw(ci, _font, vp_size, ci->get_process_delta_time());

	clear_transform_guards();
#else
	return;
#endif
}

bool DebugDraw2D::_is_enabled_override() const {
	return debug_enabled && DebugDrawManager::get_singleton()->is_debug_enabled();
}

void DebugDraw2D::_draw_2d_geometry(Control *ci) {
	Window *root = SCENE_ROOT();
	Camera2D *cam = root->get_camera_2d();

	Vector2i size = root->get_size();
	Transform2D xform = root->get_viewport()->get_canvas_transform();

	Transform2D xform_sized = xform.scaled(size);
	Rect2 camera_rect = Rect2(Point2(), size);
	Rect2 camera_rect_res = xform.xform(camera_rect);

	// PRINT(camera_rect_res);

	if (cam) {
		// camera_rect = cam->get_rec;
	}
}

void DebugDraw2D::mark_canvas_dirty() {
	_canvas_need_update = true;
}

Node *DebugDraw2D::get_root_node() {
	return root_node;
}

void DebugDraw2D::set_empty_color(const Color &_col) {
}

Color DebugDraw2D::get_empty_color() const {
	return Colors::empty_color;
}

#pragma region Exposed Parameters

void DebugDraw2D::set_debug_enabled(const bool &_state) {
	debug_enabled = _state;

	if (!_state) {
		clear_all();
	}
}

bool DebugDraw2D::is_debug_enabled() const {
	return debug_enabled;
}

void DebugDraw2D::set_default_transform_2d(const Transform2D &_xform) {
	default_transform_2d = _xform;
}

Transform2D DebugDraw2D::get_default_transform_2d() const {
	return default_transform_2d;
}

void DebugDraw2D::set_config(Ref<DebugDrawConfig2D> _cfg) {
	if (_cfg.is_valid()) {
#ifndef DISABLE_DEBUG_RENDERING
		Utils::disconnect_safe(config, DebugDrawConfig2D::s_marked_dirty, Callable(this, NAMEOF(_on_canvas_marked_dirty)));
#endif

		config = _cfg;
	} else {
		config = Ref<DebugDrawConfig2D>();
		config.instantiate();
	}

#ifndef DISABLE_DEBUG_RENDERING
	mark_canvas_dirty();
	Utils::connect_safe(config, DebugDrawConfig2D::s_marked_dirty, Callable(this, NAMEOF(_on_canvas_marked_dirty)));
#endif
}

Ref<DebugDrawConfig2D> DebugDraw2D::get_config() const {
	return config;
}

Ref<DDTransform2DGuard> DebugDraw2D::draw_set_transform_2d(const Vector2 &position, double rotation, const Vector2 &scale) {
	Transform2D xform((real_t)rotation, position);
	xform.scale_basis(scale);

	return draw_set_transform_matrix_2d(xform);
}

Ref<DDTransform2DGuard> DebugDraw2D::draw_set_transform_matrix_2d(const Transform2D &xform) {
	auto guard = Ref<DDTransform2DGuard>(::godot::_post_initialize(new ("") DDTransform2DGuard(xform)));
	return guard;
}

Transform2D DebugDraw2D::get_transform_for_current_thread() {
	LOCK_GUARD(transform_stack_mutex);

	uint64_t thread_id = OS::get_singleton()->get_thread_caller_id();
	if (transform_stack.find(thread_id) != transform_stack.end()) {
		const auto &transforms = transform_stack[thread_id];
		if (transforms.size() > 0)
			return transforms.back().second;
	}

	return default_transform_2d;
}

void DebugDraw2D::register_transform_guard(uint64_t guard_id, uint64_t thread_id, const Transform2D &xform) {
	LOCK_GUARD(transform_stack_mutex);

	transform_stack[thread_id].push_back(transform_pair(guard_id, xform));
}

void DebugDraw2D::unregister_transform_guard(uint64_t guard_id, uint64_t thread_id) {
	LOCK_GUARD(transform_stack_mutex);

	auto &transforms = transform_stack[thread_id];
	auto res = std::find_if(transforms.rbegin(), transforms.rend(), [&guard_id](const transform_pair &i) { return i.first == guard_id; });

	if (res != transforms.rend()) {
		transforms.erase(--res.base());
	}
}

void DebugDraw2D::clear_transform_guards() {
	LOCK_GUARD(transform_stack_mutex);

	transform_stack.clear();
}

void DebugDraw2D::set_custom_canvas(Control *_canvas) {
#ifndef DISABLE_DEBUG_RENDERING
	if (!_canvas) {
		Utils::connect_safe(default_canvas, "draw", Callable(this, NAMEOF(_on_canvas_item_draw)).bindv(Array::make(default_canvas)));
		if (Utils::disconnect_safe(custom_canvas, "draw", Callable(this, NAMEOF(_on_canvas_item_draw))))
			custom_canvas->queue_redraw();
	} else {
		if (Utils::disconnect_safe(default_canvas, "draw", Callable(this, NAMEOF(_on_canvas_item_draw))))
			default_canvas->queue_redraw();
		Utils::connect_safe(custom_canvas, "draw", Callable(this, NAMEOF(_on_canvas_item_draw)).bindv(Array::make(_canvas)));
	}
#endif

	custom_canvas = _canvas;
}

Control *DebugDraw2D::get_custom_canvas() const {
	return custom_canvas;
}

#pragma endregion

#pragma region Draw Functions

Ref<DebugDrawStats2D> DebugDraw2D::get_render_stats() {
#ifndef DISABLE_DEBUG_RENDERING
	Ref<DebugDrawStats2D> stats;
	stats.instantiate();
	stats->setup(
			grouped_text->get_text_group_count(),
			grouped_text->get_text_line_total_count(),

			data_graphs->get_graphs_enabled(),
			data_graphs->get_graphs_total());

	return stats;
#else
	return Ref<DebugDrawStats2D>();
#endif
}

void DebugDraw2D::clear_all() {
#ifndef DISABLE_DEBUG_RENDERING
	if (!grouped_text || !data_graphs) return;
	grouped_text->clear_text();
	data_graphs->clear_graphs();
	mark_canvas_dirty();
	_finish_frame_and_update();
#else
	return;
#endif
}

#pragma region 2D

void DebugDraw2D::draw_line(const Vector2 &from, const Vector2 &to, const Color &color, double width, bool antialiased) {
}

#pragma region Text

#ifndef DISABLE_DEBUG_RENDERING
#define CALL_TO_2D(obj, func, ...)  \
	if (!obj || NEED_LEAVE) return; \
	obj->func(__VA_ARGS__)

#define FORCE_CALL_TO_2D(obj, func, ...) \
	if (!obj) return;                    \
	obj->func(__VA_ARGS__)

#define CALL_TO_2D_RET(obj, func, def, ...) \
	if (!obj || NEED_LEAVE) return def;     \
	return obj->func(__VA_ARGS__)

#define FORCE_CALL_TO_2D_RET(obj, func, def, ...) \
	if (!obj) return def;                         \
	return obj->func(__VA_ARGS__)
#else
#define CALL_TO_2D(obj, func, ...) return
#define FORCE_CALL_TO_2D(obj, func, ...) return
#define CALL_TO_2D_RET(obj, func, def, ...) return def
#define FORCE_CALL_TO_2D_RET(obj, func, def, ...) return def
#endif

void DebugDraw2D::begin_text_group(String group_title, int group_priority, Color group_color, bool show_title, int title_size, int text_size) {
	CALL_TO_2D(grouped_text, begin_text_group, group_title, group_priority, group_color, show_title, title_size, text_size);
}

void DebugDraw2D::end_text_group() {
	CALL_TO_2D(grouped_text, end_text_group);
}

void DebugDraw2D::set_text(String key, Variant value, int priority, Color color_of_value, real_t duration) {
	CALL_TO_2D(grouped_text, set_text, key, value, priority, color_of_value, duration);
}

#pragma endregion // Text
#pragma region Graphs

Ref<DebugDrawGraph> DebugDraw2D::create_graph(const StringName &title) {
	CALL_TO_2D_RET(data_graphs, create_graph, Ref<DebugDrawGraph>(), title);
}

Ref<DebugDrawGraph> DebugDraw2D::create_fps_graph(const StringName &title) {
	CALL_TO_2D_RET(data_graphs, create_fps_graph, Ref<DebugDrawGraph>(), title);
}

void DebugDraw2D::graph_update_data(const StringName &title, real_t data) {
	CALL_TO_2D(data_graphs, graph_update_data, title, data);
}

void DebugDraw2D::remove_graph(const StringName &title) {
	FORCE_CALL_TO_2D(data_graphs, remove_graph, title);
}

void DebugDraw2D::clear_graphs() {
	FORCE_CALL_TO_2D(data_graphs, clear_graphs);
}

Ref<DebugDrawGraph> DebugDraw2D::get_graph(const StringName &title) {
	FORCE_CALL_TO_2D_RET(data_graphs, get_graph, Ref<DebugDrawGraph>(), title);
}

PackedStringArray DebugDraw2D::get_graph_names() {
	FORCE_CALL_TO_2D_RET(data_graphs, get_graph_names, PackedStringArray());
}

#pragma endregion // Graphs
#pragma endregion // 2D

#pragma endregion // Draw Functions

#undef NEED_LEAVE
