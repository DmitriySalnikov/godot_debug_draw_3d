#pragma once
#ifndef DISABLE_DEBUG_RENDERING

#include "common/colors.h"
// #include "geometry_generators.h"
// #include "render_instances.h"
#include "utils/math_utils.h"
#include "utils/utils.h"
#include "stats_2d.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/multi_mesh_instance3d.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/texture.hpp>
GODOT_WARNING_RESTORE()

#include <map>

using namespace godot;

class DebugDrawStats3D;

enum class InstanceType2D : char {
	Line,
	DashedLine,
	PolylineColors, // Used to draw Polyline, PolylineColors
	Arc,
	MultilineColors, // Used to draw Multiline, MultilineColors
	Rect,
	Circle,
	Texture,
	TextureRect,
	TextureRectRegion,
	StyleBox,
	Primitive,
	ColoredPolygon, // Used to draw Polygon, ColoredPolygon
	MultilineStringOutline, // Used to draw String, MultilineString, StringOutline, MultilineStringOutline
	Mesh,
	MultiMesh,
	ALL
};

class GeometryPoolDistanceCullingData2D {
public:
	real_t m_max_distance;
	std::vector<Vector3> m_camera_positions;
	GeometryPoolDistanceCullingData2D(const real_t &t_max_distance, const std::vector<Vector3> &t_camera_positions) {
		m_max_distance = t_max_distance;
		m_camera_positions = t_camera_positions;
	}
};

template <class TBounds>
class DelayedRenderer2D {
protected:
	void _update(double exp_time, bool is_vis) {
		expiration_time = exp_time;
		is_used_one_time = false;
		is_visible = is_vis;
	}

public:
	double expiration_time = 0;
	bool is_used_one_time = false;
	bool is_visible = true;
	TBounds bounds;
	Color color;

	DelayedRenderer2D() :
			bounds(),
			expiration_time(0),
			is_used_one_time(true),
			is_visible(false) {}

	bool is_expired() const {
		return expiration_time > 0 ? false : is_used_one_time;
	}

	// TODO: move to GeometryPool2D to avoid many calls and copies
	// TODO: replace `std::vector<Plane>` by template and change it to support 2D
	bool update_visibility(const std::vector<std::vector<Plane> > &t_frustums, const GeometryPoolDistanceCullingData2D &t_distance_data, bool _skip_expiration_check) {
		if (_skip_expiration_check || !is_expired()) {
			is_visible = false;

			if (t_distance_data.m_max_distance > 0 && t_distance_data.m_camera_positions.size()) {
				for (const auto &pos : t_distance_data.m_camera_positions) {
					if (pos.distance_to(bounds.position) > t_distance_data.m_max_distance) {
						is_visible = false;
						return is_visible;
					}
				}
			}

			if (t_frustums.size()) {
				for (const auto &frustum : t_frustums) {
					if (MathUtils::is_bounds_partially_inside_convex_shape(bounds, frustum)) {
						is_visible = true;
						return is_visible;
					}
				}
				return false;
			} else {
				is_visible = true;
				return is_visible;
			}
		}
		is_visible = false;
		return is_visible;
	}

	void update_expiration(double delta) {
		if (!is_expired()) {
			expiration_time -= delta;
		}
	}
};

class DelayedRendererInstance2D : public DelayedRenderer2D<SphereBounds> {
public:
	Transform3D transform;
	InstanceType2D type = InstanceType2D::Line;

	DelayedRendererInstance2D(){};
	// void update(real_t _exp_time, const InstanceType2D &_type, const Transform3D &_transform, const Color &_col, const SphereBounds &_bounds);
};

class GeometryPool2D {
private:
	template <class TInst>
	struct ObjectsPool {
		const real_t TIME_USED_TO_SHRINK_INSTANT = 10; // 10 sec
		const real_t TIME_USED_TO_SHRINK_DELAYED = 15; // 15 sec

		std::vector<TInst> instant = {};
		std::vector<TInst> delayed = {};
		int visible_objects = 0;

		size_t used_instant = 0;
		size_t _prev_used_instant = 0;
		size_t _prev_not_expired_delayed = 0;
		size_t used_delayed = 0;
		double time_used_less_then_half_of_instant_pool = 0;
		double time_used_less_then_quarter_of_delayed_pool = 0;

		ObjectsPool() {
			time_used_less_then_half_of_instant_pool = TIME_USED_TO_SHRINK_INSTANT;
			time_used_less_then_quarter_of_delayed_pool = TIME_USED_TO_SHRINK_DELAYED;
		}

		TInst *get(bool is_delayed) {
			auto objs = is_delayed ? &delayed : &instant;
			auto used = is_delayed ? &_prev_not_expired_delayed : &used_instant;

			if (is_delayed) {
				while (objs->size() != (*used)) {
					if (((*objs)[*used]).is_expired()) {
						return &(*objs)[(*used)++];
					}
					(*used)++;
				}
			} else {
				if (objs->size() != (*used)) {
					return &(*objs)[(*used)++];
				}
			}

			objs->push_back(TInst());
			return &(*objs)[(*used)++];
		}

		void reset_counter(double delta, int custom_type_of_buffer = 0) {
			if (instant.size() && used_instant < (instant.size() * 0.5)) {
				time_used_less_then_half_of_instant_pool -= delta;
				if (time_used_less_then_half_of_instant_pool <= 0) {
					time_used_less_then_half_of_instant_pool = TIME_USED_TO_SHRINK_INSTANT;

					DEV_PRINT_STD("Shrinking instant buffer for %s. From %d, to %d. Buffer type: %d\n", typeid(TInst).name(), instant.size(), used_instant, custom_type_of_buffer);

					instant.resize(used_instant);
				}
			} else {
				time_used_less_then_half_of_instant_pool = TIME_USED_TO_SHRINK_INSTANT;
			}

			used_instant = 0;
			_prev_not_expired_delayed = 0;

			if (delayed.size() && used_delayed < (delayed.size() * 0.25)) {
				time_used_less_then_quarter_of_delayed_pool -= delta;
				if (time_used_less_then_quarter_of_delayed_pool <= 0) {
					time_used_less_then_quarter_of_delayed_pool = TIME_USED_TO_SHRINK_DELAYED;

					DEV_PRINT_STD("Shrinking _delayed_ buffer for %s. From %d, to %d. Buffer type: %d\n", typeid(TInst).name(), delayed.size(), used_delayed, custom_type_of_buffer);

					std::sort(delayed.begin(), delayed.end(), [](const TInst &a, const TInst &b) { return (int)a.is_expired() < (int)b.is_expired(); });
					delayed.resize(used_delayed);
				}
			} else {
				time_used_less_then_quarter_of_delayed_pool = TIME_USED_TO_SHRINK_DELAYED;
			}
		}

		void reset_visible_counter() {
			visible_objects = 0;
		}

		void clear_pools() {
			instant.clear();
			delayed.clear();
			used_instant = 0;
			used_delayed = 0;
			_prev_used_instant = 0;
			_prev_not_expired_delayed = 0;
			time_used_less_then_half_of_instant_pool = 0;

			reset_visible_counter();
		}
	};

	ObjectsPool<DelayedRendererInstance2D> instances[(int)InstanceType2D::ALL] = {};

	uint64_t time_spent_to_fill_buffers_of_instances = 0;
	uint64_t time_spent_to_fill_buffers_of_lines = 0;
	uint64_t time_spent_to_cull_instant = 0;
	uint64_t time_spent_to_cull_delayed = 0;

public:
	GeometryPool2D() {}

	~GeometryPool2D() {
	}

	void reset_counter(double _delta);
	void reset_visible_objects();
	Ref<DebugDrawStats2D> get_stats() const;
	void clear_pool();
	void for_each_instance(const std::function<void(DelayedRendererInstance2D *)> &_func);
	void update_visibility(const std::vector<std::vector<Plane> > &t_frustums, const GeometryPoolDistanceCullingData2D &t_distance_data);
	void update_expiration(double _delta);
	void scan_visible_instances();
};

class GeometryContainer2D {
	friend class DebugDraw2D;
	class DebugDraw2D *owner;

	GeometryPool2D geometry_pool;
	Node *scene_world_node = nullptr;
	int32_t render_layers = 1;

	std::recursive_mutex datalock;

public:
	GeometryContainer2D(class DebugDraw2D *root);
	~GeometryContainer2D();

	void update_geometry(double delta);

	void set_render_layer_mask(int32_t layers);
	int32_t get_render_layer_mask() const;

	Ref<DebugDrawStats2D> get_render_stats();
	void clear_2d_objects();
};

#endif