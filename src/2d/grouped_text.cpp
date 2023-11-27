#include "grouped_text.h"
#include "config_2d.h"
#include "debug_draw_2d.h"
#include "utils/utils.h"

using namespace godot;

#ifndef DISABLE_DEBUG_RENDERING
TextGroupItem::TextGroupItem(const double &_expiration_time, const String &_key, const String &_text, const int &_priority, const Color &_color) {
	DEV_PRINT_STD("New " NAMEOF(TextGroupItem) " created: %s : %s\n", _key.utf8().get_data(), _text.utf8().get_data());

	expiration_time = _expiration_time;
	key = _key;
	text = _text;
	priority = _priority;
	value_color = _color;
	second_chance = true;
}

bool TextGroupItem::update(const double &_expiration_time, const String &_key, const String &_text, const int &_priority, const Color &_color) {
	bool dirty = expiration_time != _expiration_time || key != _key || text != _text || priority != _priority || value_color != _color;

	expiration_time = _expiration_time;
	key = _key;
	text = _text;
	priority = _priority;
	value_color = _color;
	second_chance = true;

	return dirty;
}

bool TextGroupItem::is_expired() {
	return expiration_time > 0 ? false : !second_chance;
}

void TextGroup::set_group_priority(int _val) {
	if (group_priority != _val)
		owner->mark_canvas_dirty();
	group_priority = _val;
}

int TextGroup::get_group_priority() {
	return group_priority;
}

void TextGroup::set_show_title(bool _val) {
	if (show_title != _val)
		owner->mark_canvas_dirty();
	show_title = _val;
}

bool TextGroup::is_show_title() {
	return show_title;
}

void TextGroup::set_group_color(Color _val) {
	if (group_color != _val)
		owner->mark_canvas_dirty();
	group_color = _val;
}

Color TextGroup::get_group_color() {
	return group_color;
}

void TextGroup::set_title_size(int _val) {
	if (title_size != _val)
		owner->mark_canvas_dirty();
	title_size = _val;
}

int TextGroup::get_title_size() {
	return title_size;
}

void TextGroup::set_text_size(int _val) {
	if (text_size != _val)
		owner->mark_canvas_dirty();
	text_size = _val;
}

int TextGroup::get_text_size() {
	return text_size;
}

TextGroup::TextGroup(DebugDraw2D *_owner, const String &_title, const int &_priority, const bool &_show_title, const Color &_group_color, const int &_title_size, const int &_text_size) {
	DEV_PRINT_STD("New " NAMEOF(TextGroup) " created: %s\n", _title.utf8().get_data());

	owner = _owner;
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
		} else {
			k->second_chance = false;
		}
	}

	for (const TextGroupItem_ptr &k : keysToRemove) {
		Texts.erase(k);
	}

	if (keysToRemove.size() > 0 && _update)
		_update();
}

void GroupedText::_create_new_default_group_if_needed() {
	LOCK_GUARD(datalock);
	if (!_current_text_group) {
		_current_text_group = std::make_shared<TextGroup>(owner, "", 0, false, owner->get_config()->get_text_foreground_color(), 0, owner->get_config()->get_text_default_size());
		_text_groups.insert(_current_text_group);
	}
}

void GroupedText::init_group(DebugDraw2D *p_owner) {
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
	size_t old_size = _text_groups.size();
	Utils::remove_where(_text_groups, [](auto g) {
		bool prev_used = g->is_used_one_time;
		g->is_used_one_time = true;
		return g->Texts.size() == 0 && prev_used;
	});

	if (old_size != _text_groups.size()) {
		owner->mark_canvas_dirty();
	}

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

	int new_title_size = _title_size > 0 ? _title_size : owner->get_config()->get_text_default_size();
	int new_text_size = _text_size > 0 ? _text_size : owner->get_config()->get_text_default_size();

	if (newGroup) {
		newGroup->set_show_title(_show_title);
		newGroup->set_group_priority(_group_priority);
		newGroup->set_group_color(_group_color);
		newGroup->set_title_size(new_title_size);
		newGroup->set_text_size(new_text_size);
		newGroup->is_used_one_time = false;
	} else {
		newGroup = std::make_shared<TextGroup>(owner, _group_title, _group_priority, _show_title, _group_color, new_title_size, new_text_size);
		_text_groups.insert(newGroup);
		owner->mark_canvas_dirty();
	}

	_current_text_group = newGroup;
}

void GroupedText::end_text_group() {
	LOCK_GUARD(datalock);

	_current_text_group = nullptr;
	for (const TextGroup_ptr &g : _text_groups) {
		if (g->title == "") {
			_current_text_group = g;
			_current_text_group->set_show_title(false);
			_current_text_group->set_group_priority(0);
			_current_text_group->set_group_color(owner->get_config()->get_text_foreground_color());
			_current_text_group->set_title_size(owner->get_config()->get_text_default_size());
			_current_text_group->set_text_size(owner->get_config()->get_text_default_size());
			break;
		}
	}
}

void GroupedText::set_text(const String &_key, const Variant &_value, const int &_priority, const Color &_color_of_value, const double &_duration) {
	double new_duration = _duration;
	if (new_duration < 0) {
		new_duration = owner->get_config()->get_text_default_duration();
	}

	String _strVal;
	if (_value.get_type() != Variant::NIL)
		_strVal = _value.stringify();

	bool need_update_canvas = false;

	{
		LOCK_GUARD(datalock);

		_create_new_default_group_if_needed();

		TextGroupItem_ptr item;

		for (const auto &d : _current_text_group->Texts) {
			if (d->key == _key) {
				item = d;
				break;
			}
		}

		if (item.get()) {
			if (item->update(new_duration, _key, _strVal, _priority, _color_of_value))
				owner->mark_canvas_dirty();
		} else {
			_current_text_group->Texts.insert(std::make_shared<TextGroupItem>(new_duration, _key, _strVal, _priority, _color_of_value));
			owner->mark_canvas_dirty();
		}
	}
}

void GroupedText::draw(CanvasItem *_ci, const Ref<Font> &_font, const Vector2 &_vp_size) {
	LOCK_GUARD(datalock);
	static const String separator = " : ";

	std::vector<DrawRectInstance> backgrounds;
	std::vector<DrawTextInstance> text_parts;

	real_t groups_height = 0;
	{
		Ref<Font> draw_font = owner->get_config()->get_text_custom_font().is_null() ? _font : owner->get_config()->get_text_custom_font();
		Vector2 pos;
		real_t right_side_multiplier = 0;

		switch (owner->get_config()->get_text_block_position()) {
			case DebugDrawConfig2D::BlockPosition::POSITION_RIGHT_TOP:
			case DebugDrawConfig2D::BlockPosition::POSITION_RIGHT_BOTTOM:
				right_side_multiplier = -1;
				break;
			default:
				break;
		}

		std::vector<TextGroup_ptr> ordered_groups = Utils::order_by(_text_groups,
				[](TextGroup_ptr const &a, TextGroup_ptr const &b) { return a->get_group_priority() < b->get_group_priority(); });

		for (const TextGroup_ptr &g : ordered_groups) {
			auto group_items = Utils::order_by(g->Texts, [](TextGroupItem_ptr const &a, TextGroupItem_ptr const &b) {
				return a->priority < b->priority || (a->priority == b->priority && a->key.naturalnocasecmp_to(b->key) < 0);
			});

			// Add title to the list
			if (g->is_show_title()) {
				item_for_title_of_groups->key = g->title;
				group_items.insert(group_items.begin(), item_for_title_of_groups);
			}

			for (const TextGroupItem_ptr &t : group_items) {
				const String keyText = t->key;
				const bool is_title_only = !t.get() || t->text == "";
				const String text = is_title_only ? keyText : keyText + separator + t->text;

				const int font_size = t->is_group_title ? g->get_title_size() : g->get_text_size();
				const Vector2 size = draw_font->get_string_size(text, HORIZONTAL_ALIGNMENT_LEFT, -1, font_size);
				const Vector2 text_padding = owner->get_config()->get_text_padding();
				const Vector2 font_offset = Vector2(0, (real_t)draw_font->get_ascent(font_size)) + text_padding;

				float size_right_revert = (size.x + text_padding.x * 2) * right_side_multiplier;
				backgrounds.push_back(DrawRectInstance(
						Rect2(Vector2(pos.x + size_right_revert, pos.y).floor(), Vector2(size.x + text_padding.x * 2, size.y + text_padding.y * 2).floor()),
						owner->get_config()->get_text_background_color()));

				// Draw colored string
				if ((t.get() && t->value_color == Colors::empty_color) || is_title_only) {
					// Both parts with same color
					text_parts.push_back(DrawTextInstance(text, draw_font, font_size,
							Vector2(pos.x + font_offset.x + size_right_revert, pos.y + font_offset.y).floor(),
							g->get_group_color()));
				} else {
					// Both parts with different colors
					String textSep = keyText + separator;
					int64_t _keyLength = textSep.length();

					text_parts.push_back(DrawTextInstance(text.substr(0, _keyLength), draw_font, font_size,
							Vector2(pos.x + font_offset.x + size_right_revert, pos.y + font_offset.y).floor(),
							g->get_group_color()));

					text_parts.push_back(DrawTextInstance(text.substr(_keyLength, text.length() - _keyLength), draw_font, font_size,
							Vector2(pos.x + font_offset.x + size_right_revert + draw_font->get_string_size(textSep, HORIZONTAL_ALIGNMENT_LEFT, -1, font_size).x, pos.y + font_offset.y).floor(),
							t->value_color));
				}
				pos.y += size.y + text_padding.y * 2;
			}
		}

		groups_height = pos.y;
	}

	Vector2 text_block_offset = owner->get_config()->get_text_block_offset();
	Vector2 pos;
	switch (owner->get_config()->get_text_block_position()) {
		case DebugDrawConfig2D::BlockPosition::POSITION_LEFT_TOP:
			pos = text_block_offset;
			break;
		case DebugDrawConfig2D::BlockPosition::POSITION_RIGHT_TOP:
			pos = Vector2(
					_vp_size.x - text_block_offset.x,
					text_block_offset.y);
			break;
		case DebugDrawConfig2D::BlockPosition::POSITION_LEFT_BOTTOM:
			pos = Vector2(
					text_block_offset.x,
					_vp_size.y - text_block_offset.y - groups_height);
			break;
		case DebugDrawConfig2D::BlockPosition::POSITION_RIGHT_BOTTOM:
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

size_t GroupedText::get_text_group_count() {
	LOCK_GUARD(datalock);
	return _text_groups.size();
}

size_t GroupedText::get_text_line_total_count() {
	LOCK_GUARD(datalock);
	size_t total = 0;
	for (const auto &g : _text_groups) {
		total += g->Texts.size();
		if (g->is_show_title()) {
			total++;
		}
	}
	return total;
}

#endif
