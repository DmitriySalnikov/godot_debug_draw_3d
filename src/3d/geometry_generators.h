#pragma once

#include "utils/compiler.h"
#include "utils/utils.h"

#include <array>
#include <vector>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/array_mesh.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class GeometryGenerator {
private:
	static void GenerateVolumetricSegment(const Vector3 &a, const Vector3 &b, const Vector3 &normal_up, PackedVector3Array &vertexes, PackedVector3Array &custom0, PackedInt32Array &indexes, const bool &add_caps = true);

public:
#pragma region Predefined Geometry Parts

	const static std::array<Vector3, 2> LineVertices;

	const static std::array<Vector3, 8> CenteredCubeVertices;
	const static std::array<Vector3, 8> CubeVertices;
	const static std::array<int, 24> CubeIndices;
	const static std::array<int, 36> CubeWithDiagonalsIndices;
	const static std::array<Vector3, 6> ArrowheadVertices;
	const static std::array<int, 18> ArrowheadIndices;
	const static std::array<int, 18> ArrowheadIndicesSimplified;
	const static std::array<Vector3, 4> CenteredSquareVertices;
	const static std::array<int, 6> SquareIndices;
	const static std::array<Vector3, 6> PositionVertices;
	const static std::array<int, 6> PositionIndices;

#pragma endregion

	template <class TVertices, class TIndices = std::array<int, 0>, class TColors = std::array<Color, 0>, class TNormal = std::array<Vector3, 0>, class TCustom0 = std::array<Vector3, 0> >
	static Ref<ArrayMesh> CreateMesh(Mesh::PrimitiveType type, const TVertices &vertices, const TIndices &indices = {}, const TColors &colors = {}, const TNormal &normals = {}, const TCustom0 &custom0 = {}, BitField<Mesh::ArrayFormat> flags = 0) {
		return CreateMesh(type,
				Utils::convert_to_packed_array<PackedVector3Array>(vertices),
				Utils::convert_to_packed_array<PackedInt32Array>(indices),
				Utils::convert_to_packed_array<PackedColorArray>(colors),
				Utils::convert_to_packed_array<PackedVector3Array>(normals),
				Utils::convert_to_packed_array_diffrent_types<PackedFloat32Array>(custom0),
				flags);
	}

	static Ref<ArrayMesh> CreateMesh(Mesh::PrimitiveType type, const PackedVector3Array &vertices, const PackedInt32Array &indices = {}, const PackedColorArray &colors = {}, const PackedVector3Array &normals = {}, const PackedFloat32Array &custom0 = {}, BitField<Mesh::ArrayFormat> flags = 0);

	static Ref<ArrayMesh> ConvertWireframeToVolumetric(Ref<ArrayMesh> mesh, const bool &add_caps = true);

	static std::vector<Vector3> CreateCameraFrustumLines(const std::array<Plane, 6> &frustum);
	// TODO like in cylinder add more lines and draw_edge_each_n_step to make them more precise
	static std::vector<Vector3> CreateSphereLines(const int &_lats, const int &_lons, const float &radius, const Vector3 &position, const int &subdivide = 1);
	static std::vector<Vector3> CreateCylinderLines(const int &edges, const float &radius, const float &height, const Vector3 &position, const int &subdivide = 1);
	static std::vector<Vector3> CreateLinesFromPath(const PackedVector3Array &path);
};
