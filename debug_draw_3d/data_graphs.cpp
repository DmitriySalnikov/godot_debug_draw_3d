#include "data_graphs.h"
#include "debug_draw.h"

#include <Engine.hpp>

using namespace godot;

void GraphParameters::_register_methods() {
#define REG_PROP(name, def) register_property(#name, &GraphParameters::set_##name, &GraphParameters::get_##name, def);
#define REG_PROP_BOOL(name, def) register_property(#name, &GraphParameters::set_##name, &GraphParameters::is_##name, def);

	REG_PROP_BOOL(enabled, true);
	REG_PROP_BOOL(show_title, false);
	REG_PROP_BOOL(frame_time_mode, true);
	REG_PROP_BOOL(centered_graph_line, true);
	REG_PROP(show_text_flags, (int)DebugDraw3D::FPSGraphTextFlags::All);
	REG_PROP(size, Vector2(256, 64));
	REG_PROP(buffer_size, 256);
	REG_PROP(offset, Vector2(8, 8));
	REG_PROP(position, (int)DebugDraw3D::BlockPosition::RightTop);
	REG_PROP(line_color, Color(1, 0.27f, 0, 1));
	REG_PROP(text_color, Color(0.96f, 0.96f, 0.96f, 1));
	REG_PROP(background_color, Color(0.2f, 0.2f, 0.2f, 0.6f));
	REG_PROP(border_color, Color(0, 0, 0, 1));
	REG_PROP(text_suffix, String());
	REG_PROP(custom_font, Ref<Font>());

#undef REG_PROP
#undef REG_PROP_BOOL
}

void GraphParameters::_init() {
	if (IS_EDITOR_HINT()) {
		FPSGraphOffset = Vector2(12, 72);
		FPSGraphPosition = DebugDraw3D::BlockPosition::LeftTop;
	}
}

void GraphParameters::set_enabled(bool state) {
	FPSGraphEnabled = state;
}

bool GraphParameters::is_enabled() {
	return FPSGraphEnabled;
}

void GraphParameters::set_show_title(bool state) {
	FPSGraphShowTitle = state;
}

bool GraphParameters::is_show_title() {
	return FPSGraphShowTitle;
}

void GraphParameters::set_frame_time_mode(bool state) {
	FPSGraphFrameTimeMode = state;
}

bool GraphParameters::is_frame_time_mode() {
	return FPSGraphFrameTimeMode;
}

void GraphParameters::set_centered_graph_line(bool state) {
	FPSGraphCenteredGraphLine = state;
}

bool GraphParameters::is_centered_graph_line() {
	return FPSGraphCenteredGraphLine;
}

void GraphParameters::set_show_text_flags(int flags) {
	FPSGraphShowTextFlags = (DebugDraw3D::FPSGraphTextFlags)flags;
}

int GraphParameters::get_show_text_flags() {
	return FPSGraphShowTextFlags;
}

void GraphParameters::set_size(Vector2 size) {
	FPSGraphSize = size;
}

Vector2 GraphParameters::get_size() {
	return FPSGraphSize;
}

void GraphParameters::set_buffer_size(int buf_size) {
	FPSBufferSize = Math::clamp(buf_size, 1, INT_MAX);
}

int GraphParameters::get_buffer_size() {
	return FPSBufferSize;
}

void GraphParameters::set_offset(Vector2 offset) {
	FPSGraphOffset = offset;
}

Vector2 GraphParameters::get_offset() {
	return FPSGraphOffset;
}

void GraphParameters::set_position(int position) {
	FPSGraphPosition = (DebugDraw3D::BlockPosition)position;
}

int GraphParameters::get_position() {
	return FPSGraphPosition;
}

void GraphParameters::set_line_color(Color new_color) {
	FPSGraphLineColor = new_color;
}

Color GraphParameters::get_line_color() {
	return FPSGraphLineColor;
}

void GraphParameters::set_text_color(Color new_color) {
	FPSGraphTextColor = new_color;
}

Color GraphParameters::get_text_color() {
	return FPSGraphTextColor;
}

void GraphParameters::set_background_color(Color new_color) {
	FPSGraphBackgroundColor = new_color;
}

Color GraphParameters::get_background_color() {
	return FPSGraphBackgroundColor;
}

void GraphParameters::set_border_color(Color new_color) {
	FPSGraphBorderColor = new_color;
}

Color GraphParameters::get_border_color() {
	return FPSGraphBorderColor;
}

void GraphParameters::set_text_suffix(String suffix) {
	FPSGraphTextSuffix = suffix;
}

String GraphParameters::get_text_suffix() {
	return FPSGraphTextSuffix;
}

void GraphParameters::set_custom_font(Ref<Font> custom_font) {
	FPSGraphCustomFont = custom_font;
}

Ref<Font> GraphParameters::get_custom_font() {
	return FPSGraphCustomFont;
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
	LOCK_GUARD_REC(datalock);

	if (config->get_buffer_size() != data->buffer_size())
		// data = CircularBuffer<real_t>(config->get_buffer_size());
		data = std::make_shared<CircularBuffer<real_t> >(config->get_buffer_size());

	_update_added(value);
}

void DataGraph::_update_added(real_t value) {
	data->add(value);
}

real_t DataGraph::draw(CanvasItem *ci, Ref<Font> font, Vector2 vp_size, String title, real_t y_offset) {
	if (!config->is_enabled())
		return y_offset;

	LOCK_GUARD_REC(datalock);

	Ref<Font> draw_font = config->get_custom_font().is_null() ? font : config->get_custom_font();

	real_t min, max, avg;
	Utils::get_min_max_avg(&data, &min, &max, &avg);

	// Truncate for pixel perfect render
	Vector2 graphSize(Vector2((real_t)(int)config->get_size().x, (real_t)(int)config->get_size().y));
	Vector2 graphOffset(Vector2((real_t)(int)config->get_offset().x, (real_t)(int)config->get_offset().y));
	Vector2 pos = graphOffset;
	Vector2 title_size = draw_font->get_string_size(title);

	switch (config->get_position()) {
		case DebugDraw3D::BlockPosition::LeftTop:
			pos.y += y_offset;
			break;
		case DebugDraw3D::BlockPosition::RightTop:
			pos = Vector2(vp_size.x - graphSize.x - graphOffset.x + 1, graphOffset.y + y_offset);
			break;
		case DebugDraw3D::BlockPosition::LeftBottom:
			pos = Vector2(graphOffset.x, y_offset - graphSize.y - graphOffset.y - (config->is_show_title() ? title_size.y - 3 : 0));
			break;
		case DebugDraw3D::BlockPosition::RightBottom:
			pos = Vector2(vp_size.x - graphSize.x - graphOffset.x + 1, y_offset - graphSize.y - graphOffset.y - (config->is_show_title() ? title_size.y - 3 : 0));
			break;
	}

	if (config->is_show_title()) {
		Vector2 title_pos = pos;

		switch (config->get_position()) {
			case DebugDraw3D::BlockPosition::RightTop:
			case DebugDraw3D::BlockPosition::RightBottom:
				title_pos.x = title_pos.x + graphSize.x - title_size.x - 8;
				break;
		}

		real_t max_height = draw_font->get_ascent();
		Rect2 border_size(title_pos + Vector2(0, -4), title_size + Vector2(8, 0));
		// Draw background
		ci->draw_rect(border_size, config->get_background_color(), true);
		ci->draw_string(draw_font, title_pos + Vector2(4, max_height - 3), title, config->get_text_color());

		pos += Vector2(0, max_height);
	}

	real_t height_multiplier = graphSize.y / max;
	real_t center_offset = config->is_centered_graph_line() ? (graphSize.y - height_multiplier * (max - min)) * 0.5f : 0;

	Rect2 border_size(pos + Vector2::UP, graphSize + Vector2::DOWN);

	// Draw background
	ci->draw_rect(border_size, config->get_background_color(), true);

	// Draw graph line
	if (data->is_filled() || data->size() > 2) {
		PoolVector2Array line_points;
		const int offset = (int)data->is_filled();
		double points_interval = (double)config->get_size().x / ((int64_t)config->get_buffer_size() - 1 - offset);

		line_points.resize((int)data->size() - offset);
		{
			auto w = line_points.write();
			for (size_t i = 0; i < data->size() - offset; i++) {
				w[(int)i] = pos + Vector2((real_t)(i * points_interval), graphSize.y - data->get(i) * height_multiplier + center_offset);
			}
		}
		ci->draw_polyline(line_points, config->get_line_color());
	}

	// Draw border
	ci->draw_rect(border_size, config->get_border_color(), false);

	// Draw text
	const wchar_t *suffix = config->get_text_suffix().unicode_str();

	if ((config->get_show_text_flags() & DebugDraw3D::FPSGraphTextFlags::Max) == DebugDraw3D::FPSGraphTextFlags::Max) {
		String max_text = Utils::string_format(L"max: %.2f %s", max, suffix);
		real_t max_height = draw_font->get_height();
		ci->draw_string(draw_font, pos + Vector2(4, max_height - 1), max_text, config->get_text_color());
	}

	if ((config->get_show_text_flags() & DebugDraw3D::FPSGraphTextFlags::Avarage) == DebugDraw3D::FPSGraphTextFlags::Avarage) {
		String avg_text = Utils::string_format(L"avg: %.2f %s", avg, suffix);
		real_t avg_height = draw_font->get_height();
		ci->draw_string(draw_font, pos + Vector2(4, graphSize.y * 0.5f + avg_height * 0.5f - 2), avg_text, config->get_text_color());
	}

	if ((config->get_show_text_flags() & DebugDraw3D::FPSGraphTextFlags::Min) == DebugDraw3D::FPSGraphTextFlags::Min) {
		String min_text = Utils::string_format(L"min: %.2f %s", min, suffix);
		ci->draw_string(draw_font, pos + Vector2(4, graphSize.y - 3), min_text, config->get_text_color());
	}

	if ((config->get_show_text_flags() & DebugDraw3D::FPSGraphTextFlags::Current) == DebugDraw3D::FPSGraphTextFlags::Current) {
		// `space` at the end of line for offset from border
		String cur_text = Utils::string_format(L"%.2f %s ", (data->size() > 1 ? data->get(data->size() - 2) : 0), suffix);
		Vector2 cur_size = draw_font->get_string_size(cur_text);
		ci->draw_string(draw_font, pos + Vector2(graphSize.x - cur_size.x, graphSize.y * 0.5f + cur_size.y * 0.5f - 2), cur_text, config->get_text_color());
	}

	switch (config->get_position()) {
		case DebugDraw3D::BlockPosition::LeftTop:
		case DebugDraw3D::BlockPosition::RightTop:
			return border_size.position.y + border_size.size.y + 0;
		case DebugDraw3D::BlockPosition::LeftBottom:
		case DebugDraw3D::BlockPosition::RightBottom:
			return border_size.position.y - (config->is_show_title() ? title_size.y : -1);
	}

	return y_offset;
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

Ref<GraphParameters> DataGraphManager::create_graph(String title) {
	Ref<GraphParameters> config;
	config.instance();

	LOCK_GUARD_REC(datalock);
	graphs[title] = std::make_shared<DataGraph>(config);
	return config;
}

Ref<GraphParameters> DataGraphManager::create_fps_graph(String title) {
	Ref<GraphParameters> config;
	config.instance();

	LOCK_GUARD_REC(datalock);
	graphs[title] = std::make_shared<FPSGraph>(config);
	return config;
}

void DataGraphManager::_update_fps(real_t delta) {
	for (auto i : graphs) {
		if (i.second->get_type() == DataGraph::Type::FPS) {
			i.second->update(delta);
		}
	}
}

void DataGraphManager::draw(CanvasItem *ci, Ref<Font> font, Vector2 vp_size) {
	real_t prev_offset[] = { 0, 0, vp_size.y, vp_size.y };
	for (auto i : graphs) {
		int pos = i.second->get_config()->get_position();
		prev_offset[pos] = i.second->draw(ci, font, vp_size, i.first, prev_offset[pos]);
	}
}

void DataGraphManager::update(String title, real_t data) {
	if (graphs.count(title)) {
		if (graphs[title]->get_type() != DataGraph::Type::FPS) {
			graphs[title]->update(data);
		} else {
			PRINT_WARNING("Trying to manually update the FPS graph");
		}
	} else {
		PRINT_ERROR("Graph with name '" + title + "' not found");
	}
}

void DataGraphManager::remove_graph(String title) {
	LOCK_GUARD_REC(datalock);
	if (graphs.count(title)) {
		graphs.erase(title);
	}
}

void DataGraphManager::clear_graphs() {
	LOCK_GUARD_REC(datalock);
	graphs.clear();
}

Ref<GraphParameters> DataGraphManager::get_graph_config(String title) {
	if (graphs.count(title)) {
		return graphs[title]->get_config();
	}
	return Ref<GraphParameters>();
}

PoolStringArray DataGraphManager::get_graph_names() {
	LOCK_GUARD_REC(datalock);

	PoolStringArray res;
	for (auto i : graphs) {
		res.append(i.first);
	}
	return res;
}