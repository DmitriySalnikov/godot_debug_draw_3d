#pragma once

#include "math_utils.h"
#include "utils.h"

#include <Godot.hpp>
#include <ImmediateGeometry.hpp>

#define INSTANCE_DATA_SIZE (sizeof(Transform) + sizeof(Color))
#define INSTANCE_DATA_FLOAT_COUNT ((sizeof(Transform) + sizeof(Color)) / sizeof(real_t))

using namespace godot;

enum InstanceType : char {
	CUBES,
	CUBES_CENTERED,
	ARROWHEADS,
	BILLBOARD_SQUARES,
	POSITIONS,
	SPHERES,
	CYLINDERS,
	ALL,
};

class DelayedRenderer {
protected:
	void _update(real_t exp_time, bool is_vis);

public:
	real_t expiration_time = 0;
	bool IsUsedOneTime = false;
	bool IsDelayedForOneFrame = false;
	bool IsVisible = true;

	DelayedRenderer();
	bool IsExpired() const;
};

class DelayedRendererInstance : public DelayedRenderer {
public:
	Transform transform;
	Color color;
	InstanceType type;
	SphereBounds bounds;

	DelayedRendererInstance();
	void update(real_t _exp_time, bool _is_visible, InstanceType _type, Transform _transform, Color _col, SphereBounds _bounds);
};

class DelayedRendererLine : public DelayedRenderer {
	std::vector<Vector3> _lines;

public:
	AABB bounds;
	Color LinesColor;

	DelayedRendererLine();
	void update(real_t exp_time, bool is_visible, const std::vector<Vector3> &lines, Color col);

	void set_lines(std::vector<Vector3> lines);
	std::vector<Vector3> get_lines();
	AABB CalculateBoundsBasedOnLines(std::vector<Vector3> &lines);
};

class GeometryPool {
private:
	template <class TInst>
	struct ObjectsPool {
		const real_t TIME_USED_TO_SHRINK_INSTANT = 10; // 10 sec
		const real_t TIME_USED_TO_SHRINK_DELAYED = 30; // 30 sec

		std::vector<TInst> instant = {};
		std::vector<TInst> delayed = {};
		int visible_objects = 0;
		size_t used_instant = 0;
		size_t prev_not_expired_delayed = 0;
		size_t used_delayed = 0;
		real_t time_used_less_then_half_of_instant_pool = 0;
		real_t time_used_less_then_quarter_of_delayed_pool = 0;

		ObjectsPool() {
			time_used_less_then_half_of_instant_pool = TIME_USED_TO_SHRINK_INSTANT;
			time_used_less_then_quarter_of_delayed_pool = TIME_USED_TO_SHRINK_DELAYED;
		}

		TInst *get(bool is_delayed) {
			auto objs = is_delayed ? &delayed : &instant;
			auto used = is_delayed ? &prev_not_expired_delayed : &used_instant;

			if (is_delayed) {
				while (objs->size() != (*used)) {
					if (((*objs)[*used]).IsExpired()) {
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

		void reset_counter(real_t delta, int custom_type_of_buffer = 0) {
			if (instant.size() && used_instant < (instant.size() * 0.5)) {
				time_used_less_then_half_of_instant_pool -= delta;
				if (time_used_less_then_half_of_instant_pool <= 0) {
					time_used_less_then_half_of_instant_pool = TIME_USED_TO_SHRINK_INSTANT;

#ifdef _DEBUG
					std::wstring w = std::wstring(L"Shrinking instant buffer for ") + String(typeid(TInst).name()).unicode_str() + L". From %d, to %d. Buffer type: %d\n";
					DEBUG_PRINT_STD(w.c_str(), instant.size(), used_instant, custom_type_of_buffer);
#endif
					// instant.resize((size_t)(instant.size() * 0.5));
					instant.resize(used_instant);
				}
			} else {
				time_used_less_then_half_of_instant_pool = TIME_USED_TO_SHRINK_INSTANT;
			}

			used_instant = 0;
			prev_not_expired_delayed = 0;

			if (delayed.size() && used_delayed < (delayed.size() * 0.25)) {
				time_used_less_then_quarter_of_delayed_pool -= delta;
				if (time_used_less_then_quarter_of_delayed_pool <= 0) {
					time_used_less_then_quarter_of_delayed_pool = TIME_USED_TO_SHRINK_DELAYED;

#ifdef _DEBUG
					std::wstring w = std::wstring(L"Shrinking _delayed_ buffer for ") + String(typeid(TInst).name()).unicode_str() + L". From %d, to %d. Buffer type: %d\n";
					DEBUG_PRINT_STD(w.c_str(), delayed.size(), used_delayed, custom_type_of_buffer);
#endif
					std::sort(delayed.begin(), delayed.end(), [](const TInst &a, const TInst &b) { return (int)a.IsExpired() < (int)b.IsExpired(); });
					// delayed.resize((size_t)(delayed.size() * 0.25));
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
			prev_not_expired_delayed = 0;
			time_used_less_then_half_of_instant_pool = 0;

			reset_visible_counter();
		}
	};

	ObjectsPool<DelayedRendererInstance> instances[InstanceType::ALL] = {};
	ObjectsPool<DelayedRendererLine> lines;

public:
	GeometryPool() {}

	~GeometryPool() {
	}

	PoolRealArray get_raw_data(InstanceType type);
	void fill_lines_data(ImmediateGeometry *ig);
	void reset_counter(real_t delta);
	void reset_visible_objects();
	size_t get_visible_instances();
	size_t get_visible_lines();
	size_t get_used_instances_total();
	size_t get_used_instances_instant(InstanceType type);
	size_t get_used_instances_delayed(InstanceType type);
	size_t get_used_lines_total();
	size_t get_used_lines_instant();
	size_t get_used_lines_delayed();
	void clear_pool();
	void for_each_instance(std::function<void(DelayedRendererInstance *)> func);
	void for_each_line(std::function<void(DelayedRendererLine *)> func);
	void update_visibility(std::vector<Plane> frustum, real_t delta);
	void rescan_visible_instances();
	void add_or_update_instance(InstanceType _type, real_t _exp_time, bool _is_visible, Transform _transform, Color _col, SphereBounds _bounds, std::function<void(DelayedRendererInstance *)> custom_upd = nullptr);
	void add_or_update_line(real_t _exp_time, bool _is_visible, std::vector<Vector3> _lines, Color _col, std::function<void(DelayedRendererLine *)> custom_upd = nullptr);
};
