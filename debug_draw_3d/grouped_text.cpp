#include "grouped_text.h"
#include "debug_draw.h"

using namespace godot;

TextGroupItem::TextGroupItem(int expirationTime, String key, String text, int priority, Color color) {
	DEBUG_PRINT("New TextGroupItem created: " + key + " : " + text);

	updated_time = TIME_NOW();
	ms_expiration = expirationTime;
	Key = key;
	Text = text;
	Priority = priority;
	ValueColor = color;
	second_chance = true;
}

void TextGroupItem::Update(int &expirationTime, String &key, String &text, int &priority, Color &color) {
	updated_time = TIME_NOW();
	ms_expiration = expirationTime;
	Key = key;
	Text = text;
	Priority = priority;
	ValueColor = color;
	second_chance = true;
}

bool TextGroupItem::IsExpired() {
	if (second_chance) {
		second_chance = false;
		return false;
	}
	return TIME_TO_MS(TIME_NOW() - updated_time).count() > ms_expiration;
}

TextGroup::TextGroup(String title, int priority, bool showTitle, Color groupColor) {
	DEBUG_PRINT("New TextGroup created: " + title);

	Title = title;
	GroupPriority = priority;
	ShowTitle = showTitle;
	GroupColor = groupColor;
}

void TextGroup::CleanupTexts(std::function<void()> update) {
	std::unordered_set<TextGroupItem_ptr> keysToRemove;
	keysToRemove.reserve(Texts.size() / 2);

	for (const TextGroupItem_ptr &k : Texts) {
		if (k->IsExpired()) {
			keysToRemove.insert(k);
		}
	}

	for (const TextGroupItem_ptr &k : keysToRemove) {
		Texts.erase(k);
	}

	if (keysToRemove.size() > 0 && update)
		update();
}

GroupedText::GroupedText(class DebugDraw3D *p_owner) :
		owner(p_owner),
		_currentTextGroup(nullptr),
		item_for_title_of_groups(std::make_shared<TextGroupItem>(0, "", "", 0, Utils::empty_color)) {
}

void GroupedText::cleanup_text() {
	// Clean texts
	Utils::remove_where(_textGroups, [](auto g) { return g->Texts.size() == 0; });

	for (const TextGroup_ptr &g : _textGroups) {
		std::function<void()> upd_txt([this] { owner->mark_canvas_needs_update(); });
		g->CleanupTexts(upd_txt);
	}
}

void GroupedText::begin_text_group(String groupTitle, int groupPriority, Color groupColor, bool showTitle) {
	LOCK_GUARD(datalock);

	TextGroup_ptr newGroup = nullptr;
	for (const TextGroup_ptr &g : _textGroups) {
		if (g->Title == groupTitle) {
			newGroup = g;
			break;
		}
	}

	if (newGroup) {
		newGroup->ShowTitle = showTitle;
		newGroup->GroupPriority = groupPriority;
		newGroup->GroupColor = groupColor;
	} else {
		newGroup = std::make_shared<TextGroup>(groupTitle, groupPriority, showTitle, groupColor);
		_textGroups.insert(newGroup);
	}

	_currentTextGroup = newGroup;
}

void GroupedText::end_text_group() {
	LOCK_GUARD(datalock);

	_currentTextGroup = nullptr;
	for (const TextGroup_ptr &g : _textGroups) {
		if (g->Title == "") {
			_currentTextGroup = g;
			_currentTextGroup->GroupColor = owner->get_text_foreground_color();
			_currentTextGroup->GroupPriority = 0;
			_currentTextGroup->ShowTitle = false;
			break;
		}
	}

	if (!_currentTextGroup) {
		_currentTextGroup = std::make_shared<TextGroup>("", 0, false, owner->get_text_foreground_color());
		_textGroups.insert(_currentTextGroup);
	}
}

bool GroupedText::set_text(String &key, Variant &value, int &priority, Color &colorOfValue, float &duration) {
	int exp_time = (int)(duration * 1000);
	String _strVal = value.operator godot::String();
	bool need_update_canvas = false;

	{
		LOCK_GUARD(datalock);

		TextGroupItem_ptr item = Utils::find(_currentTextGroup->Texts, [key](TextGroupItem_ptr a) { return a->Key == key; });
		if (item.get()) {
			if (_strVal != item->Text)
				need_update_canvas = true;

			item->Update(exp_time, key, _strVal, priority, colorOfValue);
		} else {
			_currentTextGroup->Texts.insert(std::make_shared<TextGroupItem>(exp_time, key, _strVal, priority, colorOfValue));
			need_update_canvas = true;
		}
	}
	return need_update_canvas;
}

void GroupedText::draw(CanvasItem *ci, Ref<Font> _font, Vector2 vp_size) {
	LOCK_GUARD(datalock);

	int count = Utils::sum(_textGroups, [](TextGroup_ptr g) { return (int)g->Texts.size() + (g->ShowTitle ? 1 : 0); });

	static const String separator = " : ";

	Ref<Font> draw_font = owner->get_text_custom_font().is_null() ? _font : owner->get_text_custom_font();

	Vector2 ascent = Vector2(0, draw_font->get_ascent());
	Vector2 text_padding = owner->get_text_padding();
	Vector2 font_offset = ascent + text_padding;
	float line_height = draw_font->get_height() + text_padding.y * 2;
	Vector2 pos = Vector2::ZERO;
	float size_mul = 0;

	Vector2 text_block_offset = owner->get_text_block_offset();
	switch (owner->get_text_block_position()) {
		case DebugDraw3D::BlockPosition::LeftTop:
			pos = text_block_offset;
			size_mul = 0;
			break;
		case DebugDraw3D::BlockPosition::RightTop:
			pos = Vector2(
					vp_size.x - text_block_offset.x,
					text_block_offset.y);
			size_mul = -1;
			break;
		case DebugDraw3D::BlockPosition::LeftBottom:
			pos = Vector2(
					text_block_offset.x,
					vp_size.y - text_block_offset.y - line_height * count);
			size_mul = 0;
			break;
		case DebugDraw3D::BlockPosition::RightBottom:
			pos = Vector2(
					vp_size.x - text_block_offset.x,
					vp_size.y - text_block_offset.y - line_height * count);
			size_mul = -1;
			break;
	}

	std::vector<TextGroup_ptr> ordered_groups = Utils::order_by(_textGroups,
			[](TextGroup_ptr const &a, TextGroup_ptr const &b) { return a->GroupPriority < b->GroupPriority; });

	for (const TextGroup_ptr &g : ordered_groups) {
		auto group_items = Utils::order_by(g->Texts, [](TextGroupItem_ptr const &a, TextGroupItem_ptr const &b) {
			return a->Priority < b->Priority || (a->Priority == b->Priority && a->Key.naturalnocasecmp_to(b->Key) < 0);
		});

		if (g->ShowTitle) {
			item_for_title_of_groups->Key = g->Title;
			group_items.insert(group_items.begin(), item_for_title_of_groups);
		}

		for (const TextGroupItem_ptr &t : group_items) {
			String keyText = t->Key;
			bool is_title_only = !t.get() || t->Text == "";
			String text = is_title_only ? keyText : keyText + separator + t->Text;
			Vector2 size = draw_font->get_string_size(text);

			float size_right_revert = (size.x + text_padding.x * 2) * size_mul;
			ci->draw_rect(
					Rect2(Vector2(pos.x + size_right_revert, pos.y),
							Vector2(size.x + text_padding.x * 2, line_height)),
					owner->get_text_background_color());

			// Draw colored string
			if ((t.get() && t->ValueColor == Utils::empty_color) || is_title_only) {
				// Both parts with same color
				ci->draw_string(draw_font, Vector2(pos.x + font_offset.x + size_right_revert, pos.y + font_offset.y), text, g->GroupColor);
			} else {
				// Both parts with different colors
				String textSep = keyText + separator;
				int _keyLength = textSep.length();
				ci->draw_string(draw_font,
						Vector2(pos.x + font_offset.x + size_right_revert, pos.y + font_offset.y),
						text.substr(0, _keyLength), g->GroupColor);
				ci->draw_string(draw_font,
						Vector2(pos.x + font_offset.x + size_right_revert + draw_font->get_string_size(textSep).x, pos.y + font_offset.y),
						text.substr(_keyLength, text.length() - _keyLength), t->ValueColor);
			}
			pos.y += line_height;
		}
	}
}