#include "debug_draw_2d.h"

#include "config_2d.h"
#include "debug_draw_manager.h"
#include "grouped_text.h"
#include "stats_2d.h"
#include "utils/utils.h"

GODOT_WARNING_DISABLE()
GODOT_WARNING_RESTORE()

#define NEED_LEAVE (!_is_enabled_override())

DebugDraw2D *DebugDraw2D::singleton = nullptr;

void DebugDraw2D::_bind_methods() {
#define REG_CLASS_NAME DebugDraw2D

	ClassDB::bind_method(D_METHOD(NAMEOF(_on_canvas_item_draw)), &DebugDraw2D::_on_canvas_item_draw);

#pragma region Parameters

	REG_PROP(empty_color, Variant::COLOR, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);
	REG_PROP_BOOL(debug_enabled, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);

	REG_PROP(config, Variant::OBJECT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);

	REG_PROP(custom_canvas, Variant::OBJECT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);

#pragma endregion
#undef REG_CLASS_NAME

#pragma region Draw Functions
	ClassDB::bind_method(D_METHOD(NAMEOF(clear_all)), &DebugDraw2D::clear_all);

	ClassDB::bind_method(D_METHOD(NAMEOF(begin_text_group), "group_title", "group_priority", "group_color", "show_title", "title_size", "text_size"), &DebugDraw2D::begin_text_group, 0, Colors::white_smoke, true, -1, -1);
	ClassDB::bind_method(D_METHOD(NAMEOF(end_text_group)), &DebugDraw2D::end_text_group);
	ClassDB::bind_method(D_METHOD(NAMEOF(set_text), "key", "value", "priority", "color_of_value", "duration"), &DebugDraw2D::set_text, Variant(), 0, Colors::empty_color, -1.0);
	ClassDB::bind_method(D_METHOD(NAMEOF(clear_texts)), &DebugDraw2D::clear_texts);

#pragma endregion // Draw Functions

	ClassDB::bind_method(D_METHOD(NAMEOF(get_render_stats)), &DebugDraw2D::get_render_stats);

	ADD_SIGNAL(MethodInfo(s_marked_dirty));
}

DebugDraw2D::DebugDraw2D() {
	ASSIGN_SINGLETON(DebugDraw2D);
}

void DebugDraw2D::init(DebugDrawManager *root) {
	ZoneScoped;
	root_node = root;
	call_canvas_item_draw_cache = Callable(this, NAMEOF(_on_canvas_item_draw));
	set_config(nullptr);
	stats_2d.instantiate();

#ifndef DISABLE_DEBUG_RENDERING
	grouped_text = std::make_unique<GroupedText>();
	grouped_text->init_group(this);
#endif
}

DebugDraw2D::~DebugDraw2D() {
	ZoneScoped;
	UNASSIGN_SINGLETON(DebugDraw2D);

#ifndef DISABLE_DEBUG_RENDERING
	grouped_text.reset();

	Control *default_control = Object::cast_to<Control>(ObjectDB::get_instance(default_control_id));
	Control *custom_control = Object::cast_to<Control>(ObjectDB::get_instance(custom_control_id));

	if (Utils::disconnect_safe(default_control, "draw", call_canvas_item_draw_cache))
		default_control->queue_redraw();

	if (Utils::disconnect_safe(custom_control, "draw", call_canvas_item_draw_cache))
		custom_control->queue_redraw();

	if (!IS_EDITOR_HINT()) {
		if (default_control)
			default_control->queue_free();

		default_control_id = 0;
	}
#endif

	if (config.is_valid()) {
		config->unregister_config();
	}

	root_node = nullptr;
}

#ifdef TRACY_ENABLE
static bool DebugDraw2D_frame_mark_2d_started = false;
#endif

void DebugDraw2D::process_start(double delta) {
}

void DebugDraw2D::process_end(double delta) {
	ZoneScoped;

#ifdef TRACY_ENABLE
	if (DebugDraw2D_frame_mark_2d_started) {
		FrameMarkEnd("2D Draw");
		FrameMark;
	}
	FrameMarkStart("2D Draw");

	DebugDraw2D_frame_mark_2d_started = true;
#endif

#ifndef DISABLE_DEBUG_RENDERING
	// Clean texts
	grouped_text->cleanup_text(delta);

	// Update overlay
	_finish_frame_and_update();
#endif
}

void DebugDraw2D::physics_process_start(double delta) {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	FrameMarkStart("2D Physics Step");
#endif
}

void DebugDraw2D::physics_process_end(double delta) {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	// TODO implement
	FrameMarkEnd("2D Physics Step");
#endif
}

#ifndef DISABLE_DEBUG_RENDERING
void DebugDraw2D::_finish_frame_and_update() {
	ZoneScoped;
	if (_canvas_need_update) {
		if (Control *custom_canvas = Object::cast_to<Control>(ObjectDB::get_instance(custom_control_id)); custom_canvas) {
			custom_canvas->queue_redraw();
		} else if (Control *default_control = Object::cast_to<Control>(ObjectDB::get_instance(default_control_id)); default_control) {
			default_control->queue_redraw();
		} else {
#ifdef TRACY_ENABLE
			if (DebugDraw2D_frame_mark_2d_started) {
				FrameMarkEnd("2D Draw");
				DebugDraw2D_frame_mark_2d_started = false;
			}
#endif
		}

		// reset some values
		_canvas_need_update = false;
		if (grouped_text)
			grouped_text->end_text_group();
	} else {
#ifdef TRACY_ENABLE
		if (DebugDraw2D_frame_mark_2d_started) {
			FrameMarkEnd("2D Draw");
			DebugDraw2D_frame_mark_2d_started = false;
		}
#endif
	}
}

void DebugDraw2D::_clear_all_internal() {
	if (grouped_text)
		grouped_text->clear_groups();
	mark_canvas_dirty();
	_finish_frame_and_update();

	_set_custom_canvas_internal(nullptr);
}

void DebugDraw2D::_set_custom_canvas_internal(Control *_canvas) {
	static std::function<Callable()> create_default = [this]() {
		return Callable(this, NAMEOF(_on_canvas_item_draw)).bindv(Array::make(ObjectDB::get_instance(default_control_id)));
	};
	static std::function<Callable()> create_custom = [this]() {
		return Callable(this, NAMEOF(_on_canvas_item_draw)).bindv(Array::make(ObjectDB::get_instance(custom_control_id)));
	};

	Control *default_control = Object::cast_to<Control>(ObjectDB::get_instance(default_control_id));
	Control *old_custom_canvas = Object::cast_to<Control>(ObjectDB::get_instance(custom_control_id));

	if (!_canvas) {
		Utils::connect_safe(default_control, "draw", call_canvas_item_draw_cache, 0, nullptr, create_default);
		if (Utils::disconnect_safe(old_custom_canvas, "draw", call_canvas_item_draw_cache)) {
			old_custom_canvas->queue_redraw();
		}
		custom_control_id = 0;
	} else {
		if (Utils::disconnect_safe(default_control, "draw", call_canvas_item_draw_cache)) {
			default_control->queue_redraw();
		}
		custom_control_id = _canvas->get_instance_id();

		if (old_custom_canvas != _canvas && Utils::disconnect_safe(old_custom_canvas, "draw", call_canvas_item_draw_cache)) {
			old_custom_canvas->queue_redraw();
		}

		Utils::connect_safe(_canvas, "draw", call_canvas_item_draw_cache, 0, nullptr, create_custom);
	}
}
#endif

void DebugDraw2D::_on_canvas_item_draw(Control *ci) {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	if (!ci) return;
	Vector2 vp_size = ci->has_meta("UseParentSize") ? Object::cast_to<Control>(ci->get_parent())->get_rect().size : ci->get_rect().size;

	grouped_text->draw(ci, _font, vp_size);
#endif

#ifdef TRACY_ENABLE
	if (DebugDraw2D_frame_mark_2d_started) {
		FrameMarkEnd("2D Draw");
		DebugDraw2D_frame_mark_2d_started = false;
	}
#endif
	FrameMark;
}

bool DebugDraw2D::_is_enabled_override() const {
	return debug_enabled && DebugDrawManager::get_singleton()->is_debug_enabled();
}

void DebugDraw2D::mark_canvas_dirty() {
	if (!_canvas_need_update)
		emit_signal(s_marked_dirty);

	_canvas_need_update = true;
}

bool DebugDraw2D::is_drawing_frame() const {
	return _is_drawing_frame;
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

void DebugDraw2D::set_config(Ref<DebugDraw2DConfig> _cfg) {
	ZoneScoped;
	if (_cfg.is_valid()) {
#ifndef DISABLE_DEBUG_RENDERING
		config->unregister_config();
#endif

		config = _cfg;
	} else {
		config = Ref<DebugDraw2DConfig>();
		config.instantiate();

#ifndef DISABLE_DEBUG_RENDERING
		config->register_config([this]() { mark_canvas_dirty(); });
#endif
	}

#ifndef DISABLE_DEBUG_RENDERING
	mark_canvas_dirty();
#endif
}

Ref<DebugDraw2DConfig> DebugDraw2D::get_config() const {
	return config;
}

void DebugDraw2D::set_custom_canvas(Control *_canvas) {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	_set_custom_canvas_internal(_canvas);
#else
	custom_control_id = _canvas ? _canvas->get_instance_id() : 0;
#endif
}

Control *DebugDraw2D::get_custom_canvas() const {
	return Object::cast_to<Control>(ObjectDB::get_instance(custom_control_id));
}

#pragma endregion

#pragma region Draw Functions

Ref<DebugDraw2DStats> DebugDraw2D::get_render_stats() {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	stats_2d->setup(
			grouped_text->get_text_group_count(),
			grouped_text->get_text_line_total_count());
#endif

	return stats_2d;
}

void DebugDraw2D::clear_all() {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	_clear_all_internal();
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
	ZoneScoped;
	CALL_TO_2D(grouped_text, begin_text_group, group_title, group_priority, group_color, show_title, title_size, text_size);
}

void DebugDraw2D::end_text_group() {
	ZoneScoped;
	CALL_TO_2D(grouped_text, end_text_group);
}

void DebugDraw2D::set_text(String key, Variant value, int priority, Color color_of_value, real_t duration) {
	ZoneScoped;
	CALL_TO_2D(grouped_text, set_text, key, value, priority, color_of_value, duration);
}

void DebugDraw2D::clear_texts() {
	ZoneScoped;
	FORCE_CALL_TO_2D(grouped_text, clear_groups);
}

#pragma endregion // Text
#pragma endregion // 2D

#pragma endregion // Draw Functions

#undef NEED_LEAVE