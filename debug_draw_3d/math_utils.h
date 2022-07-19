#pragma once

#include <Godot.hpp>

#include <vector>

using namespace godot;

class MathUtils {

public:
	static void GetDiagonalVectors(Vector3 a, Vector3 b, Vector3 &bottom, Vector3 &top, Vector3 &diag);
	static bool BoundsPartiallyInsideConvexShape(AABB bounds, std::vector<Plane> &planes);
	static bool BoundsPartiallyInsideConvexShape(AABB bounds, Array &planes);
	static bool BoundsPartiallyInsideConvexShape(class SphereBounds sphere, std::vector<Plane> planes);
	static bool BoundsPartiallyInsideConvexShape(class SphereBounds sphere, Array planes);
	static float GetMaxValue(Vector3 &value);
};

class SphereBounds {
public:
	Vector3 Position;
	float Radius;
};
