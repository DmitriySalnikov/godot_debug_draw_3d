#pragma once

#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#endif

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/builtin_types.hpp>

#if defined(_MSC_VER)
#pragma warning(default : 4244)
#endif

#include <vector>

using namespace godot;

class MathUtils {

public:
	static const float CubeRadiusForSphere;
	static const float CylinderRadiusForSphere;
	static const float AxisRadiusForSphere;
	static const float ArrowRadiusForSphere;

	static void get_diagonal_vectors(const Vector3 &a, const Vector3 &b, Vector3 &bottom, Vector3 &top, Vector3 &diag);
	static bool is_bounds_partially_inside_convex_shape(const AABB &bounds, const std::vector<Plane> &planes);
	static bool is_bounds_partially_inside_convex_shape(const class SphereBounds &sphere, const std::vector<Plane> &planes);
	static real_t get_max_value(const Vector3 &value);
	static real_t get_max_vector_length(const Vector3 &a, const Vector3 &b, const Vector3 &c);
	static real_t get_max_basis_length(const Basis &b);
};

/// May not work for non-local space transformations
class SphereBounds {
public:
	Vector3 position;
	real_t Radius = 1;

	SphereBounds() :
			position(),
			Radius() {}

	SphereBounds(const Vector3 &_pos, const real_t &_radius) :
			position(_pos),
			Radius(_radius) {}
};
