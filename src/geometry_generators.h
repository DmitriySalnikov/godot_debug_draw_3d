#pragma once

#include "utils_compiler.h"

MSVC_WARNING_DISABLE(4244)
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/builtin_types.hpp>
MSVC_WARNING_RESTORE(4244)

#include <array>
#include <vector>

using namespace godot;

class GeometryGenerator {
public:
#pragma region Predefined Geometry Parts

	const static std::array<Vector3, 8> CenteredCubeVertices;
	const static std::array<Vector3, 8> CubeVertices;
	const static std::array<int, 24> CubeIndices;
	const static std::array<int, 36> CubeWithDiagonalsIndices;
	const static std::array<Vector3, 6> ArrowheadVertices;
	const static std::array<int, 18> ArrowheadIndices;
	const static std::array<Vector3, 4> CenteredSquareVertices;
	const static std::array<int, 6> SquareIndices;
	const static std::array<Vector3, 6> PositionVertices;
	const static std::array<int, 6> PositionIndices;

#pragma endregion

	static std::vector<Vector3> CreateCameraFrustumLines(const std::array<Plane, 6> &frustum);
	static std::vector<Vector3> CreateCubeLines(const Vector3 &position, const Quaternion &rotation, const Vector3 &size, const bool &centered_box = true, const bool &with_diagonals = false);
	static std::vector<Vector3> CreateSphereLines(const int &_lats, const int &_lons, const float &radius, const Vector3 &position);
	static std::vector<Vector3> CreateCylinderLines(const int &edges, const float &radius, const float &height, const Vector3 &position, const int &draw_edge_each_n_step = 1);
	static std::vector<Vector3> CreateLinesFromPath(const PackedVector3Array &path);
};
