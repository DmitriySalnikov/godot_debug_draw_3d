#pragma once

#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#endif

#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/variant/builtin_types.hpp>

#if defined(_MSC_VER)
#pragma warning(default : 4244)
#endif

#include <functional>
#include <memory>
#include <mutex>
#include <unordered_set>

using namespace godot;

class TextGroupItem {
public:
	String Key;
	String Text;
	int Priority;
	double expiration_time;
	bool is_group_title = false;

	Color ValueColor;

	// Just to show the text once and not delete it until the next frame ends.
	// It is necessary to avoid the endless re - creation of these objects.
	bool second_chance = true;

	TextGroupItem(const double &_expirationTime, const String &key, const String &text, const int &priority, const Color &color);

	void update(const double &_expirationTime, const String &key, const String &text, const int &priority, const Color &color);
	bool is_expired();
};

typedef std::shared_ptr<TextGroupItem> TextGroupItem_ptr;

class TextGroup {
public:
	String title;
	int group_priority;
	bool show_title;
	Color group_color;
	int title_size;
	int text_size;
	std::unordered_set<TextGroupItem_ptr> Texts;

	TextGroup() :
			title(""),
			group_priority(0),
			show_title(true),
			group_color(Color()),
			title_size(14),
			text_size(12){

			};
	TextGroup(const String &_title, const int &priority, const bool &showTitle, const Color &groupColor, const int &titleSize, const int &textSize);
	void cleanup_texts(const std::function<void()> &update, const double &delta);
};

typedef std::shared_ptr<TextGroup> TextGroup_ptr;

class GroupedText {

	TextGroupItem_ptr item_for_title_of_groups;
	std::unordered_set<TextGroup_ptr> _textGroups;
	TextGroup_ptr _currentTextGroup;
	class DebugDraw *owner;
	std::recursive_mutex datalock;

	void _create_new_default_groupd_if_needed();

public:
	void init_group(class DebugDraw *p_owner);
	void clear_text();
	void cleanup_text(const double &delta);
	void begin_text_group(const String &groupTitle, const int &groupPriority, const Color &groupColor, const bool &showTitle, const int &titleSize, const int &textSize);
	void end_text_group();
	void set_text(const String &key, const Variant &value, const int &priority, const Color &colorOfValue, const double &duration);
	void draw(CanvasItem *ci, const Ref<Font> &_font, const Vector2 &vp_size);
};
