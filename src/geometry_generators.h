#pragma once

#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#endif

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/builtin_types.hpp>

#if defined(_MSC_VER)
#pragma warning(default : 4244)
#endif

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

	static std::vector<Vector3> CreateCameraFrustumLines(std::array<Plane, 6> frustum);
	static std::vector<Vector3> CreateCubeLines(Vector3 position, Quaternion rotation, Vector3 size, bool centered_box = true, bool with_diagonals = false);
	static std::vector<Vector3> CreateSphereLines(int lats, int lons, float radius, Vector3 position);
	static std::vector<Vector3> CreateCylinderLines(int edges, float radius, float height, Vector3 position, int draw_edge_each_n_step = 1);
	static std::vector<Vector3> CreateLinesFromPath(PackedVector3Array path);
};
