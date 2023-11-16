#include "geometry_generators.h"
#include "utils/utils.h"
using namespace godot;

#pragma region Predefined Geometry Parts

const std::array<Vector3, 8> GeometryGenerator::CenteredCubeVertices{
	Vector3(-0.5f, -0.5f, -0.5f),
	Vector3(0.5f, -0.5f, -0.5f),
	Vector3(0.5f, -0.5f, 0.5f),
	Vector3(-0.5f, -0.5f, 0.5f),
	Vector3(-0.5f, 0.5f, -0.5f),
	Vector3(0.5f, 0.5f, -0.5f),
	Vector3(0.5f, 0.5f, 0.5f),
	Vector3(-0.5f, 0.5f, 0.5f)
};

const std::array<Vector3, 8> GeometryGenerator::CubeVertices{
	Vector3(0, 0, 0),
	Vector3(1, 0, 0),
	Vector3(1, 0, 1),
	Vector3(0, 0, 1),
	Vector3(0, 1, 0),
	Vector3(1, 1, 0),
	Vector3(1, 1, 1),
	Vector3(0, 1, 1)
};

const std::array<int, 24> GeometryGenerator::CubeIndices{
	0,
	1,
	1,
	2,
	2,
	3,
	3,
	0,

	4,
	5,
	5,
	6,
	6,
	7,
	7,
	4,

	0,
	4,
	1,
	5,
	2,
	6,
	3,
	7,
};

const std::array<int, 36> GeometryGenerator::CubeWithDiagonalsIndices{
	0, 1,
	1, 2,
	2, 3,
	3, 0,

	4, 5,
	5, 6,
	6, 7,
	7, 4,

	0, 4,
	1, 5,
	2, 6,
	3, 7,

	// Diagonals

	// Top Bottom
	1, 3,
	// 0, 2,
	4, 6,
	// 5, 7,

	// Front Back
	1, 4,
	// 0, 5,
	3, 6,
	// 2, 7,

	// Left Right
	3, 4,
	// 0, 7,
	1, 6,
	// 2, 5,
};

const std::array<Vector3, 6> GeometryGenerator::ArrowheadVertices{
	Vector3(0, 0, -1),
	Vector3(0, 0.25f, 0),
	Vector3(0, -0.25f, 0),
	Vector3(0.25f, 0, 0),
	Vector3(-0.25f, 0, 0),
	// Cross to center
	Vector3(0, 0, -0.2f),
};

const std::array<int, 18> GeometryGenerator::ArrowheadIndices{
	0,
	1,
	0,
	2,
	0,
	3,
	0,
	4,
	// Center line
	5,
	0,
	// Cross
	// 1, 2,
	// 3, 4,
	// Or Cross to center
	5,
	1,
	5,
	2,
	5,
	3,
	5,
	4,
};

const std::array<Vector3, 4> GeometryGenerator::CenteredSquareVertices{
	Vector3(0.5f, 0.5f, 0),
	Vector3(0.5f, -0.5f, 0),
	Vector3(-0.5f, -0.5f, 0),
	Vector3(-0.5f, 0.5f, 0),
};

const std::array<int, 6> GeometryGenerator::SquareIndices{
	0,
	1,
	2,
	2,
	3,
	0,
};

const std::array<Vector3, 6> GeometryGenerator::PositionVertices{
	Vector3(0.5f, 0, 0),
	Vector3(-0.5f, 0, 0),
	Vector3(0, 0.5f, 0),
	Vector3(0, -0.5f, 0),
	Vector3(0, 0, 0.5f),
	Vector3(0, 0, -0.5f),
};

const std::array<int, 6> GeometryGenerator::PositionIndices{
	0,
	1,
	2,
	3,
	4,
	5,
};

#pragma endregion

Ref<ArrayMesh> GeometryGenerator::CreateMesh(Mesh::PrimitiveType type, const PackedVector3Array &vertices, const PackedInt32Array &indices, const PackedColorArray &colors, const PackedVector3Array &normals, const PackedFloat32Array &custom0, BitField<Mesh::ArrayFormat> flags) {
	Ref<ArrayMesh> mesh;
	mesh.instantiate();
	Array a;
	a.resize((int)ArrayMesh::ArrayType::ARRAY_MAX);

	a[(int)ArrayMesh::ArrayType::ARRAY_VERTEX] = vertices;
	if (indices.size())
		a[(int)ArrayMesh::ArrayType::ARRAY_INDEX] = indices;
	if (colors.size())
		a[(int)ArrayMesh::ArrayType::ARRAY_COLOR] = colors;
	if (normals.size())
		a[(int)ArrayMesh::ArrayType::ARRAY_NORMAL] = normals;
	if (custom0.size())
		a[(int)ArrayMesh::ArrayType::ARRAY_CUSTOM0] = custom0;

	mesh->add_surface_from_arrays(type, a, Array(), Dictionary(), flags);

	return mesh;
}

Ref<ArrayMesh> GeometryGenerator::ConvertWireframeToVolumetric(Ref<ArrayMesh> mesh) {
	Array arrs = mesh->surface_get_arrays(0);
	PackedVector3Array vertexes = arrs[ArrayMesh::ArrayType::ARRAY_VERTEX];
	PackedVector3Array normals = arrs[ArrayMesh::ArrayType::ARRAY_NORMAL];
	PackedInt32Array indexes = arrs[ArrayMesh::ArrayType::ARRAY_INDEX];

	ERR_FAIL_COND_V(vertexes.size() % 2 != 0, Ref<ArrayMesh>());
	// TODO re-enable
	// ERR_FAIL_COND_V(vertexes.size() != normals.size(), Ref<ArrayMesh>());

	PackedVector3Array res_vertexes;
	PackedVector3Array res_custom0;
	PackedInt32Array res_indexes;

	if (indexes.size()) {
		for (int i = 0; i < indexes.size(); i += 2) {
			GenerateVolumetricSegment(vertexes[indexes[i]], vertexes[indexes[i + 1]], true ? Vector3(0, 1, 0.0001) : normals[indexes[i]], res_vertexes, res_custom0, res_indexes);
		}
	} else {
		for (int i = 0; i < vertexes.size(); i += 2) {
			GenerateVolumetricSegment(vertexes[i], vertexes[i + 1], true ? Vector3(0, 1, 0.0001) : normals[i], res_vertexes, res_custom0, res_indexes);
		}
	}

	return CreateMesh(
			Mesh::PRIMITIVE_TRIANGLES,
			res_vertexes,
			res_indexes,
			PackedColorArray(),
			PackedVector3Array(),
			Utils::convert_packed_array_to_diffrent_types<PackedFloat32Array>(res_custom0),
			ArrayMesh::ARRAY_CUSTOM_RGB_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT);
}

void GeometryGenerator::GenerateVolumetricSegment(Vector3 a, Vector3 b, Vector3 normal_up, PackedVector3Array &vertexes, PackedVector3Array &custom0, PackedInt32Array &indexes) {
	bool debug_size = false;
	Vector3 dir = (b - a).normalized();

	Vector3 right = dir.cross(normal_up.rotated(dir, Math::deg_to_rad(45.0f))).normalized();
	Vector3 left = right * -1;
	Vector3 up = dir.cross(left).normalized();
	Vector3 down = up * -1;

	Vector3 h_right = right * 0.5;
	Vector3 h_left = left * 0.5;
	Vector3 h_up = up * 0.5;
	Vector3 h_down = down * 0.5;

	Vector3 mult = debug_size ? Vector3(1, 1, 1) * 0.5 : Vector3();

	int64_t base = vertexes.size();
	// Start X
	vertexes.push_back(a + right * mult);
	vertexes.push_back(a + left * mult);
	vertexes.push_back(a + up * mult);
	vertexes.push_back(a + down * mult);
	// End X
	vertexes.push_back(b + right * mult);
	vertexes.push_back(b + left * mult);
	vertexes.push_back(b + up * mult);
	vertexes.push_back(b + down * mult);

	// Horizontal plane
	indexes.append(base + 0);
	indexes.append(base + 1);
	indexes.append(base + 4);
	indexes.append(base + 1);
	indexes.append(base + 5);
	indexes.append(base + 4);
	// Vertical plane
	indexes.append(base + 2);
	indexes.append(base + 3);
	indexes.append(base + 6);
	indexes.append(base + 3);
	indexes.append(base + 7);
	indexes.append(base + 6);

	// Start cap
	indexes.append(base + 0);
	indexes.append(base + 1);
	indexes.append(base + 2);
	indexes.append(base + 0);
	indexes.append(base + 3);
	indexes.append(base + 1);
	// End cap
	indexes.append(base + 4);
	indexes.append(base + 6);
	indexes.append(base + 5);
	indexes.append(base + 4);
	indexes.append(base + 5);
	indexes.append(base + 7);

	indexes.append(2);
	indexes.append(3);
	indexes.append(6);
	indexes.append(3);
	indexes.append(7);
	indexes.append(6);

	custom0.push_back(h_right);
	custom0.push_back(h_left);
	custom0.push_back(h_up);
	custom0.push_back(h_down);

	custom0.push_back(h_right);
	custom0.push_back(h_left);
	custom0.push_back(h_up);
	custom0.push_back(h_down);
}

std::vector<Vector3> GeometryGenerator::CreateCameraFrustumLines(const std::array<Plane, 6> &frustum) {
	std::vector<Vector3> res;
	res.resize(CubeIndices.size());

	std::function<Vector3(const Plane &, const Plane &, const Plane &)> intersect_planes = [&](const Plane &a, const Plane &b, const Plane &c) {
		Vector3 intersec_result;
		a.intersect_3(b, c, &intersec_result);
		return intersec_result;
	};

	//  near, far, left, top, right, bottom
	//  0,    1,   2,    3,   4,     5
	Vector3 cube[]{
		intersect_planes(frustum[0], frustum[3], frustum[2]),
		intersect_planes(frustum[0], frustum[3], frustum[4]),
		intersect_planes(frustum[0], frustum[5], frustum[4]),
		intersect_planes(frustum[0], frustum[5], frustum[2]),

		intersect_planes(frustum[1], frustum[3], frustum[2]),
		intersect_planes(frustum[1], frustum[3], frustum[4]),
		intersect_planes(frustum[1], frustum[5], frustum[4]),
		intersect_planes(frustum[1], frustum[5], frustum[2]),
	};

	for (int i = 0; i < res.size(); i++)
		res[i] = cube[CubeIndices[i]];

	return res;
}

std::vector<Vector3> GeometryGenerator::CreateSphereLines(const int &_lats, const int &_lons, const float &radius, const Vector3 &position) {
	int lats = _lats;
	int lons = _lons;

	if (lats < 2)
		lats = 2;
	if (lons < 4)
		lons = 4;

	std::vector<Vector3> res;
	res.resize((size_t)lats * (size_t)lons * 6);

	int total = 0;
	for (int i = 1; i <= lats; i++) {
		float lat0 = (float)Math_PI * (-0.5f + (float)(i - 1) / lats);
		float z0 = sin(lat0);
		float zr0 = cos(lat0);

		float lat1 = (float)Math_PI * (-0.5f + (float)i / lats);
		float z1 = sin(lat1);
		float zr1 = cos(lat1);

		for (int j = lons; j >= 1; j--) {
			float lng0 = (float)Math_TAU * (j - 1) / lons;
			float x0 = cos(lng0);
			float y0 = sin(lng0);

			float lng1 = (float)Math_TAU * j / lons;
			float x1 = cos(lng1);
			float y1 = sin(lng1);

			Vector3 v[4]{
				Vector3(x1 * zr0, z0, y1 * zr0) * radius + position,
				Vector3(x1 * zr1, z1, y1 * zr1) * radius + position,
				Vector3(x0 * zr1, z1, y0 * zr1) * radius + position,
				Vector3(x0 * zr0, z0, y0 * zr0) * radius + position
			};

			res[total++] = v[0];
			res[total++] = v[1];
			res[total++] = v[2];

			res[total++] = v[2];
			res[total++] = v[3];
			res[total++] = v[0];
		}
	}
	return res;
}

std::vector<Vector3> GeometryGenerator::CreateCylinderLines(const int &edges, const float &radius, const float &height, const Vector3 &position, const int &draw_edge_each_n_step) {
	real_t angle = 360.f / edges;

	std::vector<Vector3> points;
	points.reserve(4 * (size_t)edges + (((size_t)edges / (size_t)draw_edge_each_n_step) * 2));

	Vector3 d = Vector3(0, height, 0);
	for (int i = 0; i < edges; i++) {
		float ra = Math::deg_to_rad(i * angle);
		float rb = Math::deg_to_rad((i + 1) * angle);
		Vector3 a = Vector3(sin(ra), 0, cos(ra)) * radius + position;
		Vector3 b = Vector3(sin(rb), 0, cos(rb)) * radius + position;

		// Top
		points.push_back(a + d);
		points.push_back(b + d);

		// Bottom
		points.push_back(a - d);
		points.push_back(b - d);

		// Edge
		if (i % draw_edge_each_n_step == 0) {
			points.push_back(a + d);
			points.push_back(a - d);
		}
	}

	return points;
}

std::vector<Vector3> GeometryGenerator::CreateLinesFromPath(const PackedVector3Array &path) {
	std::vector<Vector3> res;
	res.resize(((size_t)path.size() - 1) * 2);

	for (size_t i = 0; i < (size_t)path.size() - 1; i++) {
		res[i * 2] = path[(int)i];
		res[i * 2 + 1] = path[(int)i + 1];
	}

	return res;
}