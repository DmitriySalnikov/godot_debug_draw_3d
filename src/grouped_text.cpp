#include "grouped_text.h"
#include "debug_draw.h"
#include "math_utils.h"
#include "utils.h"
#include "draw_cache.h"

using namespace godot;

TextGroupItem::TextGroupItem(const double &_expiration_time, const String &_key, const String &_text, const int &_priority, const Color &_color) {
	DEBUG_PRINT_STD("New " TEXT(TextGroupItem) " created: %s : %s\n", _key.utf8().get_data(), _text.utf8().get_data());

	expiration_time = _expiration_time;
	key = _key;
	text = _text;
	priority = _priority;
	value_color = _color;
	second_chance = true;
}

void TextGroupItem::update(const double &_expiration_time, const String &_key, const String &_text, const int &_priority, const Color &_color) {
	expiration_time = _expiration_time;
	key = _key;
	text = _text;
	priority = _priority;
	value_color = _color;
	second_chance = true;
}

bool TextGroupItem::is_expired() {
	if (second_chance) {
		second_chance = false;
		return false;
	}
	return expiration_time > 0;
}

TextGroup::TextGroup(const String &_title, const int &_priority, const bool &_show_title, const Color &_group_color, const int &_title_size, const int &_text_size) {
	DEBUG_PRINT_STD("New " TEXT(TextGroup) " created: %s\n", _title.utf8().get_data());

	title = _title;
	group_priority = _priority;
	show_title = _show_title;
	group_color = _group_color;
	title_size = _title_size;
	text_size = _text_size;
}

void TextGroup::cleanup_texts(const std::function<void()> &_update, const double &_delta) {
	std::unordered_set<TextGroupItem_ptr> keysToRemove;
	keysToRemove.reserve(Texts.size() / 2);

	for (const TextGroupItem_ptr &k : Texts) {
		k->expiration_time -= _delta;
		if (k->is_expired()) {
			keysToRemove.insert(k);
		}
	}

	for (const TextGroupItem_ptr &k : keysToRemove) {
		Texts.erase(k);
	}

	if (keysToRemove.size() > 0 && _update)
		_update();
}

void GroupedText::_create_new_default_groupd_if_needed() {
	LOCK_GUARD(datalock);
	if (!_current_text_group) {
		_current_text_group = std::make_shared<TextGroup>("", 0, false, owner->get_text_foreground_color(), 0, owner->get_text_default_size());
		_text_groups.insert(_current_text_group);
	}
}

void GroupedText::init_group(DebugDraw *p_owner) {
	owner = p_owner;
	_current_text_group = nullptr;
	item_for_title_of_groups = std::make_shared<TextGroupItem>(0.0f, "", "", 0, Colors::empty_color);
	item_for_title_of_groups->is_group_title = true;
}

void GroupedText::clear_text() {
	LOCK_GUARD(datalock);
	_text_groups.clear();
}

void GroupedText::cleanup_text(const double &delta) {
	LOCK_GUARD(datalock);
	// Clean texts
	Utils::remove_where(&_text_groups, [](auto g) { return g->Texts.size() == 0; });

	for (const TextGroup_ptr &g : _text_groups) {
		std::function<void()> upd_txt([this] { owner->mark_canvas_dirty(); });
		g->cleanup_texts(upd_txt, delta);
	}
}

void GroupedText::begin_text_group(const String &_group_title, const int &_group_priority, const Color &_group_color, const bool &_show_title, const int &_title_size, const int &_text_size) {
	LOCK_GUARD(datalock);

	TextGroup_ptr newGroup = nullptr;
	for (const TextGroup_ptr &g : _text_groups) {
		if (g->title == _group_title) {
			newGroup = g;
			break;
		}
	}

	int new_title_size = _title_size > 0 ? _title_size : owner->get_text_default_size();
	int new_text_size = _text_size > 0 ? _text_size : owner->get_text_default_size();

	if (newGroup) {
		newGroup->show_title = _show_title;
		newGroup->group_priority = _group_priority;
		newGroup->group_color = _group_color;
		newGroup->title_size = new_title_size;
		newGroup->text_size = new_text_size;
	} else {
		newGroup = std::make_shared<TextGroup>(_group_title, _group_priority, _show_title, _group_color, new_title_size, new_text_size);
		_text_groups.insert(newGroup);
	}

	_current_text_group = newGroup;
}

void GroupedText::end_text_group() {
	LOCK_GUARD(datalock);

	_current_text_group = nullptr;
	for (const TextGroup_ptr &g : _text_groups) {
		if (g->title == "") {
			_current_text_group = g;
			_current_text_group->group_color = owner->get_text_foreground_color();
			_current_text_group->group_priority = 0;
			_current_text_group->show_title = false;
			_current_text_group->text_size = owner->get_text_default_size();
			break;
		}
	}
}

void GroupedText::set_text(const String &_key, const Variant &_value, const int &_priority, const Color &_color_of_value, const double &_duration) {
	double new_duration = _duration;
	if (new_duration < 0) {
		new_duration = owner->get_text_default_duration();
	}

	String _strVal;
	if (_value.get_type() != Variant::NIL)
		_strVal = _value.stringify();

	bool need_update_canvas = false;

	{
		LOCK_GUARD(datalock);

		_create_new_default_groupd_if_needed();

		TextGroupItem_ptr item;

		for (const auto &d : _current_text_group->Texts) {
			if (d->key == _key) {
				item = d;
				break;
			}
		}

		if (item.get()) {
			if (_strVal != item->text)
				owner->mark_canvas_dirty();

			item->update(new_duration, _key, _strVal, _priority, _color_of_value);
		} else {
			_current_text_group->Texts.insert(std::make_shared<TextGroupItem>(new_duration, _key, _strVal, _priority, _color_of_value));
			owner->mark_canvas_dirty();
		}
	}
}

void GroupedText::draw(CanvasItem *_ci, const Ref<Font> &_font, const Vector2 &_vp_size) {
	LOCK_GUARD(datalock);
	static const String separator = " : ";

	std::vector<DrawCachedRect> backgrounds;
	std::vector<DrawCachedText> text_parts;

	real_t groups_height = 0;
	{
		Ref<Font> draw_font = owner->get_text_custom_font().is_null() ? _font : owner->get_text_custom_font();
		Vector2 pos;
		real_t right_side_multiplier = 0;

		switch (owner->get_text_block_position()) {
			case DebugDraw::BlockPosition::POSITION_RIGHT_TOP:
			case DebugDraw::BlockPosition::POSITION_RIGHT_BOTTOM:
				right_side_multiplier = -1;
				break;
		}

		std::vector<TextGroup_ptr> ordered_groups = Utils::order_by(&_text_groups,
				[](TextGroup_ptr const &a, TextGroup_ptr const &b) { return a->group_priority < b->group_priority; });

		for (const TextGroup_ptr &g : ordered_groups) {
			auto group_items = Utils::order_by(&g->Texts, [](TextGroupItem_ptr const &a, TextGroupItem_ptr const &b) {
				return a->priority < b->priority || (a->priority == b->priority && a->key.naturalnocasecmp_to(b->key) < 0);
			});

			// Add title to the list
			if (g->show_title) {
				item_for_title_of_groups->key = g->title;
				group_items.insert(group_items.begin(), item_for_title_of_groups);
			}

			for (const TextGroupItem_ptr &t : group_items) {
				const String keyText = t->key;
				const bool is_title_only = !t.get() || t->text == "";
				const String text = is_title_only ? keyText : keyText + separator + t->text;

				const int font_size = t->is_group_title ? g->title_size : g->text_size;
				const Vector2 size = draw_font->get_string_size(text, HORIZONTAL_ALIGNMENT_LEFT, -1, font_size);
				const Vector2 text_padding = owner->get_text_padding();
				const Vector2 font_offset = Vector2(0, (real_t)draw_font->get_ascent(font_size)) + text_padding;

				float size_right_revert = (size.x + text_padding.x * 2) * right_side_multiplier;
				backgrounds.push_back(DrawCachedRect(
						Rect2(Vector2(pos.x + size_right_revert, pos.y).floor(), Vector2(size.x + text_padding.x * 2, size.y + text_padding.y * 2).floor()),
						owner->get_text_background_color()));

				// Draw colored string
				if ((t.get() && t->value_color == Colors::empty_color) || is_title_only) {
					// Both parts with same color
					text_parts.push_back(DrawCachedText(text, draw_font, font_size,
							Vector2(pos.x + font_offset.x + size_right_revert, pos.y + font_offset.y).floor(),
							g->group_color));
				} else {
					// Both parts with different colors
					String textSep = keyText + separator;
					int64_t _keyLength = textSep.length();

					text_parts.push_back(DrawCachedText(text.substr(0, _keyLength), draw_font, font_size,
							Vector2(pos.x + font_offset.x + size_right_revert, pos.y + font_offset.y).floor(),
							g->group_color));

					text_parts.push_back(DrawCachedText(text.substr(_keyLength, text.length() - _keyLength), draw_font, font_size,
							Vector2(pos.x + font_offset.x + size_right_revert + draw_font->get_string_size(textSep, HORIZONTAL_ALIGNMENT_LEFT, -1, font_size).x, pos.y + font_offset.y).floor(),
							g->group_color));
				}
				pos.y += size.y + text_padding.y * 2;
			}
		}

		groups_height = pos.y;
	}

	Vector2 text_block_offset = owner->get_text_block_offset();
	Vector2 pos;
	switch (owner->get_text_block_position()) {
		case DebugDraw::BlockPosition::POSITION_LEFT_TOP:
			pos = text_block_offset;
			break;
		case DebugDraw::BlockPosition::POSITION_RIGHT_TOP:
			pos = Vector2(
					_vp_size.x - text_block_offset.x,
					text_block_offset.y);
			break;
		case DebugDraw::BlockPosition::POSITION_LEFT_BOTTOM:
			pos = Vector2(
					text_block_offset.x,
					_vp_size.y - text_block_offset.y - groups_height);
			break;
		case DebugDraw::BlockPosition::POSITION_RIGHT_BOTTOM:
			pos = Vector2(
					_vp_size.x - text_block_offset.x,
					_vp_size.y - text_block_offset.y - groups_height);
			break;
	}

	for (auto &bg : backgrounds) {
		bg.rect.position += pos;
		_ci->draw_rect(bg.rect, bg.color);
	}

	for (auto &tp : text_parts) {
		tp.position += pos;
		_ci->draw_string(tp.font, tp.position, tp.text, godot::HORIZONTAL_ALIGNMENT_LEFT, -1, tp.font_size, tp.color);
	}
}