#include "config_2d.h"
#include "utils/utils.h"

#include "graphs.h"
#include "grouped_text.h"

#include <limits.h>

const char *DebugDrawConfig2D::s_marked_dirty = "marked_dirty";

void DebugDrawConfig2D::_bind_methods() {
#ifdef DEV_ENABLED
	ClassDB::bind_method(D_METHOD(NAMEOF(api_test1)), &DebugDrawConfig2D::api_test1);
	ClassDB::bind_method(D_METHOD(NAMEOF(api_test2)), &DebugDrawConfig2D::api_test2);
	ClassDB::bind_method(D_METHOD(NAMEOF(api_test3)), &DebugDrawConfig2D::api_test3);
	ClassDB::bind_method(D_METHOD(NAMEOF(api_test4)), &DebugDrawConfig2D::api_test4);
	ClassDB::bind_method(D_METHOD(NAMEOF(api_test5)), &DebugDrawConfig2D::api_test5);

	ClassDB::bind_method(D_METHOD(NAMEOF(api_test6)), &DebugDrawConfig2D::api_test6, 1, memnew(Object), true, 2, 4, 3.5f, "String", "StringName", "Node/Path");
	ClassDB::bind_method(D_METHOD(NAMEOF(api_test7)), &DebugDrawConfig2D::api_test7, Color(1, 2, 3, 4), Vector2(1, 2), Vector2i(3, 4), Vector3(1, 2, 3), Vector3i(4, 5, 6), Vector4(1, 2, 3, 4), Vector4i(5, 6, 7, 8), Rect2(Vector2(1, 2), Vector2(3, 4)), Rect2(Vector2(5, 6), Vector2(7, 8)));
	ClassDB::bind_method(D_METHOD(NAMEOF(api_test8)), &DebugDrawConfig2D::api_test8, Transform2D(Vector2(1, 2), Vector2(3, 4), Vector2(5, 6)), Transform3D(Vector3(1, 2, 3), Vector3(4, 5, 6), Vector3(7, 8, 9), Vector3(10, 11, 12)), Plane(1, 2, 3, 4), Quaternion(1, 2, 3, 4), AABB(Vector3(1, 2, 3), Vector3(4, 5, 6)), Basis(Vector3(1, 2, 3), Vector3(4, 5, 6), Vector3(7, 8, 9)), Projection(Vector4(1, 2, 3, 4), Vector4(5, 6, 7, 8), Vector4(9, 10, 11, 12), Vector4(13, 14, 15, 16)));
	ClassDB::bind_method(D_METHOD(NAMEOF(api_test9)), &DebugDrawConfig2D::api_test9, RID(), Callable(), Signal(), Utils::make_dict("test", 2, Vector2(2, 1), Plane(4, 3, 2, 1)), Array::make(1, "test"));
	ClassDB::bind_method(D_METHOD(NAMEOF(api_test10)), &DebugDrawConfig2D::api_test10, PackedByteArray(TypedArray<uint8_t>::make(1, 2, 3, 4)), PackedInt32Array(), PackedInt64Array(TypedArray<int64_t>::make(-1, -2, -3, -4)), PackedFloat32Array(TypedArray<float>::make(0.1f, 0.2f, 0.3f, 0.4f)), PackedFloat64Array(TypedArray<double>::make(10.5, 20.5, 30.5, 40.5)), PackedStringArray(TypedArray<String>::make("1", "2", "3", "4")), PackedVector2Array(TypedArray<String>::make(Vector2(8, 9), Vector2(28, 39))), PackedVector3Array(TypedArray<String>::make(Vector3(7, 8, 9), Vector3(9, 2, 3))), PackedColorArray(TypedArray<String>::make(Color(1, 1, 0, 0.5), Color(1, 0, 1))));
#endif

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
