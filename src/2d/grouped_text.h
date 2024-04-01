#pragma once
#ifndef DISABLE_DEBUG_RENDERING

#include "utils/compiler.h"
#include "utils/profiler.h"

#include <functional>
#include <memory>
#include <mutex>
#include <unordered_set>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/font.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class TextGroupItem {
public:
	String key;
	String text;
	int priority;
	double expiration_time;
	bool is_group_title = false;

	Color value_color;

	// Just to show the text once and not delete it until the next frame ends.
	// It is necessary to avoid the endless re - creation of these objects.
	bool second_chance = true;

	TextGroupItem(const double &p_expirationTime, const String &p_key, const String &p_text, const int &p_priority, const Color &p_color);

	bool update(const double &p_expirationTime, const String &p_key, const String &p_text, const int &p_priority, const Color &p_color);
	bool is_expired();
};

typedef std::shared_ptr<TextGroupItem> TextGroupItem_ptr;

class TextGroup {
private:
	int group_priority;
	bool show_title;
	Color group_color;
	int title_size;
	int text_size;

public:
	bool is_used_one_time = false;
	String title;
	std::vector<TextGroupItem_ptr> Texts;
	class DebugDraw2D *owner;

	void set_group_priority(int p_val);
	int get_group_priority();
	void set_show_title(bool p_val);
	bool is_show_title();
	void set_group_color(Color p_val);
	Color get_group_color();
	void set_title_size(int p_val);
	int get_title_size();
	void set_text_size(int p_val);
	int get_text_size();

	TextGroup() :
			group_priority(0),
			show_title(true),
			group_color(Color()),
			title_size(14),
			text_size(12),
			title(""),
			owner(nullptr){};
	TextGroup(class DebugDraw2D *p_owner, const String &p_title, const int &p_priority, const bool &p_show_title, const Color &p_group_color, const int &p_title_size, const int &p_text_size);
	void cleanup_texts(const std::function<void()> &p_update, const double &p_delta);
};

typedef std::shared_ptr<TextGroup> TextGroup_ptr;

class GroupedText {
	struct DrawRectInstance {
		Rect2 rect;
		Color color;
		bool filled;
		DrawRectInstance(const Rect2 &p_rect, const Color &p_col, const bool &p_filled = true) :
				rect(p_rect),
				color(p_col),
				filled(p_filled){};
	};

	struct DrawTextInstance {
		String text;
		Ref<Font> font;
		int font_size;
		Vector2 position;
		Color color;
		DrawTextInstance(const String &p_text, const Ref<Font> &p_font, const int &p_font_size, const Vector2 &p_pos, const Color &p_col) :
				text(p_text),
				font(p_font),
				font_size(p_font_size),
				position(p_pos),
				color(p_col){};
	};

	TextGroupItem_ptr item_for_title_of_groups;
	std::vector<TextGroup_ptr> _text_groups;
	TextGroup_ptr _current_text_group;
	class DebugDraw2D *owner = nullptr;

	ProfiledMutex(std::recursive_mutex, datalock, "Text lock");

	void _create_new_default_group_if_needed();

public:
	void init_group(class DebugDraw2D *p_owner);
	void clear_groups();
	void cleanup_text(const double &p_delta);
	void begin_text_group(const String &p_group_title, const int &p_group_priority, const Color &p_group_color, const bool &p_show_title, const int &p_title_size, const int &p_text_size);
	void end_text_group();
	void set_text(const String &p_key, const Variant &p_value, const int &p_priority, const Color &p_color_of_value, const double &p_duration);
	void draw(CanvasItem *p_ci, const Ref<Font> &p_font, const Vector2 &p_vp_size);

	size_t get_text_group_count();
	size_t get_text_line_total_count();
};

#endif