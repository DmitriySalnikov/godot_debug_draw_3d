#include "graphs.h"
#include "config_2d.h"
#include "debug_draw_2d.h"
#include "utils/utils.h"

#include <limits.h>

using namespace godot;

void DebugDraw2DGraph::_bind_methods() {
#define REG_CLASS_NAME DebugDraw2DGraph

	ClassDB::bind_method(D_METHOD(NAMEOF(get_title)), &DebugDraw2DGraph::get_title);
	ClassDB::bind_method(D_METHOD(NAMEOF(set_parent), "parent", "side"), &DebugDraw2DGraph::set_parent, GraphSide::SIDE_BOTTOM);

	REG_PROP_BOOL(enabled);
	REG_PROP_BOOL(upside_down);
	REG_PROP_BOOL(show_title);

	REG_PROP(show_text_flags, Variant::INT);
	REG_PROP(size, Variant::VECTOR2I);
	REG_PROP(buffer_size, Variant::INT);
	REG_PROP(offset, Variant::VECTOR2I);
	REG_PROP(corner, Variant::INT);
	REG_PROP(line_width, Variant::FLOAT);
	REG_PROP(line_color, Variant::COLOR);
	// REG_PROP(line_position, Variant::INT);
	REG_PROP(background_color, Variant::COLOR);
	REG_PROP(border_color, Variant::COLOR);
	REG_PROP(text_suffix, Variant::STRING);

	REG_PROP(custom_font, Variant::OBJECT);
	REG_PROP(title_size, Variant::INT);
	REG_PROP(text_size, Variant::INT);
	REG_PROP(title_color, Variant::COLOR);
	REG_PROP(text_color, Variant::COLOR);
	REG_PROP(text_precision, Variant::INT);

	REG_PROP(parent_graph, Variant::STRING_NAME);
	REG_PROP(parent_graph_side, Variant::INT);
	REG_PROP(data_getter, Variant::CALLABLE);

	/*BIND_ENUM_CONSTANT(LINE_TOP);
	BIND_ENUM_CONSTANT(LINE_CENTER);
	BIND_ENUM_CONSTANT(LINE_BOTTOM);*/

	BIND_ENUM_CONSTANT(POSITION_LEFT_TOP);
	BIND_ENUM_CONSTANT(POSITION_RIGHT_TOP);
	BIND_ENUM_CONSTANT(POSITION_LEFT_BOTTOM);
	BIND_ENUM_CONSTANT(POSITION_RIGHT_BOTTOM);

	BIND_ENUM_CONSTANT(SIDE_LEFT);
	BIND_ENUM_CONSTANT(SIDE_TOP);
	BIND_ENUM_CONSTANT(SIDE_RIGHT);
	BIND_ENUM_CONSTANT(SIDE_BOTTOM);

	BIND_BITFIELD_FLAG(TEXT_CURRENT);
	BIND_BITFIELD_FLAG(TEXT_AVG);
	BIND_BITFIELD_FLAG(TEXT_MAX);
	BIND_BITFIELD_FLAG(TEXT_MIN);
	BIND_BITFIELD_FLAG(TEXT_ALL);

#undef REG_CLASS_NAME
}

DebugDraw2DGraph::DebugDraw2DGraph(DataGraphManager *_owner, StringName _title) {
	_init(_owner, _title);
	if (IS_EDITOR_HINT()) {
		corner = GraphPosition::POSITION_LEFT_TOP;
	}
	graph_range.reset(get_buffer_size());
}

DebugDraw2DGraph::~DebugDraw2DGraph() {
	custom_font.unref();
}

void DebugDraw2DGraph::_init(DataGraphManager *_owner, StringName _title) {
	buffer_data = std::make_unique<CircularBuffer<double> >(get_buffer_size());
	owner = _owner;
	title = _title;
}

StringName DebugDraw2DGraph::get_title() const {
	return title;
}

void DebugDraw2DGraph::set_enabled(const bool _state) {
	enabled = _state;
}

bool DebugDraw2DGraph::is_enabled() const {
	return enabled;
}

void DebugDraw2DGraph::set_upside_down(const bool _state) {
	upside_down = _state;
}

bool DebugDraw2DGraph::is_upside_down() const {
	return upside_down;
}

void DebugDraw2DGraph::set_show_title(const bool _state) {
	show_title = _state;
}

bool DebugDraw2DGraph::is_show_title() const {
	return show_title;
}

// void DebugDraw2DGraph::set_line_position(const GraphLinePosition _position) {
//	line_position = _position;
// }
//
// DebugDraw2DGraph::GraphLinePosition DebugDraw2DGraph::get_line_position() const {
//	return line_position;
// }

void DebugDraw2DGraph::set_show_text_flags(const BitField<TextFlags> _flags) {
	show_text_flags = _flags;
}

BitField<DebugDraw2DGraph::TextFlags> DebugDraw2DGraph::get_show_text_flags() const {
	return show_text_flags;
}

void DebugDraw2DGraph::set_size(const Vector2i &_size) {
	size = _size;
	size.x = Math::clamp(size.x, 1, INT32_MAX);
	size.y = Math::clamp(size.y, 1, INT32_MAX);
}

Vector2i DebugDraw2DGraph::get_size() const {
	return size;
}

void DebugDraw2DGraph::set_buffer_size(const int _buf_size) {
	buffer_size = Math::clamp(_buf_size, 3, INT32_MAX);
}

int DebugDraw2DGraph::get_buffer_size() const {
	return buffer_size;
}

void DebugDraw2DGraph::set_offset(const Vector2i &_offset) {
	offset = _offset;
}

Vector2i DebugDraw2DGraph::get_offset() const {
	return offset;
}

void DebugDraw2DGraph::set_corner(const GraphPosition _position) {
	corner = (GraphPosition)_position;
}

DebugDraw2DGraph::GraphPosition DebugDraw2DGraph::get_corner() const {
	return corner;
}

void DebugDraw2DGraph::set_line_width(const real_t _width) {
	line_width = Math::clamp(_width, 1.0f, 32.0f);
}

real_t DebugDraw2DGraph::get_line_width() const {
	return line_width;
}

void DebugDraw2DGraph::set_line_color(const Color &_new_color) {
	line_color = _new_color;
}

Color DebugDraw2DGraph::get_line_color() const {
	return line_color;
}

void DebugDraw2DGraph::set_background_color(const Color &_new_color) {
	background_color = _new_color;
}

Color DebugDraw2DGraph::get_background_color() const {
	return background_color;
}

void DebugDraw2DGraph::set_border_color(const Color &_new_color) {
	border_color = _new_color;
}

Color DebugDraw2DGraph::get_border_color() const {
	return border_color;
}

void DebugDraw2DGraph::set_text_suffix(const String &_suffix) {
	text_suffix = _suffix;
}

String DebugDraw2DGraph::get_text_suffix() const {
	return text_suffix;
}

void DebugDraw2DGraph::set_custom_font(const Ref<Font> _custom_font) {
	custom_font = _custom_font;
}

Ref<Font> DebugDraw2DGraph::get_custom_font() const {
	return custom_font;
}

void DebugDraw2DGraph::set_title_size(const int _size) {
	title_size = Math::clamp(_size, 1, INT_MAX);
}

int DebugDraw2DGraph::get_title_size() const {
	return title_size;
}

void DebugDraw2DGraph::set_text_size(const int _size) {
	text_size = Math::clamp(_size, 1, INT_MAX);
}

int DebugDraw2DGraph::get_text_size() const {
	return text_size;
}

void DebugDraw2DGraph::set_title_color(const Color &_new_color) {
	title_color = _new_color;
}

Color DebugDraw2DGraph::get_title_color() const {
	return title_color;
}

void DebugDraw2DGraph::set_text_color(const Color &_new_color) {
	text_color = _new_color;
}

Color DebugDraw2DGraph::get_text_color() const {
	return text_color;
}

void DebugDraw2DGraph::set_text_precision(const int _precision) {
	text_precision = Math::clamp(_precision, 0, 64);
}

int DebugDraw2DGraph::get_text_precision() const {
	return text_precision;
}

void DebugDraw2DGraph::set_parent_graph(const StringName &_graph) {
	parent_graph = _graph;
}

StringName DebugDraw2DGraph::get_parent_graph() const {
	return parent_graph;
}

void DebugDraw2DGraph::set_parent_graph_side(const GraphSide _side) {
	parent_graph_side = _side;
}

DebugDraw2DGraph::GraphSide DebugDraw2DGraph::get_parent_graph_side() const {
	return parent_graph_side;
}

void DebugDraw2DGraph::set_parent(const StringName &_name, const GraphSide _side) {
	set_parent_graph(_name);
	set_parent_graph_side(_side);
}

void DebugDraw2DGraph::set_data_getter(const Callable &_callable) {
	if (data_getter != _callable)
		data_getter = _callable;
}

Callable DebugDraw2DGraph::get_data_getter() const {
	return data_getter;
}

void DebugDraw2DGraph::update(double _value) {
	ZoneScoped;
	LOCK_GUARD(datalock);

	if (get_buffer_size() != buffer_data->buffer_size()) {
		buffer_data = std::make_unique<CircularBuffer<double> >(get_buffer_size());
		graph_range.reset(get_buffer_size());
	}

	_update_received(_value);
}

void DebugDraw2DGraph::_update_received(double value) {
	LOCK_GUARD(datalock);

	buffer_data->add(value);
}

void DebugDraw2DGraph::graph_interpolated_values_range::update(const double &_min, const double &_max, const double &_avg, const double &_delta) {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING

	double _center_offset = Math::clamp(abs(max - min) * (0.25 * 0.5), 0.5, 1000.0); // react when the new limit is below 25% of the half range
	double range = abs(max - min);
	if (_max > max) {
		max = _max;
		upd_timer_max = max_timer_delay;
	} else {
		if (_max < max - _center_offset) {
			if (upd_timer_max > 0) {
				upd_timer_max -= _delta;

				if (range > 1.0)
					shrink_weight_max = buffer_size / range;
				else
					shrink_weight_max = 1.0;
			} else {
				max = Math::max(_max, Math::move_toward(max, _max, shrink_weight_max));
				if ((max - min) == 0) {
					max += shrink_weight_max;
				}
			}
		} else {
			upd_timer_max = max_timer_delay;
		}
	}

	if (_min < min) {
		min = _min;
		upd_timer_min = max_timer_delay;
	} else {
		if (_min > min + _center_offset) {
			if (upd_timer_min > 0) {
				upd_timer_min -= _delta;

				if (range > 1.0)
					shrink_weight_min = buffer_size / range;
				else
					shrink_weight_max = 1.0;
			} else {
				min = Math::min(_min, Math::move_toward(min, _min, shrink_weight_min));
				if ((max - min) == 0) {
					min -= shrink_weight_min;
				}
			}
		} else {
			upd_timer_min = max_timer_delay;
		}
	}

	avg = (max + min) * 0.5;
#endif
}

void DebugDraw2DGraph::graph_interpolated_values_range::reset(uint32_t _buffer_size, double _upd_timer_delay) {
	max_timer_delay = _upd_timer_delay;
	upd_timer_max = max_timer_delay;
	upd_timer_min = max_timer_delay;

	shrink_weight_max = 1.0;
	shrink_weight_min = 1.0;

	min = 0;
	max = 1;
	avg = 0.5;
	buffer_size = _buffer_size;
}

Vector2i DebugDraw2DGraph::_get_graph_position(const bool &_is_root, const DebugDraw2DGraph::GraphPosition &_corner, const DebugDraw2DGraph::graph_rects &_rects) const {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	Vector2i pos = _rects.base.position;

	Vector2i graph_size = get_size();
	Vector2i graph_offset = get_offset();

	if (_is_root) {
		switch (_corner) {
			case GraphPosition::POSITION_LEFT_TOP:
				pos.x = pos.x + graph_offset.x;
				pos.y = pos.y + graph_offset.y;
				break;
			case GraphPosition::POSITION_RIGHT_TOP:
				pos.x = pos.x - graph_size.x - graph_offset.x;
				pos.y = pos.y + graph_offset.y;
				break;
			case GraphPosition::POSITION_LEFT_BOTTOM:
				pos.x = pos.x + graph_offset.x;
				pos.y = pos.y - graph_size.y - graph_offset.y;
				break;
			case GraphPosition::POSITION_RIGHT_BOTTOM:
			default:
				pos.x = pos.x - graph_size.x - graph_offset.x;
				pos.y = pos.y - graph_size.y - graph_offset.y;
				break;
		}
	} else {
		bool is_right = _corner == POSITION_RIGHT_TOP || _corner == POSITION_RIGHT_BOTTOM;
		switch (get_parent_graph_side()) {
			case GraphSide::SIDE_LEFT:
				pos.x = _rects.full.position.x - graph_size.x - graph_offset.x;
				break;
			case GraphSide::SIDE_TOP:
				if (is_right)
					pos.x = _rects.full.position.x + _rects.full.size.x - graph_size.x;
				pos.y = _rects.full.position.y - graph_size.y - graph_offset.y;
				break;
			case GraphSide::SIDE_RIGHT:
				pos.x = _rects.full.position.x + _rects.full.size.x + graph_offset.x;
				break;
			case GraphSide::SIDE_BOTTOM:
			default:
				if (is_right)
					pos.x = _rects.full.position.x + _rects.full.size.x - graph_size.x;
				pos.y = _rects.full.position.y + _rects.full.size.y + graph_offset.y;
				break;
		}
	}

	return pos;
#else
	return Vector2i();
#endif
}

DebugDraw2DGraph::graph_rects DebugDraw2DGraph::draw(CanvasItem *_ci, const Ref<Font> &_font, const graph_rects &_prev_rects, const GraphPosition &_corner, const bool &_is_root, const double &_delta) const {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	if (!is_enabled())
		return _prev_rects;

	LOCK_GUARD(datalock);

	Ref<Font> draw_font = get_custom_font().is_null() ? _font : get_custom_font();

	double min, max, avg;
	buffer_data->get_min_max_avg(&min, &max, &avg);
	graph_range.update(min, max, avg, _delta);

	bool is_on_right_side = _corner == POSITION_RIGHT_TOP || _corner == POSITION_RIGHT_BOTTOM;
	bool is_on_top_side = _corner == POSITION_LEFT_TOP || _corner == POSITION_RIGHT_TOP;

	Vector2 graph_size = get_size();
	Vector2 pos = _get_graph_position(_is_root, _corner, _prev_rects);

	graph_rects rects = { Rect2i(pos, graph_size), Rect2i(pos, graph_size) };

	// Draw title
	if (is_show_title()) {
		String str_title = get_title();
		Vector2 title_size = draw_font->get_string_size(str_title, HORIZONTAL_ALIGNMENT_LEFT, -1.0, get_title_size());
		Vector2 title_pos = pos;
		if (is_on_top_side)
			pos += Vector2(0, title_size.y);
		else
			title_pos.y -= title_size.y;

		if (is_on_right_side)
			title_pos.x = title_pos.x + graph_size.x - title_size.x - 4;

		// 4 for horizontal padding
		Rect2 border_rect(title_pos, title_size + Vector2(4, 0));
		// Draw background

		real_t a = (real_t)draw_font->get_ascent(get_title_size());
		_ci->draw_rect(border_rect, get_background_color(), true);
		_ci->draw_string(draw_font,
				(title_pos + Vector2(2, a)).floor(),
				str_title,
				godot::HORIZONTAL_ALIGNMENT_LEFT, -1, get_title_size(), get_title_color());

		if (is_on_right_side)
			rects.full.position.x = rects.base.position.x = Math::min((int)pos.x, (int)title_pos.x);
		rects.full.size.x = rects.base.size.x = Math::max(rects.full.size.x, (int)border_rect.size.x);
		rects.full.size.y += (int)title_size.y;
		if (is_on_top_side)
			rects.base.position.y += (int)title_size.y;
		else
			rects.full.position.y -= (int)title_size.y;
	}

	Rect2 border_rect_fixed = Rect2i(pos + Vector2(1, 1), graph_size - Vector2(1, 1));

	// Draw background
	_ci->draw_rect(border_rect_fixed, get_background_color(), true);

	// Draw graph line
	if (buffer_data->is_filled() || buffer_data->size() > 2) {
		Vector2 base_pos = rects.base.position + Vector2(1, 1);
		Vector2 size = rects.base.size - Vector2(1, 3);
		double difference = max - min;
		double size_multiplier_x = size.x / (get_buffer_size() - 2);
		// double size_multiplier_y = difference != 0.0 ? size.y / Math::remap(buffer_data->get(0), min, max, graph_range.min, graph_range.max) : 0.0;
		PackedVector2Array line_points;

		// TODO: return the line to the center
		// TODO: fix the centering of the line when max and min are equal
		/*double center_offset = 0;
		switch (get_line_position()) {
			case DebugDraw2DGraph::LINE_TOP: {
				center_offset = 0;
				break;
			}
			case DebugDraw2DGraph::LINE_CENTER: {
				center_offset = graph_range.min * size_multiplier_y;
				break;
			}
			case DebugDraw2DGraph::LINE_BOTTOM: {
				center_offset = size.y - graph_range.min * size_multiplier_y - 1;
				break;
			}
		}*/

		{
			size_t s = buffer_data->size() - (int)!buffer_data->is_filled();
			line_points.resize(buffer_data->size() - 1);
			double top_limit = upside_down ? 1 : 0;
			double bottom_limit = upside_down ? 0 : 1;

			auto w = line_points.ptrw();
			for (size_t i = 0; i < s; i++) {
				w[i] = base_pos + Vector2((real_t)(size_multiplier_x * i), (real_t)(1.0 + Math::remap(buffer_data->get(i), graph_range.min, graph_range.max, top_limit, bottom_limit) * size.y));
			}
		}

		// prepare array for `draw_multiline`
		{
			size_t s = line_points.size() - 1;
			line_points.resize(line_points.size() * 2);
			auto w = line_points.ptrw();
			for (size_t i = s; i > 0; i--) {
				w[i * 2] = w[i];
				w[i * 2 + 1] = w[i - 1];
			}
		}

		_ci->draw_multiline(line_points, get_line_color(), get_line_width());
	}

	// Draw border
	_ci->draw_rect(border_rect_fixed, get_border_color(), false);

	auto format_float = [](auto real_val, int precision = 2) -> String {
		int size_s = std::snprintf(nullptr, 0, "%.*f", precision, real_val) + 1; // Extra space for '\0'
		if (size_s <= 0) {
			PRINT_ERROR("Error during formatting.");
			return "{FORMAT FAILED}";
		}
		std::unique_ptr<char[]> buf(new char[(size_t)size_s]);
		buf[(size_t)size_s - 1] = '\0';
		std::snprintf(buf.get(), (size_t)size_s, "%.*f", precision, real_val);
		return String(buf.get());
	};

	// Draw text
	int precision = get_text_precision();
	if (get_show_text_flags() & DebugDraw2DGraph::TextFlags::TEXT_MAX) {
		_ci->draw_string(draw_font,
				pos + Vector2(4, (real_t)draw_font->get_ascent(get_text_size()) - 1).floor(),
				FMT_STR("max: {0} {1}", format_float(max, precision), get_text_suffix()),
				godot::HORIZONTAL_ALIGNMENT_LEFT, -1, get_text_size(), get_text_color());
	}

	if (get_show_text_flags() & DebugDraw2DGraph::TextFlags::TEXT_AVG) {
		_ci->draw_string(draw_font,
				(pos + Vector2(4, (graph_size.y * 0.5f + (real_t)get_text_size() * 0.5f - 2))).floor(),
				FMT_STR("avg: {0} {1}", format_float(avg, precision), get_text_suffix()),
				godot::HORIZONTAL_ALIGNMENT_LEFT, -1, get_text_size(), get_text_color());
	}

	if (get_show_text_flags() & DebugDraw2DGraph::TextFlags::TEXT_MIN) {
		_ci->draw_string(draw_font,
				(pos + Vector2(4, graph_size.y - 3)).floor(),
				FMT_STR("min: {0} {1}", format_float(min, precision), get_text_suffix()),
				godot::HORIZONTAL_ALIGNMENT_LEFT, -1, get_text_size(), get_text_color());
	}

	if (get_show_text_flags() & DebugDraw2DGraph::TextFlags::TEXT_CURRENT) {
		// `space` at the end of the line to offset from the border
		String text = FMT_STR("{0} {1} ", format_float((buffer_data->size() > 1 ? buffer_data->get(buffer_data->size() - 2) : 0), precision), get_text_suffix());
		_ci->draw_string(draw_font,
				(pos + Vector2(graph_size.x - draw_font->get_string_size(text, HORIZONTAL_ALIGNMENT_LEFT, -1.0, get_text_size()).x, graph_size.y * 0.5f + get_text_size() * 0.5f - 2)).floor(),
				text,
				godot::HORIZONTAL_ALIGNMENT_LEFT, -1, get_text_size(), get_text_color());
	}

	// Debug outlines
#if 0
	Rect2i a = rects.base;
	a.position += Vector2i(1, 1);
	a.size -= Vector2i(2, 2);

	_ci->draw_rect(a, Colors::orange, false);
	_ci->draw_rect(rects.full, Colors::red, false);
#endif

	// Debug graph range
#if 0
	double _center_offset = abs(graph_range.max - graph_range.min) * (0.25 * 0.5);

	double _pos_y = Math::remap(graph_range.max - _center_offset, graph_range.max, graph_range.min, rects.base.position.y, rects.base.position.y + rects.base.size.y);
	_ci->draw_line(Vector2(rects.base.position.x, _pos_y), Vector2(rects.base.position.x + rects.base.size.x, _pos_y), Colors::green);
	_pos_y = Math::remap(graph_range.min + _center_offset, graph_range.max, graph_range.min, rects.base.position.y, rects.base.position.y + rects.base.size.y);
	_ci->draw_line(Vector2(rects.base.position.x, _pos_y), Vector2(rects.base.position.x + rects.base.size.x, _pos_y), Colors::green);

	_ci->draw_string(draw_font,
			pos + Vector2(4, (real_t)draw_font->get_ascent(get_text_size()) - 1).floor() + Vector2(100, 0),
			FMT_STR("{0}c / {1}t", format_float(graph_range.max, 1), format_float(graph_range.max - _center_offset, 1)),
			godot::HORIZONTAL_ALIGNMENT_LEFT, -1, 13, Colors::black);
	_ci->draw_string(draw_font,
			(pos + Vector2(4, graph_size.y - 3)).floor() + Vector2(100, 0),
			FMT_STR("{0}c / {1}t", format_float(graph_range.min, 1), format_float(graph_range.min + _center_offset, 1)),
			godot::HORIZONTAL_ALIGNMENT_LEFT, -1, 13, Colors::black);
#endif

	return rects;
#else
	return _prev_rects;
#endif
}

////////////////////////////////////
// DebugDraw2DFPSGraph

void DebugDraw2DFPSGraph::_bind_methods() {
#define REG_CLASS_NAME DebugDraw2DFPSGraph

	REG_PROP_BOOL(frame_time_mode);

#undef REG_CLASS_NAME
}

#ifndef DISABLE_DEBUG_RENDERING
void DebugDraw2DFPSGraph::_update_received(double _value) {
	ZoneScoped;
	LOCK_GUARD(datalock);
	if (is_ms != is_frame_time_mode()) {
		buffer_data->reset();
		is_ms = is_frame_time_mode();
		set_text_suffix(is_ms ? "ms" : "fps");
		set_upside_down(is_ms ? true : false);
	}

	buffer_data->add(is_ms ? _value * 1000.f : 1.f / _value);
}

DebugDraw2DFPSGraph::DebugDraw2DFPSGraph(DataGraphManager *_owner, StringName _title) {
	_init(_owner, _title);
};

void DebugDraw2DFPSGraph::set_data_getter(const Callable &_callable) {
	PRINT_WARNING("The FPS graph is already updated automatically");
}
#endif

void DebugDraw2DFPSGraph::set_frame_time_mode(const bool _state) {
	frametime_mode = _state;
}

bool DebugDraw2DFPSGraph::is_frame_time_mode() const {
	return frametime_mode;
}

////////////////////////////////////
// DataGraphManager

#ifndef DISABLE_DEBUG_RENDERING
DataGraphManager::DataGraphManager(DebugDraw2D *root) {
	owner = root;
}

DataGraphManager::~DataGraphManager() {
	for (auto &g : graphs) {
		g.unref();
	}
	graphs.clear();
}

void DataGraphManager::draw(CanvasItem *_ci, Ref<Font> _font, Vector2 _vp_size, double _delta) const {
	ZoneScoped;
	LOCK_GUARD(datalock);

	struct GraphsNode {
		const Ref<DebugDraw2DGraph> instance;
		std::vector<GraphsNode> children;
		GraphsNode(const Ref<DebugDraw2DGraph> &_node) :
				instance(_node){};
	};

	std::vector<GraphsNode> root_graphs;

	for (auto &p : graphs) {
		if (p->get_parent_graph().is_empty()) {
			root_graphs.push_back(p);
		}
	}

	std::function<void(GraphsNode *)> populate_nodes;
	populate_nodes = [&populate_nodes, this](GraphsNode *node) {
		for (auto &g : graphs) {
			if (g->get_parent_graph() == node->instance->get_title()) {
				GraphsNode n = g;
				populate_nodes(&n);
				node->children.push_back(n);
			}
		}
	};

	for (auto &n : root_graphs) {
		populate_nodes(&n);
	}

	Vector2 base_offset = owner->get_config()->get_graphs_base_offset();
	Rect2i base_rect[DebugDraw2DGraph::POSITION_MAX] = {
		Rect2i(base_offset, Vector2i()), // left_top
		Rect2i(Vector2(_vp_size.x - base_offset.x, base_offset.y), Vector2i()), // right_top
		Rect2i(Vector2(base_offset.x, _vp_size.y - base_offset.y), Vector2i()), // left_bottom
		Rect2i(Vector2(_vp_size.x - base_offset.x, _vp_size.y - base_offset.y), Vector2i()) // right_bottom
	};

	// 'rect' is a parameter storing the rectangle of the parent node
	// 'corner' is a parameter inherited from the root node
	std::function<void(const GraphsNode *, const DebugDraw2DGraph::graph_rects &, const DebugDraw2DGraph::GraphPosition &, const bool &)> iterate_nodes;
	iterate_nodes = [&, this](const GraphsNode *node, const DebugDraw2DGraph::graph_rects &rects, const DebugDraw2DGraph::GraphPosition &corner, const bool &is_root) {
		DebugDraw2DGraph::graph_rects prev_rects = node->instance->draw(_ci, _font, rects, corner, is_root, _delta);

		for (auto &g : node->children) {
			iterate_nodes(&g, prev_rects, corner, false);
		}
	};

	for (auto &n : root_graphs) {
		iterate_nodes(&n, { base_rect[n.instance->get_corner()], base_rect[n.instance->get_corner()] }, n.instance->get_corner(), true);
	}
}

Ref<DebugDraw2DGraph> DataGraphManager::create_graph(const StringName &_title) {
	ZoneScoped;
	ERR_FAIL_COND_V(_title.is_empty(), Ref<DebugDraw2DGraph>());

	Ref<DebugDraw2DGraph> config = Ref<DebugDraw2DGraph>(memnew(DebugDraw2DGraph(this, _title)));

	LOCK_GUARD(datalock);
	graphs.push_back(config);
	return config;
}

Ref<DebugDraw2DGraph> DataGraphManager::create_fps_graph(const StringName &_title) {
	ZoneScoped;
	ERR_FAIL_COND_V(_title.is_empty(), Ref<DebugDraw2DFPSGraph>());

	Ref<DebugDraw2DFPSGraph> config = Ref<DebugDraw2DFPSGraph>(memnew(DebugDraw2DFPSGraph(this, _title)));

	LOCK_GUARD(datalock);
	graphs.push_back(config);
	return config;
}

void DataGraphManager::auto_update_graphs(double _delta) {
	ZoneScoped;
	LOCK_GUARD(datalock);
	for (auto &i : graphs) {
		Ref<DebugDraw2DGraph> g = i;

		if (g->get_type() == DebugDraw2DGraph::GRAPH_FPS) {
			g->update(_delta);
			owner->mark_canvas_dirty();
		} else if (g->get_type() == DebugDraw2DGraph::GRAPH_NORMAL) {
			Callable callable = g->get_data_getter();
			if (callable.is_valid()) {
				Variant res = callable.callv(Array());
				if (res.get_type() == Variant::FLOAT || res.get_type() == Variant::INT) {
					g->update(res);
					owner->mark_canvas_dirty();
				}
			}
		}
	}
}

void DataGraphManager::graph_update_data(const StringName &_title, const double &_data) {
	ZoneScoped;
	LOCK_GUARD(datalock);

	auto graph = std::find_if(graphs.begin(), graphs.end(), [&_title](auto g) { return g->get_title() == _title; });
	if (graph != graphs.end()) {
		Ref<DebugDraw2DGraph> g = *graph;
		if (g->get_type() != DebugDraw2DGraph::GRAPH_FPS) {
			g->update(_data);
			owner->mark_canvas_dirty();
		} else {
			PRINT_WARNING("Trying to manually update the FPS graph");
		}

	} else {
		PRINT_ERROR("Graph with name '" + _title + "' not found");
	}
}

void DataGraphManager::remove_graph(const StringName &_title) {
	ZoneScoped;
	LOCK_GUARD(datalock);

	auto graph = std::find_if(graphs.begin(), graphs.end(), [&_title](auto g) { return g->get_title() == _title; });
	if (graph != graphs.end()) {
		graphs.erase(graph);
	}

	// Clear connections in other graphs
	for (auto &p : graphs) {
		if (p->get_parent_graph() == _title)
			p->set_parent_graph(StringName());
	}
}

void DataGraphManager::clear_graphs() {
	ZoneScoped;
	LOCK_GUARD(datalock);
	graphs.clear();
}

Ref<DebugDraw2DGraph> DataGraphManager::get_graph(const StringName &_title) const {
	ZoneScoped;
	LOCK_GUARD(datalock);

	auto graph = std::find_if(graphs.begin(), graphs.end(), [&_title](auto g) { return g->get_title() == _title; });
	if (graph != graphs.end()) {
		return *graph;
	}
	return Ref<DebugDraw2DGraph>();
}

PackedStringArray DataGraphManager::get_graph_names() const {
	ZoneScoped;
	LOCK_GUARD(datalock);

	PackedStringArray res;
	for (auto &i : graphs) {
		res.append(i->get_title());
	}
	return res;
}

size_t DataGraphManager::get_graphs_enabled() const {
	ZoneScoped;
	LOCK_GUARD(datalock);
	size_t total = 0;
	for (auto &g : graphs) {
		if (g->is_enabled())
			total++;
	}
	return total;
}

size_t DataGraphManager::get_graphs_total() const {
	ZoneScoped;
	LOCK_GUARD(datalock);
	return graphs.size();
}

#endif