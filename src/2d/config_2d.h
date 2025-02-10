#pragma once

#include "common/colors.h"
#include "utils/compiler.h"

#include <functional>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

/**
 * @brief
 * This is a class for storing part of the DebugDraw2D configuration.
 *
 * You can use DebugDraw2D.get_config to get the current instance of the configuration.
 */
class DebugDraw2DConfig : public RefCounted {
	GDCLASS(DebugDraw2DConfig, RefCounted)

public:
	/**
	 * Available positions for placing text blocks.
	 */
	enum BlockPosition : int {
		POSITION_LEFT_TOP = 0,
		POSITION_RIGHT_TOP = 1,
		POSITION_LEFT_BOTTOM = 2,
		POSITION_RIGHT_BOTTOM = 3,
	};

private:
#pragma region Exposed Parameter Values

	/// @private
	void mark_canvas_dirty();

	// TEXT
	BlockPosition text_block_position = BlockPosition::POSITION_LEFT_TOP;
	Vector2i text_block_offset = Vector2i(8, 8);
	Vector2i text_padding = Vector2i(3, 1);
	real_t text_default_duration = 0.5f;
	int text_default_size = 12;
	Color text_foreground_color = Colors::white;
	Color text_background_color = Colors::gray_bg;
	Ref<Font> text_custom_font = nullptr;

#pragma endregion // Exposed Parameter Values

	std::function<void()> mark_dirty_func = nullptr;

protected:
	/// @private
	static void _bind_methods();

public:
	DebugDraw2DConfig();
	~DebugDraw2DConfig();

	/// @private
	void register_config(std::function<void()> p_mark_dirty);
	/// @private
	void unregister_config();

	/**
	 * Position of the text block
	 */
	void set_text_block_position(BlockPosition _position);
	BlockPosition get_text_block_position() const;

	/**
	 * Offset from the corner selected in 'set_text_block_position'
	 */
	void set_text_block_offset(const Vector2i &_offset);
	Vector2i get_text_block_offset() const;

	/**
	 * Text padding for each line
	 */
	void set_text_padding(const Vector2i &_padding);
	Vector2i get_text_padding() const;

	/**
	 * How long the text remains visible after creation.
	 */
	void set_text_default_duration(const real_t &_duration);
	real_t get_text_default_duration() const;

	/**
	 * Default text size
	 */
	void set_text_default_size(const int &_size);
	int get_text_default_size() const;

	/**
	 * Default color of the text
	 */
	void set_text_foreground_color(const Color &_new_color);
	Color get_text_foreground_color() const;

	/**
	 * Background color of the text
	 */
	void set_text_background_color(const Color &_new_color);
	Color get_text_background_color() const;

	/**
	 * Custom text Font
	 */
	void set_text_custom_font(const Ref<Font> &_custom_font);
	Ref<Font> get_text_custom_font() const;
};

VARIANT_ENUM_CAST(DebugDraw2DConfig::BlockPosition);
