#pragma once

#include "debug_draw.h"
#include "math_utils.h"
#include "utils.h"

#include <Godot.hpp>
#include <Mutex.hpp>

#include <chrono>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <vector>

using namespace godot;

class DelayedText;

class TextGroup {
public:
	String Title;
	int GroupPriority;
	Color GroupColor;
	bool ShowTitle;
	std::map<String, DelayedText> Texts;

	TextGroup(String title, int priority, bool showTitle, Color groupColor);
	void CleanTexts(std::function<void()> update);
};

class DelayedText {
public:
	std::chrono::milliseconds ExpirationTime;
	String Text;
	int Priority;
	Color ValueColor = Utils::empty_color;
	DebugDraw3D *owner_debug_draw = nullptr;

	DelayedText(std::chrono::milliseconds expirationTime, String text, int priority, Color color, DebugDraw3D *owner);
	bool IsExpired();
};

class DelayedRenderer {
public:
	std::chrono::milliseconds ExpirationTime;
	bool IsUsedOneTime = false;
	bool IsVisible = true;

	bool IsExpired() {
		return ((TIME_NOW() - ExpirationTime).time_since_epoch().count() > 0 && IsUsedOneTime);
	}

	void Returned() {
		IsUsedOneTime = false;
		IsVisible = true;
	}
};

class DelayedRendererInstance : DelayedRenderer {
public:
	Transform InstanceTransform;
	Color InstanceColor;
	SphereBounds Bounds;
};

class DelayedRendererLine : DelayedRenderer {
	std::vector<Vector3> _lines;

public:
	AABB Bounds;
	Color LinesColor;

	void set_lines(std::vector<Vector3> lines) {
		_lines = lines;
		Bounds = CalculateBoundsBasedOnLines(_lines);
	}

	std::vector<Vector3> get_lines() {
		return _lines;
	}

	AABB CalculateBoundsBasedOnLines(std::vector<Vector3> &lines) {
		if (lines.size() > 0) {
			// Using the original Godot expand_to code to avoid creating new AABB instances
			Vector3 begin = lines[0];
			Vector3 end = lines[0] + Vector3::ZERO;

			for (Vector3 v : lines) {
				if (v.x < begin.x) {
					begin.x = v.x;
				}
				if (v.y < begin.y) {
					begin.y = v.y;
				}
				if (v.z < begin.z) {
					begin.z = v.z;
				}

				if (v.x > end.x) {
					end.x = v.x;
				}
				if (v.y > end.y) {
					end.y = v.y;
				}
				if (v.z > end.z) {
					end.z = v.z;
				}
			}

			return AABB(begin, end - begin);
		} else {
			return AABB();
		}
	}
};

// https://docs.microsoft.com/en-gb/dotnet/standard/collections/thread-safe/how-to-create-an-object-pool
template <class T = DelayedRenderer>
class ObjectPool {
private:
	std::queue<T> _objects;
	std::function<T()> _objectGenerator;
	std::mutex _lock;

public:
	ObjectPool(std::function<T()> objectGenerator) {
		_objectGenerator = objectGenerator;
	}

	T Get() {
		LOCK_METHOD(_lock);

		if (_objects.size() > 0) {
			T res = _objects.front();
			_objects.pop();
			return res;
		} else {
			return _objectGenerator();
		}
	};

	void Return(T item) {
		LOCK_METHOD(_lock);
		_objects.push(item);
		item.Returned();
	}
};
