#include "grouped_text.h"
#include "debug_draw.h"
#include "math_utils.h"
#include "utils.h"

using namespace godot;

TextGroupItem::TextGroupItem(double _expirationTime, const String &key, const String &text, const int &priority, const Color &color) {
	DEBUG_PRINT_STD("New " TEXT(TextGroupItem) " created: %s : %s\n", key.utf8().get_data(), text.utf8().get_data());

	expiration_time = _expirationTime;
	Key = key;
	Text = text;
	Priority = priority;
	ValueColor = color;
	second_chance = true;
}

void TextGroupItem::update(const double &_expirationTime, const String &key, const String &text, const int &priority, const Color &color) {
	expiration_time = _expirationTime;
	Key = key;
	Text = text;
	Priority = priority;
	ValueColor = color;
	second_chance = true;
}

bool TextGroupItem::is_expired() {
	if (second_chance) {
		second_chance = false;
		return false;
	}
	return expiration_time > 0;
}

TextGroup::TextGroup(String _title, int priority, bool showTitle, Color groupColor, int titleSize, int textSize) {
	DEBUG_PRINT_STD("New " TEXT(TextGroup) " created: %s\n", _title.utf8().get_data());

	title = _title;
	group_priority = priority;
	show_title = showTitle;
	group_color = groupColor;
	title_size = titleSize;
	text_size = textSize;
}

void TextGroup::cleanup_texts(std::function<void()> update, double delta) {
	std::unordered_set<TextGroupItem_ptr> keysToRemove;
	keysToRemove.reserve(Texts.size() / 2);

	for (const TextGroupItem_ptr &k : Texts) {
		k->expiration_time -= delta;
		if (k->is_expired()) {
			keysToRemove.insert(k);
		}
	}

	for (const TextGroupItem_ptr &k : keysToRemove) {
		Texts.erase(k);
	}

	if (keysToRemove.size() > 0 && update)
		update();
}

void GroupedText::_create_new_default_groupd_if_needed() {
	LOCK_GUARD(datalock);
	if (!_currentTextGroup) {
		_currentTextGroup = std::make_shared<TextGroup>("", 0, false, owner->get_text_foreground_color(), 0, owner->get_text_default_size());
		_textGroups.insert(_currentTextGroup);
	}
}

void GroupedText::init_group(DebugDraw *p_owner) {
	owner = p_owner;
	_currentTextGroup = nullptr;
	item_for_title_of_groups = std::make_shared<TextGroupItem>(0.0f, "", "", 0, Colors::empty_color);
	item_for_title_of_groups->is_group_title = true;
}

void GroupedText::clear_text() {
	LOCK_GUARD(datalock);
	_textGroups.clear();
}

void GroupedText::cleanup_text(double delta) {
	LOCK_GUARD(datalock);
	// Clean texts
	Utils::remove_where(&_textGroups, [](auto g) { return g->Texts.size() == 0; });

	for (const TextGroup_ptr &g : _textGroups) {
		std::function<void()> upd_txt([this] { owner->mark_canvas_needs_update(); });
		g->cleanup_texts(upd_txt, delta);
	}
}

void GroupedText::begin_text_group(String groupTitle, int groupPriority, Color groupColor, bool showTitle, int titleSize, int textSize) {
	LOCK_GUARD(datalock);

	TextGroup_ptr newGroup = nullptr;
	for (const TextGroup_ptr &g : _textGroups) {
		if (g->title == groupTitle) {
			newGroup = g;
			break;
		}
	}

	if (newGroup) {
		newGroup->show_title = showTitle;
		newGroup->group_priority = groupPriority;
		newGroup->group_color = groupColor;
		newGroup->title_size = titleSize;
		newGroup->text_size= textSize;
	} else {
		newGroup = std::make_shared<TextGroup>(groupTitle, groupPriority, showTitle, groupColor, titleSize, textSize);
		_textGroups.insert(newGroup);
	}

	_currentTextGroup = newGroup;
}

void GroupedText::end_text_group() {
	LOCK_GUARD(datalock);

	_currentTextGroup = nullptr;
	for (const TextGroup_ptr &g : _textGroups) {
		if (g->title == "") {
			_currentTextGroup = g;
			_currentTextGroup->group_color = owner->get_text_foreground_color();
			_currentTextGroup->group_priority = 0;
			_currentTextGroup->show_title = false;
			break;
		}
	}
}

void GroupedText::set_text(String &key, Variant &value, int &priority, Color &colorOfValue, double duration) {
	if (duration < 0) {
		duration = owner->get_text_default_duration();
	}

	String _strVal;
	if (value.get_type() != Variant::NIL)
		_strVal = value.stringify();

	bool need_update_canvas = false;

	{
		LOCK_GUARD(datalock);

		_create_new_default_groupd_if_needed();

		TextGroupItem_ptr item;

		for (const auto &d : _currentTextGroup->Texts) {
			if (d->Key == key) {
				item = d;
				break;
			}
		}

		if (item.get()) {
			if (_strVal != item->Text)
				owner->mark_canvas_needs_update();

			item->update(duration, key, _strVal, priority, colorOfValue);
		} else {
			_currentTextGroup->Texts.insert(std::make_shared<TextGroupItem>(duration, key, _strVal, priority, colorOfValue));
			owner->mark_canvas_needs_update();
		}
	}
}

void GroupedText::draw(CanvasItem *ci, Ref<Font> _font, Vector2 vp_size) {
	LOCK_GUARD(datalock);
	int count = Utils::sum(&_textGroups, [](TextGroup_ptr g) { return (int)g->Texts.size() + (g->show_title ? 1 : 0); });

	static const String separator = " : ";

	Ref<Font> draw_font = owner->get_text_custom_font().is_null() ? _font : owner->get_text_custom_font();

	Vector2 pos = Vector2_ZERO;
	real_t size_mul = 0;

	// TODO bottom position is broken
	// TODO add buffering to accumulate first and then draw

	Vector2 text_block_offset = owner->get_text_block_offset();
	switch (owner->get_text_block_position()) {
		case DebugDraw::BlockPosition::POSITION_LEFT_TOP:
			pos = text_block_offset;
			size_mul = 0;
			break;
		case DebugDraw::BlockPosition::POSITION_RIGHT_TOP:
			pos = Vector2(
					vp_size.x - text_block_offset.x,
					text_block_offset.y);
			size_mul = -1;
			break;
		case DebugDraw::BlockPosition::POSITION_LEFT_BOTTOM:
			pos = Vector2(
					text_block_offset.x,
					vp_size.y - text_block_offset.y);
			// vp_size.y - text_block_offset.y - line_height * count);
			size_mul = 0;
			break;
		case DebugDraw::BlockPosition::POSITION_RIGHT_BOTTOM:
			pos = Vector2(
					vp_size.x - text_block_offset.x,
					vp_size.y - text_block_offset.y);
			// vp_size.y - text_block_offset.y - line_height * count);
			size_mul = -1;
			break;
	}

	std::vector<TextGroup_ptr> ordered_groups = Utils::order_by(&_textGroups,
			[](TextGroup_ptr const &a, TextGroup_ptr const &b) { return a->group_priority < b->group_priority; });

	for (const TextGroup_ptr &g : ordered_groups) {
		auto group_items = Utils::order_by(&g->Texts, [](TextGroupItem_ptr const &a, TextGroupItem_ptr const &b) {
			return a->Priority < b->Priority || (a->Priority == b->Priority && a->Key.naturalnocasecmp_to(b->Key) < 0);
		});

		// Add title to the list
		if (g->show_title) {
			item_for_title_of_groups->Key = g->title;
			group_items.insert(group_items.begin(), item_for_title_of_groups);
		}

		for (const TextGroupItem_ptr &t : group_items) {
			String keyText = t->Key;
			bool is_title_only = !t.get() || t->Text == "";
			String text = is_title_only ? keyText : keyText + separator + t->Text;

			int font_size = t->is_group_title ? g->title_size : g->text_size;
			Vector2 size = draw_font->get_string_size(text, HORIZONTAL_ALIGNMENT_LEFT, -1, font_size);
			Vector2 ascent = Vector2(0, (real_t)draw_font->get_ascent(font_size));
			Vector2 text_padding = owner->get_text_padding();
			Vector2 font_offset = ascent + text_padding;
			real_t line_height = (real_t)draw_font->get_height(font_size) + text_padding.y * 2;

			float size_right_revert = (size.x + text_padding.x * 2) * size_mul;
			ci->draw_rect(
					Rect2(Vector2(pos.x + size_right_revert, pos.y).floor(),
							Vector2(size.x + text_padding.x * 2, line_height).floor()),
					owner->get_text_background_color());

			// Draw colored string
			if ((t.get() && t->ValueColor == Colors::empty_color) || is_title_only) {
				// Both parts with same color
				ci->draw_string(draw_font,
						Vector2(pos.x + font_offset.x + size_right_revert, pos.y + font_offset.y).floor(),
						text, godot::HORIZONTAL_ALIGNMENT_CENTER, -1, font_size, g->group_color);
			} else {
				// Both parts with different colors
				String textSep = keyText + separator;
				int64_t _keyLength = textSep.length();
				ci->draw_string(draw_font,
						Vector2(pos.x + font_offset.x + size_right_revert, pos.y + font_offset.y).floor(),
						text.substr(0, _keyLength), godot::HORIZONTAL_ALIGNMENT_CENTER, -1, font_size, g->group_color);
				ci->draw_string(draw_font,
						Vector2(pos.x + font_offset.x + size_right_revert + draw_font->get_string_size(textSep, HORIZONTAL_ALIGNMENT_LEFT, -1, font_size).x, pos.y + font_offset.y).floor(),
						text.substr(_keyLength, text.length() - _keyLength), godot::HORIZONTAL_ALIGNMENT_CENTER, -1, font_size, t->ValueColor);
			}
			pos.y += line_height;
		}
	}
}