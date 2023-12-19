#pragma once

#ifndef DISABLE_DEBUG_RENDERING

#include "utils/math_utils.h"
#include "utils/utils.h"

#include <array>
#include <functional>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/array_mesh.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

namespace godot {
class MultiMesh;
}
class DebugDraw3DStats;
class GeometryPool;

enum class ConvertableInstanceType : char {
	CUBE,
	CUBE_CENTERED,
	ARROWHEAD,
	POSITION,
	SPHERE,
	SPHERE_HD,
	CYLINDER,
};

enum class InstanceType : char {
	// Basic wireframe
	CUBE,
	CUBE_CENTERED,
	ARROWHEAD,
	POSITION,
	SPHERE,
	SPHERE_HD,
	CYLINDER,

	// Volumetric from wireframes
	LINE_VOLUMETRIC,
	CUBE_VOLUMETRIC,
	CUBE_CENTERED_VOLUMETRIC,
	ARROWHEAD_VOLUMETRIC,
	POSITION_VOLUMETRIC,
	SPHERE_VOLUMETRIC,
	SPHERE_HD_VOLUMETRIC,
	CYLINDER_VOLUMETRIC,

	// Solid geometry
	BILLBOARD_SQUARE,

	MAX,
};

enum class ProcessType : char {
	PROCESS,
	PHYSICS_PROCESS,
	MAX,
};

class GeometryPoolDistanceCullingData {
public:
	real_t m_max_distance;
	std::vector<Vector3> m_camera_positions;
	GeometryPoolDistanceCullingData(const real_t &t_max_distance, const std::vector<Vector3> &t_camera_positions) {
		m_max_distance = t_max_distance;
		m_camera_positions = t_camera_positions;
	}
};

template <class TBounds>
class DelayedRenderer {
protected:
	void _update(const double &exp_time, const bool &is_vis) {
		ZoneScoped;
		expiration_time = exp_time;
		is_used_one_time = false;
		is_visible = is_vis;
	}

public:
	double expiration_time;
	bool is_used_one_time;
	bool is_visible;
	TBounds bounds;

	DelayedRenderer() :
			bounds(),
			expiration_time(0),
			is_used_one_time(true),
			is_visible(false) {}

	bool is_expired() const {
		return expiration_time < 0 ? is_used_one_time : false;
	}

	bool update_visibility(const std::vector<std::vector<Plane> > &t_frustums, const GeometryPoolDistanceCullingData &t_distance_data, bool _skip_expiration_check) {
		ZoneScoped;
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

			// TODO mb move to draw_* for instant draws
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

	void update_expiration(const double &_delta) {
		if (!is_expired()) {
			expiration_time -= _delta;
		}
	}
};

class InstanceTransformAndData3D {
	Vector3 basis_x;
	float origin_x;
	Vector3 basis_y;
	float origin_y;
	Vector3 basis_z;
	float origin_z;
	Color color;
	Color custom;

public:
	InstanceTransformAndData3D() :
			basis_x(Vector3()),
			origin_x(0),
			basis_y(Vector3()),
			origin_y(0),
			basis_z(Vector3()),
			origin_z(0),
			color(Color()),
			custom(Color()) {}

	InstanceTransformAndData3D(const Transform3D &p_xf, const Color &p_color, const Color &p_custom) :
			basis_x(p_xf.basis[0]),
			origin_x(p_xf.origin.x),
			basis_y(p_xf.basis[1]),
			origin_y(p_xf.origin.y),
			basis_z(p_xf.basis[2]),
			origin_z(p_xf.origin.z),
			color(p_color),
			custom(p_custom) {}
};

class DelayedRendererInstance : public DelayedRenderer<SphereBounds> {
public:
	InstanceTransformAndData3D data;

	DelayedRendererInstance();
	void update(const real_t &_exp_time, const Transform3D &_transform, const Color &_col, const Color &_custom_col, const SphereBounds &_bounds);
};

class DelayedRendererLine : public DelayedRenderer<AABB> {
	std::unique_ptr<Vector3[]> lines;
	size_t lines_count;

public:
	Color color;
	Color customColor;

	DelayedRendererLine();
	void update(const real_t &_exp_time, std::unique_ptr<Vector3[]> _lines, const size_t _lines_count, const Color &_col);

	const Vector3 *get_lines() const;
	const size_t get_lines_count() const;
	AABB calculate_bounds_based_on_lines();
};

class GeometryPool {
private:
	enum ShrinkTimers : char {
		TIME_USED_TO_SHRINK_TEMP_BUFFER = 5,
		TIME_USED_TO_SHRINK_INSTANT = 10,
		TIME_USED_TO_SHRINK_DELAYED = 15,
	};

	template <class TInst>
	struct ObjectsPool {
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
			ZoneScoped;
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
			ZoneScoped;
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

	template <class T, class TSlice, size_t _TStepSize>
	struct TempBigBuffer {
	private:
		std::unique_ptr<T[]> m_buffer;
		size_t current_size;
		double m_shrink_timer;

		void resize(size_t new_size) {
			m_buffer.reset(new T[new_size]);
			current_size = new_size;
		}

	public:
		TempBigBuffer() {
			resize(_TStepSize);
			m_shrink_timer = TIME_USED_TO_SHRINK_TEMP_BUFFER;
		}

		void prepare_buffer(size_t t_expected_size) {
			ZoneScoped;
			if (current_size < t_expected_size) {
				size_t new_size = (size_t)Math::ceil(t_expected_size / (double)_TStepSize) * _TStepSize;
				DEV_PRINT_STD(NAMEOF(TempBigBuffer) ": extending from %d to %d\n", current_size, new_size);
				resize(new_size);
				m_shrink_timer = TIME_USED_TO_SHRINK_TEMP_BUFFER;
			}
		}

		void update(size_t max_expected_size, const double &delta) {
			size_t new_size = (size_t)Math::ceil(((double)current_size / 2) / _TStepSize) * _TStepSize;
			if (max_expected_size <= new_size) {
				m_shrink_timer -= delta;
				if (m_shrink_timer < 0) {
					if (new_size != current_size) {
						DEV_PRINT_STD(NAMEOF(TempBigBuffer) ": shrinking from %d to %d\n", current_size, new_size);
						resize(new_size);
						m_shrink_timer = TIME_USED_TO_SHRINK_TEMP_BUFFER;
					}
				}
			} else {
				m_shrink_timer = TIME_USED_TO_SHRINK_TEMP_BUFFER;
			}
		}

		inline auto ptrw() {
			return m_buffer.get();
		}

		// TODO stupid and slow
		inline auto slice(int64_t begin, int64_t end = 2147483647) {
			ZoneScoped;
			TSlice res;
			end = Math::min(end, (int64_t)current_size);
			res.resize(end - begin);
			memcpy(res.ptrw(), m_buffer.get(), (end - begin) * sizeof(T));
			return res;
		}
	};

	struct {
		ObjectsPool<DelayedRendererInstance> instances[(int)InstanceType::MAX];
		ObjectsPool<DelayedRendererLine> lines;
	} pools[(int)ProcessType::MAX];

	uint64_t time_spent_to_fill_buffers_of_instances = 0;
	uint64_t time_spent_to_fill_buffers_of_lines = 0;
	uint64_t time_spent_to_cull_instant = 0;
	uint64_t time_spent_to_cull_delayed = 0;

	typedef TempBigBuffer<float, PackedFloat32Array, 128 * 1020> temp_raw_buffer;
	PackedFloat32Array get_raw_data(InstanceType _type, temp_raw_buffer &buffer, size_t &out_buffer_size);

public:
	GeometryPool() {}

	~GeometryPool() {
	}

	void fill_instance_data(const std::array<Ref<MultiMesh> *, (int)InstanceType::MAX> &t_meshes, const double &delta);
	void fill_lines_data(Ref<ArrayMesh> _ig, const double &delta);
	void reset_counter(const double &_delta, const ProcessType &p_proc = ProcessType::MAX);
	void reset_visible_objects();
	void update_stats(Ref<DebugDraw3DStats> &stats) const;
	void clear_pool();
	void for_each_instance(const std::function<void(DelayedRendererInstance *)> &_func);
	void for_each_line(const std::function<void(DelayedRendererLine *)> &_func);
	void update_visibility(const std::vector<std::vector<Plane> > &t_frustums, const GeometryPoolDistanceCullingData &t_distance_data);
	void update_expiration(const double &_delta, const ProcessType &p_proc);
	void scan_visible_instances();
	void add_or_update_instance(InstanceType _type, const real_t &_exp_time, const ProcessType &p_proc, const Transform3D &_transform, const Color &_col, const Color &_custom_col, const SphereBounds &_bounds, const std::function<void(DelayedRendererInstance *)> &_custom_upd = nullptr);
	void add_or_update_line(const real_t &_exp_time, const ProcessType &p_proc, std::unique_ptr<Vector3[]> _lines, const size_t _line_count, const Color &_col, const std::function<void(DelayedRendererLine *)> _custom_upd = nullptr);
};

#endif