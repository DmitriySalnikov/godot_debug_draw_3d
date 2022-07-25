#pragma once

#include "math_utils.h"
#include "utils.h"

#include <Godot.hpp>

#include <queue>

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
	void _update(int exp_time, bool is_vis);

public:
	int ms_expiration = 0;
	std::chrono::steady_clock::time_point updated_time;
	bool IsUsedOneTime = false;
	bool IsVisible = true;

	bool IsExpired();
	void Returned();
};

class DelayedRendererInstance : public DelayedRenderer {
public:
	InstanceType type;
	Transform transform;
	Color color;
	SphereBounds bounds;

	DelayedRendererInstance() ;
	void update(int _exp_time, bool _is_visible, InstanceType _type, Transform _transform, Color _col, SphereBounds _bounds);
};

class DelayedRendererLine : public DelayedRenderer {
	std::vector<Vector3> _lines;

public:
	AABB bounds;
	Color LinesColor;

	DelayedRendererLine();
	void update(int exp_time, bool is_visible, const std::vector<Vector3> &lines, Color col);

	void set_lines(std::vector<Vector3> lines);
	std::vector<Vector3> get_lines();
	AABB CalculateBoundsBasedOnLines(std::vector<Vector3> &lines);
};

template <class T = DelayedRenderer>
class GeometryPool {
private:
	std::vector<T> objects;
	size_t used_objects;
	int times_used_less_then_half_of_pool = 0;

public:
	GeometryPool() :
			objects(),
			used_objects(0) {
	}

	~GeometryPool() {
	}

	void reset_counter() {
		// shrink pool if too many unused objects
		if (used_objects < ceil(objects.size() / 2.0)) {
			times_used_less_then_half_of_pool++;

			if (times_used_less_then_half_of_pool > 25) {
				times_used_less_then_half_of_pool = 0;
				objects.resize(objects.size() / 2);
			}
		} else {
			times_used_less_then_half_of_pool = 0;
		}

		used_objects = 0;
	}

	size_t get_used_objects() {
		return used_objects;
	}

	size_t get_pool_size() {
		return objects.size();
	}

	void resize_pool(size_t new_size) {
		objects.resize(new_size);
	}

	void clear_pool() {
		objects.clear();
	}

	void for_each(std::function<void(T *)> func) {
		for (size_t i = 0; i < used_objects; i++) {
			func(&objects[i]);
		}
	}

	T *get() {
		while (objects.size() != used_objects) {
			if (objects[used_objects].IsExpired()) {
				return &objects[used_objects++];
			}
			used_objects++;
		}

		if (objects.size() == used_objects) {
			objects.push_back(T());
			return &objects[used_objects++];
		} else {
			return &objects[used_objects++];
		}
	};
};
