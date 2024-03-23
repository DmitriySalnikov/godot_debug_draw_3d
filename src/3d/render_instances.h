#pragma once

#ifndef DISABLE_DEBUG_RENDERING

#include "3d/config_scope_3d.h"
#include "utils/math_utils.h"
#include "utils/utils.h"

#include <array>
#include <functional>
#include <unordered_set>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/array_mesh.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

namespace godot {
class MultiMesh;
}
class DebugDraw3DStats;
class GeometryPool;

// TODO RENAME ALL AGAIN!!!!

enum class GeometryType : char {
	Wireframe,
	Volumetric,
	Solid,
};

enum class ConvertableInstanceType : char {
	CUBE,
	CUBE_CENTERED,
	ARROWHEAD,
	POSITION,
	SPHERE,
	CYLINDER,
	CYLINDER_AB,
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
	CYLINDER_AB,

	// Volumetric from wireframes
	LINE_VOLUMETRIC,
	CUBE_VOLUMETRIC,
	CUBE_CENTERED_VOLUMETRIC,
	ARROWHEAD_VOLUMETRIC,
	POSITION_VOLUMETRIC,
	SPHERE_VOLUMETRIC,
	SPHERE_HD_VOLUMETRIC,
	CYLINDER_VOLUMETRIC,
	CYLINDER_AB_VOLUMETRIC,

	// Solid geometry
	BILLBOARD_SQUARE,
	PLANE,

	MAX,
};

enum class ProcessType : char {
	PROCESS,
	PHYSICS_PROCESS,
	MAX,
};

class GeometryPoolCullingData {
public:
	std::vector<std::array<Plane, 6> > m_frustums;
	std::vector<AABBMinMax> m_frustum_boxes;
	GeometryPoolCullingData(const std::vector<std::array<Plane, 6> > &p_frustums, const std::vector<AABBMinMax> p_frustum_boxes) {
		m_frustums = p_frustums;
		m_frustum_boxes = p_frustum_boxes;
	}
};

struct GeometryPoolData3DInstance {
	Vector3 basis_x;
	float origin_x;
	Vector3 basis_y;
	float origin_y;
	Vector3 basis_z;
	float origin_z;
	Color color;
	Color custom;

	GeometryPoolData3DInstance() :
			basis_x(Vector3()),
			origin_x(0),
			basis_y(Vector3()),
			origin_y(0),
			basis_z(Vector3()),
			origin_z(0),
			color(Color()),
			custom(Color()) {}

	GeometryPoolData3DInstance(const Transform3D &p_xf, const Color &p_color, const Color &p_custom) :
			basis_x(p_xf.basis[0]),
			origin_x(p_xf.origin.x),
			basis_y(p_xf.basis[1]),
			origin_y(p_xf.origin.y),
			basis_z(p_xf.basis[2]),
			origin_z(p_xf.origin.z),
			color(p_color),
			custom(p_custom) {}
};

struct DelayedRenderer {
	double expiration_time;
	bool is_used_one_time;
	bool is_visible;
	AABBMinMax bounds;

	DelayedRenderer() :
			bounds(),
			expiration_time(0),
			is_used_one_time(true),
			is_visible(false) {}

	inline bool is_expired() const {
		return expiration_time < 0 ? is_used_one_time : false;
	}

	inline void update_expiration(const double &_delta) {
		if (!is_expired()) {
			expiration_time -= _delta;
		}
	}

	inline bool update_visibility(const std::shared_ptr<GeometryPoolCullingData> &p_culling_data) {
		is_visible = false;
		for (auto &box : p_culling_data->m_frustum_boxes) {
			if (box.intersects(bounds)) {
				goto frustum;
			}
		}
		return is_visible = false;
	frustum:
		if (p_culling_data->m_frustums.size()) {
			for (auto &frustum : p_culling_data->m_frustums) {
				if (MathUtils::is_bounds_partially_inside_convex_shape(bounds, frustum)) {
					return is_visible = true;
				}
			}
			return is_visible = false;
		} else {
			return is_visible = true;
		}
	}
};

struct DelayedRendererInstance : public DelayedRenderer {
	GeometryPoolData3DInstance data;

	DelayedRendererInstance();
};

struct DelayedRendererLine : public DelayedRenderer {
	std::unique_ptr<Vector3[]> lines;
	size_t lines_count;
	Color color;

	DelayedRendererLine();
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

		size_t used_instant = 0;
		size_t used_delayed = 0;
		size_t _prev_used_instant = 0;
		size_t _prev_not_expired_delayed = 0;
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

			_prev_used_instant = used_instant;
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

		void clear_pools() {
			instant.clear();
			delayed.clear();
			used_instant = 0;
			used_delayed = 0;
			_prev_used_instant = 0;
			_prev_not_expired_delayed = 0;
			time_used_less_then_half_of_instant_pool = 0;
		}
	};

	struct vpCacheData {
		double exp_time;
		uint64_t vp_id;
		bool is_phys_used = false;
	};

	struct processTypePools {
		ObjectsPool<DelayedRendererInstance> instances[(int)InstanceType::MAX];
		ObjectsPool<DelayedRendererLine> lines;
		std::unordered_map<Viewport *, vpCacheData> viewports;
	};

	processTypePools pools[(int)ProcessType::MAX];

	PackedFloat32Array temp_instances_buffers[(int)InstanceType::MAX];

	uint64_t stat_visible_instances = 0;
	uint64_t stat_visible_lines = 0;
	int64_t time_spent_to_fill_buffers_of_instances = 0;
	int64_t time_spent_to_fill_buffers_of_lines = 0;
	int64_t time_spent_to_cull_instances = 0;
	int64_t time_spent_to_cull_lines = 0;

	// Internal use of raw pointer to avoid ref/unref
	Color _scoped_config_to_custom(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg);
	InstanceType _scoped_config_type_convert(ConvertableInstanceType type, const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg);
	GeometryType _scoped_config_get_geometry_type(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg);

public:
	GeometryPool() {}

	~GeometryPool() {
	}

	std::unordered_set<Viewport *> get_and_validate_viewports();

	void fill_instance_data(const std::vector<Ref<MultiMesh> *> &p_meshes, const std::shared_ptr<GeometryPoolCullingData> &p_culling_data, const double &delta);
	void fill_lines_data(Ref<ArrayMesh> _ig, const std::shared_ptr<GeometryPoolCullingData> &p_culling_data, const double &delta);
	void reset_counter(const double &_delta, const ProcessType &p_proc = ProcessType::MAX);
	void reset_visible_objects();
	void set_stats(Ref<DebugDraw3DStats> &stats) const;
	void clear_pool();
	void for_each_instance(const std::function<void(DelayedRendererInstance *)> &_func);
	void for_each_line(const std::function<void(DelayedRendererLine *)> &_func);
	void update_expiration(const double &_delta, const ProcessType &p_proc);
	void add_or_update_instance(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg, ConvertableInstanceType _type, const real_t &_exp_time, const ProcessType &p_proc, const Transform3D &_transform, const Color &_col, const SphereBounds &_bounds, const Color *p_custom_col = nullptr);
	void add_or_update_instance(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg, InstanceType _type, const real_t &_exp_time, const ProcessType &p_proc, const Transform3D &_transform, const Color &_col, const SphereBounds &_bounds, const Color *p_custom_col = nullptr);
	void add_or_update_line(const std::shared_ptr<DebugDraw3DScopeConfig::Data> &cfg, const real_t &_exp_time, const ProcessType &p_proc, std::unique_ptr<Vector3[]> _lines, const size_t _line_count, const Color &_col);
};

#endif