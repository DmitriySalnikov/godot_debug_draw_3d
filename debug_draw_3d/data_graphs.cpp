#include "data_graphs.h"
#include "debug_draw.h"
#include "math_utils.h"
#include "utils.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GraphParameters::_bind_methods() {
#define REG_PROP_BASE(name, type, getter)                                                    \
	ClassDB::bind_method(D_METHOD(TEXT(set_##name), "value"), &GraphParameters::set_##name); \
	ClassDB::bind_method(D_METHOD(TEXT(getter##name)), &GraphParameters::getter##name);      \
	ADD_PROPERTY(PropertyInfo(type, #name), TEXT(set_##name), TEXT(getter##name));
#define REG_PROP(name, type) REG_PROP_BASE(name, type, get_)
#define REG_PROP_BOOL(name) REG_PROP_BASE(name, Variant::BOOL, is_)

	REG_PROP_BOOL(enabled);
	REG_PROP_BOOL(show_title);
	REG_PROP_BOOL(frame_time_mode);
	REG_PROP_BOOL(centered_graph_line);
	REG_PROP(show_text_flags, Variant::INT);
	REG_PROP(size, Variant::VECTOR2);
	REG_PROP(buffer_size, Variant::INT);
	REG_PROP(offset, Variant::VECTOR2);
	REG_PROP(position, Variant::INT);
	REG_PROP(line_color, Variant::COLOR);
	REG_PROP(text_color, Variant::COLOR);
	REG_PROP(background_color, Variant::COLOR);
	REG_PROP(border_color, Variant::COLOR);
	REG_PROP(text_suffix, Variant::STRING);

	/* TODO pointers is not available..
	REG_PROP(custom_font, Variant::OBJECT);

#undef REG_PROP
#undef REG_PROP_BOOL
#undef REG_PROP_BASE
	*/
}

GraphParameters::GraphParameters() {
	if (IS_EDITOR_HINT()) {
		position = BlockPosition::LeftTop;
	}
}

void GraphParameters::set_enabled(bool _state) {
	enabled = _state;
}

bool GraphParameters::is_enabled() {
	return enabled;
}

void GraphParameters::set_show_title(bool _state) {
	show_title = _state;
}

bool GraphParameters::is_show_title() {
	return show_title;
}

void GraphParameters::set_frame_time_mode(bool _state) {
	frametime_mode = _state;
}

bool GraphParameters::is_frame_time_mode() {
	return frametime_mode;
}

void GraphParameters::set_centered_graph_line(bool _state) {
	centered_graph_line = _state;
}

bool GraphParameters::is_centered_graph_line() {
	return centered_graph_line;
}

void GraphParameters::set_show_text_flags(int _flags) {
	show_text_flags = (GraphTextFlags)_flags;
}

int GraphParameters::get_show_text_flags() {
	return show_text_flags;
}

void GraphParameters::set_size(Vector2 _size) {
	size = _size;
}

Vector2 GraphParameters::get_size() {
	return size;
}

void GraphParameters::set_buffer_size(int _buf_size) {
	buffer_size = Math::clamp(_buf_size, 1, INT32_MAX);
}

int GraphParameters::get_buffer_size() {
	return buffer_size;
}

void GraphParameters::set_offset(Vector2 _offset) {
	offset = _offset;
}

Vector2 GraphParameters::get_offset() {
	return offset;
}

void GraphParameters::set_position(int _position) {
	position = (BlockPosition)_position;
}

int GraphParameters::get_position() {
	return position;
}

void GraphParameters::set_line_color(Color _new_color) {
	line_color = _new_color;
}

Color GraphParameters::get_line_color() {
	return line_color;
}

void GraphParameters::set_text_color(Color _new_color) {
	text_color = _new_color;
}

Color GraphParameters::get_text_color() {
	return text_color;
}

void GraphParameters::set_background_color(Color _new_color) {
	background_color = _new_color;
}

Color GraphParameters::get_background_color() {
	return background_color;
}

void GraphParameters::set_border_color(Color _new_color) {
	border_color = _new_color;
}

Color GraphParameters::get_border_color() {
	return border_color;
}

void GraphParameters::set_text_suffix(String _suffix) {
	text_suffix = _suffix;
}

String GraphParameters::get_text_suffix() {
	return text_suffix;
}

void GraphParameters::set_custom_font(Ref<Font> _custom_font) {
	custom_font = _custom_font;
}

Ref<Font> GraphParameters::get_custom_font() {
	return custom_font;
}

////////////////////////////////////
// DataGraph

DataGraph::DataGraph(Ref<GraphParameters> _owner) :
		config(_owner),
		data(std::make_shared<CircularBuffer<real_t> >(_owner->get_buffer_size())),
		type(Type::Custom) {
}

DataGraph::Type DataGraph::get_type() {
	return type;
}

Ref<GraphParameters> DataGraph::get_config() {
	return config;
}

void DataGraph::update(real_t value) {
	LOCK_GUARD(datalock);

	if (config->get_buffer_size() != data->buffer_size())
		data = std::make_shared<CircularBuffer<real_t> >(config->get_buffer_size());

	_update_added(value);
}

void DataGraph::_update_added(real_t value) {
	data->add(value);
}

Vector2 DataGraph::draw(CanvasItem *ci, Ref<Font> font, Vector2 vp_size, String title, Vector2 base_offset) {
	if (!config->is_enabled())
		return base_offset;

	LOCK_GUARD(datalock);

	Ref<Font> draw_font = config->get_custom_font().is_null() ? font : config->get_custom_font();

	real_t min, max, avg;
	data->get_min_max_avg(&min, &max, &avg);

	// Truncate for pixel perfect render
	Vector2 graphSize(Vector2((real_t)(int)config->get_size().x, (real_t)(int)config->get_size().y));
	Vector2 graphOffset(Vector2((real_t)(int)config->get_offset().x, (real_t)(int)config->get_offset().y));
	Vector2 pos = graphOffset;
	Vector2 title_size = draw_font->get_string_size(title);

	switch (config->get_position()) {
		case BlockPosition::LeftTop:
			pos.y += base_offset.y;
			pos.x += base_offset.x;
			break;
		case BlockPosition::RightTop:
			pos = Vector2(vp_size.x - graphSize.x - graphOffset.x + 1, graphOffset.y + base_offset.y);
			pos.x -= base_offset.x;
			break;
		case BlockPosition::LeftBottom:
			pos = Vector2(graphOffset.x, base_offset.y - graphSize.y - graphOffset.y - (config->is_show_title() ? title_size.y - 3 : 0));
			pos.x += base_offset.x;
			break;
		case BlockPosition::RightBottom:
			pos = Vector2(vp_size.x - graphSize.x - graphOffset.x + 1, base_offset.y - graphSize.y - graphOffset.y - (config->is_show_title() ? title_size.y - 3 : 0));
			pos.x -= base_offset.x;
			break;
	}

	if (config->is_show_title()) {
		Vector2 title_pos = pos;

		switch (config->get_position()) {
			case BlockPosition::RightTop:
			case BlockPosition::RightBottom:
				title_pos.x = title_pos.x + graphSize.x - title_size.x - 8;
				break;
		}

		real_t max_height = draw_font->get_ascent();
		Rect2 border_size(title_pos + Vector2(0, -4), title_size + Vector2(8, 0));
		// Draw background
		ci->draw_rect(border_size, config->get_background_color(), true);
		ci->draw_string(draw_font, (title_pos + Vector2(4, max_height - 3)).floor(), title, godot::HORIZONTAL_ALIGNMENT_LEFT, -1, 16, config->get_text_color()); // TODO font size must be in cofig, not in font

		pos += Vector2(0, max_height);
	}

	real_t height_multiplier = graphSize.y / max;
	real_t center_offset = config->is_centered_graph_line() ? (graphSize.y - height_multiplier * (max - min)) * 0.5f : 0;

	Rect2 border_size(pos + Vector2_UP, graphSize + Vector2_DOWN);

	// Draw background
	ci->draw_rect(border_size, config->get_background_color(), true);

	// Draw graph line
	if (data->is_filled() || data->size() > 2) {
		PackedVector2Array line_points;

		const int offset = (int)data->is_filled();
		double points_interval = (double)config->get_size().x / ((int64_t)config->get_buffer_size() - 1 - offset);

		line_points.resize((int)data->size() - offset);
		{
			auto w = line_points.ptrw();
			for (size_t i = 0; i < data->size() - offset; i++) {
				w[(int)i] = pos + Vector2((real_t)(i * points_interval), graphSize.y - data->get(i) * height_multiplier + center_offset);
			}
		}
		ci->draw_polyline(line_points, config->get_line_color());
	}

	// Draw border
	ci->draw_rect(border_size, config->get_border_color(), false);

	// Draw text
	auto suffix = config->get_text_suffix().is_empty() ? "" : config->get_text_suffix().utf8().get_data();

	if ((config->get_show_text_flags() & GraphTextFlags::Max) == GraphTextFlags::Max) {
		String max_text = Utils::string_format("max: %.2f %s", max, suffix);
		real_t max_height = draw_font->get_height();
		Vector2 text_pos = pos + Vector2(4, max_height - 1);
		ci->draw_string(draw_font, text_pos.floor(), max_text, godot::HORIZONTAL_ALIGNMENT_LEFT, -1, 16, config->get_text_color()); // TODO font size must be in cofig, not in font
	}

	if ((config->get_show_text_flags() & GraphTextFlags::Avarage) == GraphTextFlags::Avarage) {
		String avg_text = Utils::string_format("avg: %.2f %s", avg, suffix);
		real_t avg_height = draw_font->get_height();
		Vector2 text_pos = pos + Vector2(4, (graphSize.y * 0.5f + avg_height * 0.5f - 2));
		ci->draw_string(draw_font, text_pos.floor(), avg_text, godot::HORIZONTAL_ALIGNMENT_LEFT, -1, 16, config->get_text_color()); // TODO font size must be in cofig, not in font
	}

	if ((config->get_show_text_flags() & GraphTextFlags::Min) == GraphTextFlags::Min) {
		String min_text = Utils::string_format("min: %.2f %s", min, suffix);
		Vector2 text_pos = pos + Vector2(4, graphSize.y - 3);
		ci->draw_string(draw_font, text_pos.floor(), min_text, godot::HORIZONTAL_ALIGNMENT_LEFT, -1, 16, config->get_text_color()); // TODO font size must be in cofig, not in font
	}

	if ((config->get_show_text_flags() & GraphTextFlags::Current) == GraphTextFlags::Current) {
		// `space` at the end of line for offset from border
		String cur_text = Utils::string_format("%.2f %s ", (data->size() > 1 ? data->get(data->size() - 2) : 0), suffix);
		Vector2 cur_size = draw_font->get_string_size(cur_text);
		Vector2 text_pos = pos + Vector2(graphSize.x - cur_size.x, graphSize.y * 0.5f + cur_size.y * 0.5f - 2);
		ci->draw_string(draw_font, text_pos.floor(), cur_text, godot::HORIZONTAL_ALIGNMENT_LEFT, -1, 16, config->get_text_color()); // TODO font size must be in cofig, not in font
	}

	switch (config->get_position()) {
		case BlockPosition::LeftTop:
		case BlockPosition::RightTop:
			return Vector2(base_offset.x, border_size.position.y + border_size.size.y + 0);
		case BlockPosition::LeftBottom:
		case BlockPosition::RightBottom:
			return Vector2(base_offset.x, border_size.position.y - (config->is_show_title() ? title_size.y : -1));
	}

	return base_offset;
}

////////////////////////////////////
// FPSGraph

void FPSGraph::_update_added(real_t value) {
	if (is_ms != config->is_frame_time_mode()) {
		data->reset();
		is_ms = config->is_frame_time_mode();
		config->set_text_suffix(is_ms ? "ms" : "fps");
	}

	data->add(is_ms ? value * 1000.f : 1.f / value);
}

////////////////////////////////////
// DataGraphManager

DataGraphManager::DataGraphManager(DebugDraw3D *root) {
	owner = root;
}

DataGraphManager::~DataGraphManager() {
}

Ref<GraphParameters> DataGraphManager::create_graph(String title) {
	Ref<GraphParameters> config;
	config.instantiate();

	LOCK_GUARD(datalock);
	graphs[title] = std::make_shared<DataGraph>(config);
	return config;
}

Ref<GraphParameters> DataGraphManager::create_fps_graph(String title) {
	Ref<GraphParameters> config;
	config.instantiate();

	LOCK_GUARD(datalock);
	graphs[title] = std::make_shared<FPSGraph>(config);
	return config;
}

void DataGraphManager::_update_fps(real_t delta) {
	for (auto i : graphs) {
		if (i.second->get_type() == DataGraph::Type::FPS) {
			i.second->update(delta);
			owner->mark_canvas_needs_update();
		}
	}
}

void DataGraphManager::draw(CanvasItem *ci, Ref<Font> font, Vector2 vp_size) {
	Vector2 base_offset = owner->get_graphs_base_offset();
	Vector2 prev_offset[] = { base_offset, Vector2(base_offset.x, base_offset.y), Vector2(base_offset.x, vp_size.y - base_offset.y), Vector2(base_offset.x, vp_size.y - base_offset.y) };
	for (auto i : graphs) {
		int pos = i.second->get_config()->get_position();
		prev_offset[pos] = i.second->draw(ci, font, vp_size, i.first, prev_offset[pos]);
	}
}

void DataGraphManager::graph_update_data(String title, real_t data) {
	if (graphs.count(title)) {
		if (graphs[title]->get_type() != DataGraph::Type::FPS) {
			graphs[title]->update(data);
			owner->mark_canvas_needs_update();
		} else {
			PRINT_WARNING("Trying to manually update the FPS graph");
		}
	} else {
		PRINT_ERROR("Graph with name '" + title + "' not found");
	}
}

void DataGraphManager::remove_graph(String title) {
	LOCK_GUARD(datalock);
	if (graphs.count(title)) {
		graphs.erase(title);
	}
}

void DataGraphManager::clear_graphs() {
	LOCK_GUARD(datalock);
	graphs.clear();
}

Ref<GraphParameters> DataGraphManager::get_graph_config(String title) {
	if (graphs.count(title)) {
		return graphs[title]->get_config();
	}
	return Ref<GraphParameters>();
}

PackedStringArray DataGraphManager::get_graph_names() {
	LOCK_GUARD(datalock);

	PackedStringArray res;
	for (auto i : graphs) {
		res.append(i.first);
	}
	return res;
}