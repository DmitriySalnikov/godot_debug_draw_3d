#pragma once

#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#endif

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/variant/builtin_types.hpp>

#if defined(_MSC_VER)
#pragma warning(default : 4244)
#endif

using namespace godot;

struct DrawCachedRect {
	Rect2 rect;
	Color color;
	bool filled;
	DrawCachedRect(const Rect2 &_rect, const Color &_col, const bool &_filled = true) :
			rect(_rect),
			color(_col),
			filled(_filled){};
};

struct DrawCachedMultiline {
	PackedVector2Array lines;
	Color color;
	real_t width;
	DrawCachedMultiline(const PackedVector2Array &_lines, const Color &_col, const real_t &_width) :
			lines(_lines),
			color(_col),
			width(_width){};
};

struct DrawCachedText {
	String text;
	Ref<Font> font;
	int font_size;
	Vector2 position;
	Color color;
	DrawCachedText(const String &_text, const Ref<Font> &_font, const int &_font_size, const Vector2 &_pos, const Color &_col) :
			text(_text),
			font(_font),
			font_size(_font_size),
			position(_pos),
			color(_col){};
};