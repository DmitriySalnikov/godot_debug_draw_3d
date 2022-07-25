#pragma once

#include <Godot.hpp>

#include <vector>

using namespace godot;

class MathUtils {

public:
	static void get_diagonal_vectors(Vector3 a, Vector3 b, Vector3 &bottom, Vector3 &top, Vector3 &diag);
	static bool is_bounds_partially_inside_convex_shape(AABB bounds, std::vector<Plane> &planes);
	static bool is_bounds_partially_inside_convex_shape(AABB bounds, Array &planes);
	static bool is_bounds_partially_inside_convex_shape(class SphereBounds sphere, std::vector<Plane> planes);
	static bool is_bounds_partially_inside_convex_shape(class SphereBounds sphere, Array planes);
	static float get_max_value(Vector3 &value);
};

class SphereBounds {
public:
	Vector3 Position;
	float Radius = 1;

	SphereBounds() :
			Position(),
			Radius() {}

	SphereBounds(Vector3 pos, float radius) :
			Position(pos),
			Radius(radius) {}
};
