#pragma once
#ifndef DISABLE_DEBUG_RENDERING

#include "utils_compiler.h"

MSVC_WARNING_DISABLE(4244)
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/variant/builtin_types.hpp>
MSVC_WARNING_RESTORE(4244)

#include <functional>
#include <memory>
#include <mutex>
#include <unordered_set>

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

	TextGroupItem(const double &_expirationTime, const String &_key, const String &_text, const int &_priority, const Color &_color);

	bool update(const double &_expirationTime, const String &_key, const String &_text, const int &_priority, const Color &_color);
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
	std::unordered_set<TextGroupItem_ptr> Texts;
	class DebugDraw *owner;

	void set_group_priority(int _val);
	int get_group_priority();
	void set_show_title(bool _val);
	bool is_show_title();
	void set_group_color(Color _val);
	Color get_group_color();
	void set_title_size(int _val);
	int get_title_size();
	void set_text_size(int _val);
	int get_text_size();

	TextGroup() :
			title(""),
			group_priority(0),
			show_title(true),
			group_color(Color()),
			title_size(14),
			text_size(12),
			owner(nullptr){};
	TextGroup(class DebugDraw *_owner, const String &_title, const int &_priority, const bool &_show_title, const Color &_group_color, const int &_title_size, const int &_text_size);
	void cleanup_texts(const std::function<void()> &_update, const double &_delta);
};

typedef std::shared_ptr<TextGroup> TextGroup_ptr;

class GroupedText {
	TextGroupItem_ptr item_for_title_of_groups;
	std::unordered_set<TextGroup_ptr> _text_groups;
	TextGroup_ptr _current_text_group;
	class DebugDraw *owner = nullptr;
	std::recursive_mutex datalock;

	void _create_new_default_groupd_if_needed();

public:
	void init_group(class DebugDraw *p_owner);
	void clear_text();
	void cleanup_text(const double &_delta);
	void begin_text_group(const String &_group_title, const int &_group_priority, const Color &_group_color, const bool &_show_title, const int &_title_size, const int &_text_size);
	void end_text_group();
	void set_text(const String &_key, const Variant &_value, const int &_priority, const Color &_color_of_value, const double &_duration);
	void draw(CanvasItem *_ci, const Ref<Font> &_font, const Vector2 &_vp_size);
};

#endif