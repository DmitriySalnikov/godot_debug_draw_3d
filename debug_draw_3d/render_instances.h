#pragma once

#include "math_utils.h"
#include "utils.h"

#include <Godot.hpp>

#include <queue>

using namespace godot;

class DelayedRenderer {
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
	Transform InstanceTransform;
	Color InstanceColor;
	SphereBounds Bounds;
};

class DelayedRendererLine : public DelayedRenderer {
	std::vector<Vector3> _lines;

public:
	AABB Bounds;
	Color LinesColor;

	void set_lines(std::vector<Vector3> lines);
	std::vector<Vector3> get_lines();
	AABB CalculateBoundsBasedOnLines(std::vector<Vector3> &lines);
};

// https://docs.microsoft.com/en-gb/dotnet/standard/collections/thread-safe/how-to-create-an-object-pool
template <class T = DelayedRenderer *>
class ObjectPool {
private:
	std::queue<T *> objects_queue;
	std::function<T *()> object_create;
	std::mutex _lock;

public:
	ObjectPool(const std::function<T *()> &objectGenerator) :
			objects_queue(),
			object_create(objectGenerator),
			_lock{} {
	}

	~ObjectPool() {
		while (objects_queue.size() > 0) {
			delete objects_queue.front();
			objects_queue.pop();
		}
	}

	T *Get() {
		LOCK_GUARD(_lock);

		if (objects_queue.size() > 0) {
			T *res = objects_queue.front();
			objects_queue.pop();
			return res;
		} else {
			return object_create();
		}
	};

	void Return(T *item) {
		LOCK_GUARD(_lock);
		objects_queue.push(item);
		item->Returned();
	}
};
