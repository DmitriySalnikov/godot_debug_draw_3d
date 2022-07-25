#pragma once

#include "math_utils.h"
#include "utils.h"

#include <CanvasItem.hpp>
#include <Font.hpp>
#include <Godot.hpp>

using namespace godot;

class TextGroupItem {
public:
	String Key;
	String Text;
	int Priority;
	int ms_expiration = 0;
	std::chrono::steady_clock::time_point updated_time;

	Color ValueColor = Colors::empty_color;

	// Just to show the text once and not delete it until the next frame ends.
	// It is necessary to avoid the endless re - creation of these objects.
	bool second_chance = true;

	TextGroupItem(int expirationTime, String key, String text, int priority, Color color);

	void update(int &expirationTime, String &key, String &text, int &priority, Color &color);
	bool IsExpired();
};

typedef std::shared_ptr<TextGroupItem> TextGroupItem_ptr;

class TextGroup {
public:
	String Title;
	int GroupPriority;
	bool ShowTitle;
	Color GroupColor;
	// TODO replace by unordered_map
	std::unordered_set<TextGroupItem_ptr> Texts;

	TextGroup() :
			Title(""),
			GroupPriority(0),
			ShowTitle(true),
			GroupColor(Color()){

			};
	TextGroup(String title, int priority, bool showTitle, Color groupColor);
	void CleanupTexts(std::function<void()> update);
};

typedef std::shared_ptr<TextGroup> TextGroup_ptr;

class GroupedText {

	TextGroupItem_ptr item_for_title_of_groups;
	std::unordered_set<TextGroup_ptr> _textGroups;
	TextGroup_ptr _currentTextGroup;
	class DebugDraw3D *owner;
	std::recursive_mutex datalock;

	void _create_new_default_groupd_if_needed();

public:
	GroupedText(class DebugDraw3D *p_owner);
	void clear_text();
	void cleanup_text();
	void begin_text_group(String groupTitle, int groupPriority, Color groupColor, bool showTitle);
	void end_text_group();
	void set_text(String &key, Variant &value, int &priority, Color &colorOfValue, real_t duration);
	void draw(CanvasItem *ci, Ref<Font> _font, Vector2 vp_size);
};