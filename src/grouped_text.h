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

	Color ValueColor;

	// Just to show the text once and not delete it until the next frame ends.
	// It is necessary to avoid the endless re - creation of these objects.
	bool second_chance = true;

	TextGroupItem(double expirationTime, String key, String text, int priority, Color color);

	void update(double &expirationTime, String &key, String &text, int &priority, Color &color);
	bool is_expired();
};

typedef std::shared_ptr<TextGroupItem> TextGroupItem_ptr;

class TextGroup {
public:
	String Title;
	int GroupPriority;
	bool ShowTitle;
	Color GroupColor;
	std::unordered_set<TextGroupItem_ptr> Texts;

	TextGroup() :
			Title(""),
			GroupPriority(0),
			ShowTitle(true),
			GroupColor(Color()){

			};
	TextGroup(String title, int priority, bool showTitle, Color groupColor);
	void cleanup_texts(std::function<void()> update, double delta);
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
	void cleanup_text(double delta);
	void begin_text_group(String groupTitle, int groupPriority, Color groupColor, bool showTitle);
	void end_text_group();
	void set_text(String &key, Variant &value, int &priority, Color &colorOfValue, double duration);
	void draw(CanvasItem *ci, Ref<Font> _font, Vector2 vp_size);
};
