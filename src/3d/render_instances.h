#pragma once

#ifndef DISABLE_DEBUG_RENDERING

#include "utils/math_utils.h"
#include "utils/utils.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/array_mesh.hpp>
GODOT_WARNING_RESTORE()

#include <array>
#include <functional>
#include <queue>

namespace godot {
class MultiMesh;
}
class DebugDrawStats3D;

using namespace godot;

enum class InstanceType : char {
	Cube,
	CubeCentered,
	ArrowHead,
	BillboardSquare,
	Position,
	Sphere,
	SphereHD,
	Cylinder,
	ALL,
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

class DelayedRenderer {
protected:
	void _update(double exp_time, bool is_vis);

public:
	double expiration_time = 0;
	bool is_used_one_time = false;
	bool is_visible = true;

	DelayedRenderer() :
			expiration_time(0),
			is_used_one_time(true),
			is_visible(false) {}

	bool is_expired() const;

	void update_expiration(double delta);
};

class DelayedRenderer3D : public DelayedRenderer {
protected:
	template <class TBounds>
	_FORCE_INLINE_ bool update_visibility_internal(TBounds bounds, const std::vector<std::vector<Plane> > &t_frustums, const GeometryPoolDistanceCullingData &t_distance_data, bool _skip_expiration_check) {
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

public:
	virtual bool update_visibility(const std::vector<std::vector<Plane> > &t_frustums, const GeometryPoolDistanceCullingData &t_distance_data, bool _skip_expiration_check) = 0;

	DelayedRenderer3D() :
			DelayedRenderer() {}
};

class DelayedRendererInstance : public DelayedRenderer3D {
public:
	Transform3D transform;
	InstanceType type = InstanceType::Cube;
	Color color;
	SphereBounds bounds;

	DelayedRendererInstance();
	void update(real_t _exp_time, const InstanceType &_type, const Transform3D &_transform, const Color &_col, const SphereBounds &_bounds);
	virtual bool update_visibility(const std::vector<std::vector<Plane> > &t_frustums, const GeometryPoolDistanceCullingData &t_distance_data, bool _skip_expiration_check) override;
};

class DelayedRendererLine : public DelayedRenderer3D {
	std::vector<Vector3> lines;
	Color color;
	AABB bounds;

public:
	DelayedRendererLine();
	void update(real_t _exp_time, const std::vector<Vector3> &_lines, const Color &_col);

	void set_lines(const std::vector<Vector3> &_lines);
	const std::vector<Vector3> &get_lines() const;
	AABB calculate_bounds_based_on_lines(const std::vector<Vector3> &_lines);
	virtual bool update_visibility(const std::vector<std::vector<Plane> > &t_frustums, const GeometryPoolDistanceCullingData &t_distance_data, bool _skip_expiration_check) override;
};

class GeometryPool {
private:
	template <class TInst, size_t InitialSize = 256>
	struct ObjectsPool {
		const real_t TIME_USED_TO_SHRINK_INSTANT = 10; // 10 sec

		std::queue<std::shared_ptr<TInst> > buffer;

		int64_t taken_instances = 0;
		size_t used_instant = 0;
		size_t _prev_used_instant = 0;
		double time_used_less_then_half_of_instant_pool = 0;

		ObjectsPool() {
			for (size_t i = 0; i < InitialSize; i++) {
				buffer.push(std::make_shared<TInst>());
			}

			time_used_less_then_half_of_instant_pool = TIME_USED_TO_SHRINK_INSTANT;
		}

		~ObjectsPool() {
			if (taken_instances > 0) {
				DEV_PRINT_STD("%s<%s> created more objects than were returned.", NAMEOF(ObjectsPool), typeid(TInst).name());
			}
			else if (taken_instances > 0) {
				DEV_PRINT_STD("%s<%s> got more objects than it created.", NAMEOF(ObjectsPool), typeid(TInst).name());
			}
		}

		std::shared_ptr<TInst> get() {
			taken_instances++;

			if (!buffer.empty()) {
				auto v = buffer.front();
				buffer.pop();
				return v;
			}

			return std::make_shared<TInst>();
		}

		void push_back(std::shared_ptr<TInst> inst) {
			taken_instances--;
			buffer.push(inst);
		}

		void reset_counter(double delta, int custom_type_of_buffer = 0) {
			// TODO
			return;

			if (buffer.size() && used_instant < (buffer.size() * 0.5)) {
				time_used_less_then_half_of_instant_pool -= delta;
				if (time_used_less_then_half_of_instant_pool <= 0) {
					time_used_less_then_half_of_instant_pool = TIME_USED_TO_SHRINK_INSTANT;

					DEV_PRINT_STD("Shrinking instant buffer for %s. From %d, to %d. Buffer type: %d\n", typeid(TInst).name(), buffer.size(), used_instant, custom_type_of_buffer);

					while (buffer.size() > used_instant) {
						buffer.pop();
					}
				}
			} else {
				time_used_less_then_half_of_instant_pool = TIME_USED_TO_SHRINK_INSTANT;
			}

			used_instant = 0;
		}

		void clear_pools() {
			while (!buffer.empty()) {
				buffer.pop();
			}
			used_instant = 0;
			_prev_used_instant = 0;
			time_used_less_then_half_of_instant_pool = 0;
		}
	};

	template<class TInst>
	class RenderingObjectPoolContainer {
		ObjectsPool<DelayedRendererInstance> instances;
		ObjectsPool<DelayedRendererInstance> instant;
		ObjectsPool<DelayedRendererInstance> delayed;

	};

	ObjectsPool<DelayedRendererInstance> instances[(int)InstanceType::ALL] = {};
	ObjectsPool<DelayedRendererLine> lines;

	uint64_t time_spent_to_fill_buffers_of_instances = 0;
	uint64_t time_spent_to_fill_buffers_of_lines = 0;
	uint64_t time_spent_to_cull_instant = 0;
	uint64_t time_spent_to_cull_delayed = 0;

	PackedFloat32Array get_raw_data(InstanceType _type);

public:
	GeometryPool() {}

	~GeometryPool() {
	}

	void fill_instance_data(const std::array<Ref<MultiMesh> *, (int)InstanceType::ALL> &t_meshes);
	void fill_lines_data(Ref<ArrayMesh> _ig);
	void reset_counter(double _delta);
	void reset_visible_objects();
	Ref<DebugDrawStats3D> get_stats() const;
	void clear_pool();
	void for_each_instance(const std::function<void(std::shared_ptr<DelayedRendererInstance>)> &_func);
	void for_each_line(const std::function<void(std::shared_ptr<DelayedRendererLine>)> &_func);
	void update_visibility(const std::vector<std::vector<Plane> > &t_frustums, const GeometryPoolDistanceCullingData &t_distance_data);
	void update_expiration(double _delta);
	void scan_visible_instances();
	void add_or_update_instance(InstanceType _type, real_t _exp_time, const Transform3D &_transform, const Color &_col, const SphereBounds &_bounds, const std::function<void(std::shared_ptr<DelayedRendererInstance>)> &_custom_upd = nullptr);
	void add_or_update_line(real_t _exp_time, const std::vector<Vector3> &_lines, const Color &_col, const std::function<void(std::shared_ptr<DelayedRendererLine>)> _custom_upd = nullptr);
};

#endif