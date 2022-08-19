#pragma once

#include <Godot.hpp>

#include <vector>

using namespace godot;

class MathUtils {

public:
	static const float CubeRadiusForSphere;
	static const float CylinderRadiusForSphere;
	static const float AxisRadiusForSphere;
	static const float ArrowRadiusForSphere;

	static void get_diagonal_vectors(Vector3 a, Vector3 b, Vector3 &bottom, Vector3 &top, Vector3 &diag);
	static bool is_bounds_partially_inside_convex_shape(AABB bounds, std::vector<Plane> &planes);
	static bool is_bounds_partially_inside_convex_shape(class SphereBounds sphere, std::vector<Plane> planes);
	static real_t get_max_value(Vector3 &value);
	static real_t get_max_vector_length(Vector3 &a, Vector3 &b, Vector3 &c);
	static real_t get_max_basis_length(Basis &b);
};

/// May not work for non-local space transformations
class SphereBounds {
public:
	Vector3 Position;
	real_t Radius = 1;

	SphereBounds() :
			Position(),
			Radius() {}

	SphereBounds(Vector3 pos, real_t radius) :
			Position(pos),
			Radius(radius) {}
};
