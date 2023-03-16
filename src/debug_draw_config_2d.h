#pragma once

#include "colors.h"

MSVC_WARNING_DISABLE(4244)
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
MSVC_WARNING_RESTORE(4244)

using namespace godot;

class DebugDrawConfig2D : public RefCounted {
	GDCLASS(DebugDrawConfig2D, RefCounted)

public:
	enum BlockPosition : int {
		POSITION_LEFT_TOP = 0,
		POSITION_RIGHT_TOP = 1,
		POSITION_LEFT_BOTTOM = 2,
		POSITION_RIGHT_BOTTOM = 3,
	};

private:
#pragma region Exposed Parameter Values

	void mark_canvas_dirty();
	// 2D
	/// Base offset for all graphs
	Vector2i graphs_base_offset = Vector2i(8, 8);

	// TEXT
	/// Position of text block
	BlockPosition text_block_position = BlockPosition::POSITION_LEFT_TOP;
	/// Offset from the corner selected in 'text_block_position'
	Vector2i text_block_offset = Vector2i(8, 8);
	/// Text padding for each line
	Vector2i text_padding = Vector2i(3, 1);
	/// How long text remain shown after being invoked.
	real_t text_default_duration = 0.5f;
	/// Default text size
	int text_default_size = 12;
	/// Default color of the text
	Color text_foreground_color = Colors::white;
	/// Background color of the text
	Color text_background_color = Colors::gray_bg;
	/// Custom text Font
	Ref<Font> text_custom_font = nullptr;

#pragma endregion // Exposed Parameter Values

protected:
	static void _bind_methods();

public:
	const static char *s_marked_dirty;

	DebugDrawConfig2D();

	void set_graphs_base_offset(const Vector2i &_offset);
	Vector2i get_graphs_base_offset() const;

	void set_text_block_position(BlockPosition _position);
	BlockPosition get_text_block_position() const;

	void set_text_block_offset(const Vector2i &_offset);
	Vector2i get_text_block_offset() const;

	void set_text_padding(const Vector2i &_padding);
	Vector2i get_text_padding() const;

	void set_text_default_duration(const real_t &_duration);
	real_t get_text_default_duration() const;

	void set_text_default_size(const int &_size);
	int get_text_default_size() const;

	void set_text_foreground_color(const Color &_new_color);
	Color get_text_foreground_color() const;

	void set_text_background_color(const Color &_new_color);
	Color get_text_background_color() const;

	void set_text_custom_font(const Ref<Font> &_custom_font);
	Ref<Font> get_text_custom_font() const;
};

VARIANT_ENUM_CAST(DebugDrawConfig2D::BlockPosition);
