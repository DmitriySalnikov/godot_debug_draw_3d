#include "debug_draw_config_2d.h"
#include "utils.h"

#include "data_graphs.h"
#include "debug_geometry_container.h"
#include "grouped_text.h"

#include <limits.h>

const char *DebugDrawConfig2D::s_marked_dirty = "marked_dirty";

void DebugDrawConfig2D::_bind_methods() {
#define REG_CLASS_NAME DebugDrawConfig2D

	BIND_ENUM_CONSTANT(POSITION_LEFT_TOP);
	BIND_ENUM_CONSTANT(POSITION_RIGHT_TOP);
	BIND_ENUM_CONSTANT(POSITION_LEFT_BOTTOM);
	BIND_ENUM_CONSTANT(POSITION_RIGHT_BOTTOM);

	REG_PROP(graphs_base_offset, Variant::VECTOR2I);
	REG_PROP(text_block_position, Variant::INT);
	REG_PROP(text_block_offset, Variant::VECTOR2I);
	REG_PROP(text_padding, Variant::VECTOR2I);
	REG_PROP(text_default_duration, Variant::FLOAT);
	REG_PROP(text_default_size, Variant::INT);
	REG_PROP(text_foreground_color, Variant::COLOR);
	REG_PROP(text_background_color, Variant::COLOR);
	REG_PROP(text_custom_font, Variant::OBJECT);

	ADD_SIGNAL(MethodInfo(s_marked_dirty));
#undef REG_CLASS_NAME
}

DebugDrawConfig2D::~DebugDrawConfig2D() {
	text_custom_font.unref();
}

DebugDrawConfig2D::DebugDrawConfig2D() {
	if (IS_EDITOR_HINT()) {
		text_block_position = BlockPosition::POSITION_LEFT_BOTTOM;
		text_block_offset = Vector2(24, 24);
		graphs_base_offset = Vector2(12, 72);
	}
}

void DebugDrawConfig2D::mark_canvas_dirty() {
	emit_signal(s_marked_dirty);
}

void DebugDrawConfig2D::set_graphs_base_offset(const Vector2i &_offset) {
	if (graphs_base_offset != _offset)
		mark_canvas_dirty();
	graphs_base_offset = _offset;
}

Vector2i DebugDrawConfig2D::get_graphs_base_offset() const {
	return graphs_base_offset;
}

void DebugDrawConfig2D::set_text_block_position(BlockPosition _position) {
	if (text_block_position != (BlockPosition)_position)
		mark_canvas_dirty();
	text_block_position = (BlockPosition)_position;
}

DebugDrawConfig2D::BlockPosition DebugDrawConfig2D::get_text_block_position() const {
	return (BlockPosition)text_block_position;
}

void DebugDrawConfig2D::set_text_block_offset(const Vector2i &_offset) {
	if (text_block_offset != _offset)
		mark_canvas_dirty();
	text_block_offset = _offset;
}

Vector2i DebugDrawConfig2D::get_text_block_offset() const {
	return text_block_offset;
}

void DebugDrawConfig2D::set_text_padding(const Vector2i &_padding) {
	if (text_padding != _padding)
		mark_canvas_dirty();
	text_padding = _padding;
	text_padding.x = Math::clamp(text_padding.x, 0, INT_MAX);
	text_padding.y = Math::clamp(text_padding.y, 0, INT_MAX);
}

Vector2i DebugDrawConfig2D::get_text_padding() const {
	return text_padding;
}

void DebugDrawConfig2D::set_text_default_duration(const real_t &_duration) {
	text_default_duration = _duration;
}

real_t DebugDrawConfig2D::get_text_default_duration() const {
	return text_default_duration;
}

void DebugDrawConfig2D::set_text_default_size(const int &_size) {
	if (text_default_size != _size)
		mark_canvas_dirty();
	text_default_size = Math::clamp(_size, 1, INT_MAX);
}

int DebugDrawConfig2D::get_text_default_size() const {
	return text_default_size;
}

void DebugDrawConfig2D::set_text_foreground_color(const Color &_new_color) {
	if (text_foreground_color != _new_color)
		mark_canvas_dirty();
	text_foreground_color = _new_color;
}

Color DebugDrawConfig2D::get_text_foreground_color() const {
	return text_foreground_color;
}

void DebugDrawConfig2D::set_text_background_color(const Color &_new_color) {
	if (text_background_color != _new_color)
		mark_canvas_dirty();
	text_background_color = _new_color;
}

Color DebugDrawConfig2D::get_text_background_color() const {
	return text_background_color;
}

void DebugDrawConfig2D::set_text_custom_font(const Ref<Font> &_custom_font) {
	if (text_custom_font != _custom_font)
		mark_canvas_dirty();
	text_custom_font = _custom_font;
}

Ref<Font> DebugDrawConfig2D::get_text_custom_font() const {
	return text_custom_font;
}
