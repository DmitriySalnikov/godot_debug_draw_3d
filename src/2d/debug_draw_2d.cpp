#include "debug_draw_2d.h"

#include "config_2d.h"
#include "debug_draw_manager.h"
#include "graphs.h"
#include "grouped_text.h"
#include "stats_2d.h"
#include "utils/utils.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/standard_material3d.hpp>
GODOT_WARNING_RESTORE()

#include <limits.h>

#define NEED_LEAVE (!debug_enabled)

DebugDraw2D *DebugDraw2D::singleton = nullptr;

void DebugDraw2D::_bind_methods() {
#define REG_CLASS_NAME DebugDraw2D

	ClassDB::bind_method(D_METHOD(NAMEOF(_on_canvas_marked_dirty)), &DebugDraw2D::_on_canvas_marked_dirty);
	ClassDB::bind_method(D_METHOD(NAMEOF(_on_canvas_item_draw)), &DebugDraw2D::_on_canvas_item_draw);

#pragma region Parameters

	REG_PROP(empty_color, Variant::COLOR);
	REG_PROP_BOOL(debug_enabled);

	REG_PROP(config, Variant::OBJECT);

	REG_PROP(custom_canvas, Variant::OBJECT);

#pragma endregion
#undef REG_CLASS_NAME

#pragma region Draw Functions
	ClassDB::bind_method(D_METHOD(NAMEOF(clear_objects)), &DebugDraw2D::clear_objects);

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
	_font.unref();

	data_graphs.reset();
	grouped_text.reset();

	if (Utils::disconnect_safe(default_canvas, "draw", Callable(this, NAMEOF(_on_canvas_item_draw))))
		default_canvas->queue_redraw();
	if (Utils::disconnect_safe(custom_canvas, "draw", Callable(this, NAMEOF(_on_canvas_item_draw))))
		custom_canvas->queue_redraw();

	if (!IS_EDITOR_HINT()) {
		if (UtilityFunctions::is_instance_valid(_canvas_layer))
			_canvas_layer->queue_free();
		if (UtilityFunctions::is_instance_valid(default_canvas))
			default_canvas->queue_free();

		_canvas_layer = nullptr;
		default_canvas = nullptr;
	}
#endif

	root_node = nullptr;
	config.unref();
}

void DebugDraw2D::process(double delta) {
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
#endif
}

void DebugDraw2D::_on_canvas_marked_dirty() {
	mark_canvas_dirty();
}

void DebugDraw2D::_on_canvas_item_draw(Control *ci) {
#ifndef DISABLE_DEBUG_RENDERING
	Vector2 vp_size = ci->has_meta("UseParentSize") ? Object::cast_to<Control>(ci->get_parent())->get_rect().size : ci->get_rect().size;

	grouped_text->draw(ci, _font, vp_size);
	data_graphs->draw(ci, _font, vp_size, ci->get_process_delta_time());
#else
	return;
#endif
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
		clear_objects();
	}
}

bool DebugDraw2D::is_debug_enabled() const {
	return debug_enabled;
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

void DebugDraw2D::clear_objects() {
#ifndef DISABLE_DEBUG_RENDERING
	if (!grouped_text || !data_graphs) return;
	grouped_text->clear_text();
	data_graphs->clear_graphs();
	mark_canvas_dirty();
#else
	return;
#endif
}

#pragma region 2D
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