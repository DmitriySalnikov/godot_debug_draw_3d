#include "grouped_text.h"
#include "config_2d.h"
#include "debug_draw_2d.h"
#include "utils/utils.h"

using namespace godot;

#ifndef DISABLE_DEBUG_RENDERING
TextGroupItem::TextGroupItem(const double &p_expiration_time, const String &p_key, const String &p_text, const int &p_priority, const Color &p_color) {
	DEV_PRINT_STD("New %s created: %s : %s\n", NAMEOF(TextGroupItem), p_key.utf8().get_data(), p_text.utf8().get_data());

	expiration_time = p_expiration_time;
	key = p_key;
	text = p_text;
	priority = p_priority;
	value_color = p_color;
	second_chance = true;
}

bool TextGroupItem::update(const double &p_expiration_time, const String &p_key, const String &p_text, const int &p_priority, const Color &p_color) {
	bool dirty = expiration_time != p_expiration_time || key != p_key || text != p_text || priority != p_priority || value_color != p_color;

	expiration_time = p_expiration_time;
	key = p_key;
	text = p_text;
	priority = p_priority;
	value_color = p_color;
	second_chance = true;

	return dirty;
}

bool TextGroupItem::is_expired() {
	return expiration_time > 0 ? false : !second_chance;
}

void TextGroup::set_group_priority(int p_val) {
	if (group_priority != p_val)
		owner->mark_canvas_dirty();
	group_priority = p_val;
}

int TextGroup::get_group_priority() {
	return group_priority;
}

void TextGroup::set_show_title(bool p_val) {
	if (show_title != p_val)
		owner->mark_canvas_dirty();
	show_title = p_val;
}

bool TextGroup::is_show_title() {
	return show_title;
}

void TextGroup::set_group_color(Color p_val) {
	if (group_color != p_val)
		owner->mark_canvas_dirty();
	group_color = p_val;
}

Color TextGroup::get_group_color() {
	return group_color;
}

void TextGroup::set_title_size(int p_val) {
	if (title_size != p_val)
		owner->mark_canvas_dirty();
	title_size = p_val;
}

int TextGroup::get_title_size() {
	return title_size;
}

void TextGroup::set_text_size(int p_val) {
	if (text_size != p_val)
		owner->mark_canvas_dirty();
	text_size = p_val;
}

int TextGroup::get_text_size() {
	return text_size;
}

TextGroup::TextGroup(DebugDraw2D *p_owner, const String &p_title, const int &p_priority, const bool &p_show_title, const Color &p_group_color, const int &p_title_size, const int &p_text_size) {
	DEV_PRINT_STD("New %s created: %s\n", NAMEOF(TextGroup), p_title.utf8().get_data());

	owner = p_owner;
	title = p_title;
	group_priority = p_priority;
	show_title = p_show_title;
	group_color = p_group_color;
	title_size = p_title_size;
	text_size = p_text_size;
}

void TextGroup::cleanup_texts(const std::function<void()> &p_update, const double &p_delta) {
	ZoneScoped;
	size_t old_size = Texts.size();

	Texts.erase(std::remove_if(Texts.begin(), Texts.end(),
						[&p_delta](auto &t) {
							t->expiration_time -= p_delta;
							if (t->is_expired()) {
								return true;
							} else {
								t->second_chance = false;
							}
							return false;
						}),
			Texts.end());

	if (old_size != Texts.size() && p_update)
		p_update();
}

void GroupedText::_create_new_default_group_if_needed() {
	ZoneScoped;
	LOCK_GUARD(datalock);
	if (!_current_text_group) {
		_current_text_group = std::make_shared<TextGroup>(owner, "", 0, false, owner->get_config()->get_text_foreground_color(), 0, owner->get_config()->get_text_default_size());
		_text_groups.push_back(_current_text_group);
	}
}

void GroupedText::init_group(DebugDraw2D *p_owner) {
	owner = p_owner;
	_current_text_group = nullptr;
	item_for_title_of_groups = std::make_shared<TextGroupItem>(0.0f, "", "", 0, Colors::empty_color);
	item_for_title_of_groups->is_group_title = true;
}

void GroupedText::clear_groups() {
	LOCK_GUARD(datalock);
	_text_groups.clear();
}

void GroupedText::cleanup_text(const double &p_delta) {
	ZoneScoped;
	LOCK_GUARD(datalock);

	size_t old_size = _text_groups.size();
	_text_groups.erase(std::remove_if(_text_groups.begin(), _text_groups.end(),
							   [](const auto &g) {
								   bool prev_used = g->is_used_one_time;
								   g->is_used_one_time = true;
								   return g->Texts.size() == 0 && prev_used;
							   }),
			_text_groups.end());

	if (old_size != _text_groups.size()) {
		owner->mark_canvas_dirty();
	}

	for (const TextGroup_ptr &g : _text_groups) {
		std::function<void()> upd_txt([this] { owner->mark_canvas_dirty(); });
		g->cleanup_texts(upd_txt, p_delta);
	}
}

void GroupedText::begin_text_group(const String &p_group_title, const int &p_group_priority, const Color &p_group_color, const bool &p_show_title, const int &p_title_size, const int &p_text_size) {
	ZoneScoped;
	LOCK_GUARD(datalock);

	TextGroup_ptr newGroup = nullptr;
	for (const TextGroup_ptr &g : _text_groups) {
		if (g->title == p_group_title) {
			newGroup = g;
			break;
		}
	}

	int new_title_size = p_title_size > 0 ? p_title_size : owner->get_config()->get_text_default_size();
	int new_text_size = p_text_size > 0 ? p_text_size : owner->get_config()->get_text_default_size();

	if (newGroup) {
		newGroup->set_show_title(p_show_title);
		newGroup->set_group_priority(p_group_priority);
		newGroup->set_group_color(p_group_color);
		newGroup->set_title_size(new_title_size);
		newGroup->set_text_size(new_text_size);
		newGroup->is_used_one_time = false;
	} else {
		newGroup = std::make_shared<TextGroup>(owner, p_group_title, p_group_priority, p_show_title, p_group_color, new_title_size, new_text_size);
		_text_groups.push_back(newGroup);
		owner->mark_canvas_dirty();
	}

	_current_text_group = newGroup;
}

void GroupedText::end_text_group() {
	ZoneScoped;
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

void GroupedText::set_text(const String &p_key, const Variant &p_value, const int &p_priority, const Color &p_color_of_value, const double &p_duration) {
	ZoneScoped;
	double new_duration = p_duration;
	if (new_duration < 0) {
		new_duration = owner->get_config()->get_text_default_duration();
	}

	String _strVal;
	{
		ZoneScopedN("stringify");
		if (p_value.get_type() != Variant::NIL)
			_strVal = p_value.stringify();
	}

	{
		LOCK_GUARD(datalock);

		_create_new_default_group_if_needed();

		TextGroupItem_ptr item;

		for (const auto &d : _current_text_group->Texts) {
			if (d->key == p_key) {
				item = d;
				break;
			}
		}

		if (item.get()) {
			if (item->update(new_duration, p_key, _strVal, p_priority, p_color_of_value))
				owner->mark_canvas_dirty();
		} else {
			_current_text_group->Texts.push_back(std::make_shared<TextGroupItem>(new_duration, p_key, _strVal, p_priority, p_color_of_value));
			owner->mark_canvas_dirty();
		}
	}
}

void GroupedText::draw(CanvasItem *p_ci, const Ref<Font> &p_font, const Vector2 &p_vp_size) {
	ZoneScoped;
	LOCK_GUARD(datalock);
	static const String separator = " : ";

	std::vector<DrawRectInstance> backgrounds;
	std::vector<DrawTextInstance> text_parts;

	real_t groups_height = 0;
	{
		Ref<Font> draw_font = owner->get_config()->get_text_custom_font().is_null() ? p_font : owner->get_config()->get_text_custom_font();
		Vector2 pos;
		real_t right_side_multiplier = 0;

		switch (owner->get_config()->get_text_block_position()) {
			case DebugDraw2DConfig::BlockPosition::POSITION_RIGHT_TOP:
			case DebugDraw2DConfig::BlockPosition::POSITION_RIGHT_BOTTOM:
				right_side_multiplier = -1;
				break;
			default:
				break;
		}

		std::sort(_text_groups.begin(), _text_groups.end(),
				[](TextGroup_ptr const &a, TextGroup_ptr const &b) { return a->get_group_priority() < b->get_group_priority(); });

		for (const TextGroup_ptr &g : _text_groups) {
			std::vector<TextGroupItem_ptr> group_items(g->Texts.begin(), g->Texts.end());

			std::sort(group_items.begin(), group_items.end(), [](TextGroupItem_ptr const &a, TextGroupItem_ptr const &b) {
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

				real_t size_right_revert = (size.x + text_padding.x * 2) * right_side_multiplier;
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
		case DebugDraw2DConfig::BlockPosition::POSITION_LEFT_TOP:
			pos = text_block_offset;
			break;
		case DebugDraw2DConfig::BlockPosition::POSITION_RIGHT_TOP:
			pos = Vector2(
					p_vp_size.x - text_block_offset.x,
					text_block_offset.y);
			break;
		case DebugDraw2DConfig::BlockPosition::POSITION_LEFT_BOTTOM:
			pos = Vector2(
					text_block_offset.x,
					p_vp_size.y - text_block_offset.y - groups_height);
			break;
		case DebugDraw2DConfig::BlockPosition::POSITION_RIGHT_BOTTOM:
			pos = Vector2(
					p_vp_size.x - text_block_offset.x,
					p_vp_size.y - text_block_offset.y - groups_height);
			break;
	}

	for (auto &bg : backgrounds) {
		bg.rect.position += pos;
		p_ci->draw_rect(bg.rect, bg.color);
	}

	for (auto &tp : text_parts) {
		tp.position += pos;
		p_ci->draw_string(tp.font, tp.position, tp.text, godot::HORIZONTAL_ALIGNMENT_LEFT, -1, tp.font_size, tp.color);
	}
}

size_t GroupedText::get_text_group_count() {
	LOCK_GUARD(datalock);
	return _text_groups.size();
}

size_t GroupedText::get_text_line_total_count() {
	ZoneScoped;
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
