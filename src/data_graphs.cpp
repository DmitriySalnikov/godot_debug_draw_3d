#include "data_graphs.h"
#include "debug_draw.h"
#include "math_utils.h"
#include "utils.h"

#include <limits.h>

using namespace godot;

void GraphParameters::_bind_methods() {
#define REG_CLASS_NAME GraphParameters

	ClassDB::bind_method(D_METHOD(TEXT(get_title)), &GraphParameters::get_title);
	ClassDB::bind_method(D_METHOD(TEXT(set_parent), "parent", "side"), &GraphParameters::set_parent, GraphSide::SIDE_BOTTOM);

	REG_PROP_BOOL(enabled);
	REG_PROP_BOOL(show_title);

	REG_PROP(show_text_flags, Variant::INT);
	REG_PROP(size, Variant::VECTOR2I);
	REG_PROP(buffer_size, Variant::INT);
	REG_PROP(offset, Variant::VECTOR2I);
	REG_PROP(corner, Variant::INT);
	REG_PROP(line_width, Variant::FLOAT);
	REG_PROP(line_color, Variant::COLOR);
	REG_PROP(line_position, Variant::INT);
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

	BIND_ENUM_CONSTANT(LINE_TOP);
	BIND_ENUM_CONSTANT(LINE_CENTER);
	BIND_ENUM_CONSTANT(LINE_BOTTOM);

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

GraphParameters::GraphParameters(DataGraphManager *_owner, StringName _title) {
	_init(_owner, _title);
	if (IS_EDITOR_HINT()) {
		corner = GraphPosition::POSITION_LEFT_TOP;
	}
}

void GraphParameters::_init(DataGraphManager *_owner, StringName _title) {
	buffer_data = std::make_unique<CircularBuffer<double> >(get_buffer_size());
	owner = _owner;
	title = _title;
}

StringName GraphParameters::get_title() const {
	return title;
}

void GraphParameters::set_enabled(const bool _state) {
	enabled = _state;
}

bool GraphParameters::is_enabled() const {
	return enabled;
}

void GraphParameters::set_show_title(const bool _state) {
	show_title = _state;
}

bool GraphParameters::is_show_title() const {
	return show_title;
}

void GraphParameters::set_line_position(const GraphLinePosition _position) {
	line_position = _position;
}

GraphParameters::GraphLinePosition GraphParameters::get_line_position() const {
	return line_position;
}

void GraphParameters::set_show_text_flags(const BitField<TextFlags> _flags) {
	show_text_flags = _flags;
}

BitField<GraphParameters::TextFlags> GraphParameters::get_show_text_flags() const {
	return show_text_flags;
}

void GraphParameters::set_size(const Vector2i &_size) {
	size = _size;
	size.x = Math::clamp(size.x, 1, INT32_MAX);
	size.y = Math::clamp(size.y, 1, INT32_MAX);
}

Vector2i GraphParameters::get_size() const {
	return size;
}

void GraphParameters::set_buffer_size(const int _buf_size) {
	buffer_size = Math::clamp(_buf_size, 3, INT32_MAX);
}

int GraphParameters::get_buffer_size() const {
	return buffer_size;
}

void GraphParameters::set_offset(const Vector2i &_offset) {
	offset = _offset;
}

Vector2i GraphParameters::get_offset() const {
	return offset;
}

void GraphParameters::set_corner(const GraphPosition _position) {
	corner = (GraphPosition)_position;
}

GraphParameters::GraphPosition GraphParameters::get_corner() const {
	return corner;
}

void GraphParameters::set_line_width(const real_t _width) {
	line_width = Math::clamp(_width, 1.0f, 32.0f);
}

real_t GraphParameters::get_line_width() const {
	return line_width;
}

void GraphParameters::set_line_color(const Color &_new_color) {
	line_color = _new_color;
}

Color GraphParameters::get_line_color() const {
	return line_color;
}

void GraphParameters::set_background_color(const Color &_new_color) {
	background_color = _new_color;
}

Color GraphParameters::get_background_color() const {
	return background_color;
}

void GraphParameters::set_border_color(const Color &_new_color) {
	border_color = _new_color;
}

Color GraphParameters::get_border_color() const {
	return border_color;
}

void GraphParameters::set_text_suffix(const String &_suffix) {
	text_suffix = _suffix;
}

String GraphParameters::get_text_suffix() const {
	return text_suffix;
}

void GraphParameters::set_custom_font(const Ref<Font> _custom_font) {
	custom_font = _custom_font;
}

Ref<Font> GraphParameters::get_custom_font() const {
	return custom_font;
}

void GraphParameters::set_title_size(const int _size) {
	title_size = Math::clamp(_size, 1, INT_MAX);
}

int GraphParameters::get_title_size() const {
	return title_size;
}

void GraphParameters::set_text_size(const int _size) {
	text_size = Math::clamp(_size, 1, INT_MAX);
}

int GraphParameters::get_text_size() const {
	return text_size;
}

void GraphParameters::set_title_color(const Color &_new_color) {
	title_color = _new_color;
}

Color GraphParameters::get_title_color() const {
	return title_color;
}

void GraphParameters::set_text_color(const Color &_new_color) {
	text_color = _new_color;
}

Color GraphParameters::get_text_color() const {
	return text_color;
}

void GraphParameters::set_text_precision(const int _precision) {
	text_precision = Math::clamp(_precision, 0, 64);
}

int GraphParameters::get_text_precision() const {
	return text_precision;
}

void GraphParameters::set_parent_graph(const StringName &_graph) {
	parent_graph = _graph;
}

StringName GraphParameters::get_parent_graph() const {
	return parent_graph;
}

void GraphParameters::set_parent_graph_side(const GraphSide _side) {
	parent_graph_side = _side;
}

GraphParameters::GraphSide GraphParameters::get_parent_graph_side() const {
	return parent_graph_side;
}

void GraphParameters::set_parent(const StringName &_name, const GraphSide _side) {
	set_parent_graph(_name);
	set_parent_graph_side(_side);
}

void GraphParameters::update(double _value) {
	LOCK_GUARD(datalock);

	if (get_buffer_size() != buffer_data->buffer_size())
		buffer_data = std::make_unique<CircularBuffer<double> >(get_buffer_size());

	_update_received(_value);
}

void GraphParameters::_update_received(double value) {
	LOCK_GUARD(datalock);

	buffer_data->add(value);
}

Vector2i GraphParameters::_get_graph_position(const bool &_is_root, const GraphParameters::GraphPosition &_corner, const GraphParameters::graph_rects &_rects) const {
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
				if (is_right)
					pos.x = _rects.full.position.x + _rects.full.size.x - graph_size.x;
				pos.y = _rects.full.position.y + _rects.full.size.y + graph_offset.y;
				break;
		}
	}

	return pos;
}

GraphParameters::graph_rects GraphParameters::draw(CanvasItem *_ci, const Ref<Font> &_font, const graph_rects &_prev_rects, const GraphPosition &_corner, const bool &_is_root) const {
	if (!is_enabled())
		return _prev_rects;

	LOCK_GUARD(datalock);

	Ref<Font> draw_font = get_custom_font().is_null() ? _font : get_custom_font();

	double min, max, avg;
	buffer_data->get_min_max_avg(&min, &max, &avg);

	bool is_on_right_side = _corner == POSITION_RIGHT_TOP || _corner == POSITION_RIGHT_BOTTOM;
	bool is_on_top_side = _corner == POSITION_LEFT_TOP || _corner == POSITION_RIGHT_TOP;

	Vector2 graph_size = get_size();
	Vector2 graph_offset = get_offset();
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
		Vector2 base_pos = border_rect_fixed.position + Vector2(0, 1);
		double height = border_rect_fixed.size.y - 1;
		double height_multiplier = height / max;
		double center_offset = 0;
		switch (get_line_position()) {
			case GraphParameters::LINE_TOP: {
				center_offset = 0;
				break;
			}
			case GraphParameters::LINE_CENTER: {
				center_offset = (height - height_multiplier * (max - min)) * 0.5;
				break;
			}
			case GraphParameters::LINE_BOTTOM: {
				center_offset = height - height_multiplier * (max - min) - 1;
				break;
			}
		}

		PackedVector2Array line_points;

		const int offset = (int)buffer_data->is_filled();
		double points_interval = (double)border_rect_fixed.size.x / ((int64_t)get_buffer_size() - 1 - offset);

		line_points.resize((buffer_data->size() - offset) * 2);
		{
			auto w = line_points.ptrw();
			for (size_t i = 1; i < buffer_data->size() - offset; i++) {
				w[(int)i * 2] = base_pos + Vector2((real_t)(i * points_interval), (real_t)(height - (buffer_data->get(i) * height_multiplier) + center_offset));
				w[(int)i * 2 + 1] = base_pos + Vector2((real_t)((i - 1) * points_interval), (real_t)(height - (buffer_data->get(i - 1) * height_multiplier) + center_offset));
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
		buf[size_s - 1] = '\0';
		std::snprintf(buf.get(), (size_t)size_s, "%.*f", precision, real_val);
		return String(buf.get());
	};

	// Draw text
	int precision = get_text_precision();
	if (get_show_text_flags() & GraphParameters::TextFlags::TEXT_MAX) {
		_ci->draw_string(draw_font,
				pos + Vector2(4, (real_t)draw_font->get_ascent(get_text_size()) - 1).floor(),
				String("max: {0} {1}").format(Array::make(format_float(max, precision), get_text_suffix())),
				godot::HORIZONTAL_ALIGNMENT_LEFT, -1, get_text_size(), get_text_color());
	}

	if (get_show_text_flags() & GraphParameters::TextFlags::TEXT_AVG) {
		_ci->draw_string(draw_font,
				(pos + Vector2(4, (graph_size.y * 0.5f + (real_t)get_text_size() * 0.5f - 2))).floor(),
				String("avg: {0} {1}").format(Array::make(format_float(avg, precision), get_text_suffix())),
				godot::HORIZONTAL_ALIGNMENT_LEFT, -1, get_text_size(), get_text_color());
	}

	if (get_show_text_flags() & GraphParameters::TextFlags::TEXT_MIN) {
		_ci->draw_string(draw_font,
				(pos + Vector2(4, graph_size.y - 3)).floor(),
				String("min: {0} {1}").format(Array::make(format_float(min, precision), get_text_suffix())),
				godot::HORIZONTAL_ALIGNMENT_LEFT, -1, get_text_size(), get_text_color());
	}

	if (get_show_text_flags() & GraphParameters::TextFlags::TEXT_CURRENT) {
		// `space` at the end of the line to offset from the border
		String text = String("{0} {1} ").format(Array::make(format_float((buffer_data->size() > 1 ? buffer_data->get(buffer_data->size() - 2) : 0), precision), get_text_suffix()));
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

	return rects;
}

////////////////////////////////////
// FPSGraphParameters

void FPSGraphParameters::_update_received(double _value) {
	LOCK_GUARD(datalock);
	if (is_ms != is_frame_time_mode()) {
		buffer_data->reset();
		is_ms = is_frame_time_mode();
		set_text_suffix(is_ms ? "ms" : "fps");
	}

	buffer_data->add(is_ms ? _value * 1000.f : 1.f / _value);
}

FPSGraphParameters::FPSGraphParameters(DataGraphManager *_owner, StringName _title) {
	_init(_owner, _title);
};

void FPSGraphParameters::_bind_methods() {
#define REG_CLASS_NAME FPSGraphParameters

	REG_PROP_BOOL(frame_time_mode);

#undef REG_CLASS_NAME
}

void FPSGraphParameters::set_frame_time_mode(const bool _state) {
	frametime_mode = _state;
}

bool FPSGraphParameters::is_frame_time_mode() const {
	return frametime_mode;
}

////////////////////////////////////
// DataGraphManager

DataGraphManager::DataGraphManager(DebugDraw *root) {
	owner = root;
}

DataGraphManager::~DataGraphManager() {
}

void DataGraphManager::draw(CanvasItem *_ci, Ref<Font> _font, Vector2 _vp_size) const {
	LOCK_GUARD(datalock);

	struct GraphsNode {
		const Ref<GraphParameters> instance;
		std::vector<GraphsNode> children;
		GraphsNode(const Ref<GraphParameters> &_node) :
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

	Vector2 base_offset = owner->get_graphs_base_offset();
	Rect2i base_rect[GraphParameters::POSITION_MAX] = {
		Rect2i(base_offset, Vector2i()), // left_top
		Rect2i(Vector2(_vp_size.x - base_offset.x, base_offset.y), Vector2i()), // right_top
		Rect2i(Vector2(base_offset.x, _vp_size.y - base_offset.y), Vector2i()), // left_bottom
		Rect2i(Vector2(_vp_size.x - base_offset.x, _vp_size.y - base_offset.y), Vector2i()) // right_bottom
	};

	// 'rect' is a parameter storing the rectangle of the parent node
	// 'corner' is a parameter inherited from the root node
	std::function<void(GraphsNode *, GraphParameters::graph_rects, GraphParameters::GraphPosition, bool)> iterate_nodes;
	iterate_nodes = [&, this](GraphsNode *node, GraphParameters::graph_rects rects, GraphParameters::GraphPosition corner, bool is_root) {
		GraphParameters::graph_rects prev_rects = node->instance->draw(_ci, _font, rects, corner, is_root);

		for (auto &g : node->children) {
			iterate_nodes(&g, prev_rects, corner, false);
		}
	};

	for (auto &n : root_graphs) {
		iterate_nodes(&n, { base_rect[n.instance->get_corner()], base_rect[n.instance->get_corner()] }, n.instance->get_corner(), true);
	}
}

Ref<GraphParameters> DataGraphManager::create_graph(const StringName &_title) {
	ERR_FAIL_COND_V(_title.is_empty(), Ref<GraphParameters>());

	Ref<GraphParameters> config = Ref<GraphParameters>(memnew(GraphParameters(this, _title)));

	LOCK_GUARD(datalock);
	graphs.push_back(config);
	return config;
}

Ref<GraphParameters> DataGraphManager::create_fps_graph(const StringName &_title) {
	ERR_FAIL_COND_V(_title.is_empty(), Ref<FPSGraphParameters>());

	Ref<FPSGraphParameters> config = Ref<FPSGraphParameters>(memnew(FPSGraphParameters(this, _title)));

	LOCK_GUARD(datalock);
	graphs.push_back(config);
	return config;
}

void DataGraphManager::update_fps_graphs(double _delta) {
	LOCK_GUARD(datalock);
	for (auto &i : graphs) {
		Ref<GraphParameters> g = i;
		if (g->get_type() == GraphParameters::GRAPH_FPS) {
			g->update(_delta);
			owner->mark_canvas_dirty();
		}
	}
}

void DataGraphManager::graph_update_data(const StringName &_title, const double &_data) {
	LOCK_GUARD(datalock);

	auto graph = Utils::find<Ref<GraphParameters> >(graphs, [&_title](auto g) { return g->get_title() == _title; });
	if (graph != graphs.end()) {
		Ref<GraphParameters> g = *graph;
		if (g->get_type() != GraphParameters::GRAPH_FPS) {
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
	LOCK_GUARD(datalock);

	auto graph = Utils::find<Ref<GraphParameters> >(graphs, [&_title](auto g) { return g->get_title() == _title; });
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
	LOCK_GUARD(datalock);
	graphs.clear();
}

Ref<GraphParameters> DataGraphManager::get_graph_config(const StringName &_title) const {
	LOCK_GUARD(datalock);

	auto graph = Utils::find<Ref<GraphParameters> >(graphs, [&_title](auto g) { return g->get_title() == _title; });
	if (graph != graphs.end()) {
		return *graph;
	}
	return Ref<GraphParameters>();
}

PackedStringArray DataGraphManager::get_graph_names() const {
	LOCK_GUARD(datalock);

	PackedStringArray res;
	for (auto &i : graphs) {
		res.append(i->get_title());
	}
	return res;
}