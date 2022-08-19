#pragma once

#include <CanvasItem.hpp>
#include <Font.hpp>
#include <Godot.hpp>

#include <memory>
#include <unordered_set>
#include <functional>
#include <mutex>

using namespace godot;

class TextGroupItem {
public:
	String Key;
	String Text;
	int Priority;
	real_t expiration_time;

	Color ValueColor;

	// Just to show the text once and not delete it until the next frame ends.
	// It is necessary to avoid the endless re - creation of these objects.
	bool second_chance = true;

	TextGroupItem(real_t expirationTime, String key, String text, int priority, Color color);

	void update(real_t &expirationTime, String &key, String &text, int &priority, Color &color);
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
	void cleanup_texts(std::function<void()> update, real_t delta);
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
	void cleanup_text(real_t delta);
	void begin_text_group(String groupTitle, int groupPriority, Color groupColor, bool showTitle);
	void end_text_group();
	void set_text(String &key, Variant &value, int &priority, Color &colorOfValue, real_t duration);
	void draw(CanvasItem *ci, Ref<Font> _font, Vector2 vp_size);
};