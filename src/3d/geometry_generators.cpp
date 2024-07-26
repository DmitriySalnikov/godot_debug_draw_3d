#include "geometry_generators.h"
#include "utils/math_utils.h"
#include "utils/utils.h"

#include <vector>

using namespace godot;

#pragma region Predefined Geometry Parts

const std::array<Vector3, 8> GeometryGenerator::CenteredCubeVertexes{
	Vector3(-0.5f, -0.5f, -0.5f),
	Vector3(0.5f, -0.5f, -0.5f),
	Vector3(0.5f, -0.5f, 0.5f),
	Vector3(-0.5f, -0.5f, 0.5f),
	Vector3(-0.5f, 0.5f, -0.5f),
	Vector3(0.5f, 0.5f, -0.5f),
	Vector3(0.5f, 0.5f, 0.5f),
	Vector3(-0.5f, 0.5f, 0.5f)
};

const std::array<Vector3, 8> GeometryGenerator::CubeVertexes{
	Vector3(0, 0, 0), // 0
	Vector3(1, 0, 0), // 1
	Vector3(1, 0, 1), // 2
	Vector3(0, 0, 1), // 3
	Vector3(0, 1, 0), // 4
	Vector3(1, 1, 0), // 5
	Vector3(1, 1, 1), // 6
	Vector3(0, 1, 1) //  7
};

const std::array<int, 24> GeometryGenerator::CubeIndexes{
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
	3, 7
};

const std::array<int, 36> GeometryGenerator::CubeWithDiagonalsIndexes{
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

const std::array<Vector3, 2> GeometryGenerator::LineVertexes{
	Vector3(0, 0, 0),
	Vector3(0, 0, -1),
};

const std::array<Vector3, 6> GeometryGenerator::ArrowheadVertexes{
	Vector3(0, 0, 0),
	Vector3(0, 0.25f, 1),
	Vector3(0, -0.25f, 1),
	Vector3(0.25f, 0, 1),
	Vector3(-0.25f, 0, 1),
	// Cross to center
	Vector3(0, 0, 0.8f),
};

const std::array<int, 18> GeometryGenerator::ArrowheadIndexes{
	0, 1,
	0, 2,
	0, 3,
	0, 4,
	// Center line
	5, 0,
	// Cross
	// 1, 2,
	// 3, 4,
	// Or Cross to center
	5, 1,
	5, 2,
	5, 3,
	5, 4
};

const std::array<int, 8> GeometryGenerator::ArrowheadIndexesSimplified{
	0, 1,
	0, 2,
	0, 3,
	0, 4
};

const std::array<Vector3, 4> GeometryGenerator::CenteredSquareVertexes{
	Vector3(0.5f, 0.5f, 0),
	Vector3(0.5f, -0.5f, 0),
	Vector3(-0.5f, -0.5f, 0),
	Vector3(-0.5f, 0.5f, 0),
};

const std::array<int, 6> GeometryGenerator::SquareBackwardsIndexes{
	0, 1,
	2, 2,
	3, 0
};

const std::array<int, 6> GeometryGenerator::SquareIndexes{
	0, 2,
	1, 2,
	0, 3
};

const std::array<Vector3, 6> GeometryGenerator::PositionVertexes{
	Vector3(0.5f, 0, 0),
	Vector3(-0.5f, 0, 0),
	Vector3(0, 0.5f, 0),
	Vector3(0, -0.5f, 0),
	Vector3(0, 0, 0.5f),
	Vector3(0, 0, -0.5f),
};

const std::array<int, 6> GeometryGenerator::PositionIndexes{
	0, 1,
	2, 3,
	4, 5
};

#pragma endregion

Ref<ArrayMesh> GeometryGenerator::CreateMesh(Mesh::PrimitiveType type, const PackedVector3Array &vertexes, const PackedInt32Array &indexes, const PackedColorArray &colors, const PackedVector3Array &normals, const PackedVector2Array &uv, const PackedFloat32Array &custom0, BitField<Mesh::ArrayFormat> flags) {
	ZoneScoped;
	Ref<ArrayMesh> mesh;
	mesh.instantiate();
	Array a;
	a.resize((int)ArrayMesh::ArrayType::ARRAY_MAX);

	a[(int)ArrayMesh::ArrayType::ARRAY_VERTEX] = vertexes;
	if (indexes.size())
		a[(int)ArrayMesh::ArrayType::ARRAY_INDEX] = indexes;
	if (colors.size())
		a[(int)ArrayMesh::ArrayType::ARRAY_COLOR] = colors;
	if (normals.size())
		a[(int)ArrayMesh::ArrayType::ARRAY_NORMAL] = normals;
	if (uv.size())
		a[(int)ArrayMesh::ArrayType::ARRAY_TEX_UV] = uv;
	if (custom0.size())
		a[(int)ArrayMesh::ArrayType::ARRAY_CUSTOM0] = custom0;

	mesh->add_surface_from_arrays(type, a, Array(), Dictionary(), flags);

	return mesh;
}

Ref<ArrayMesh> GeometryGenerator::RotatedMesh(const Ref<ArrayMesh> mesh, const Vector3 &axis, const real_t &angle) {
	Array arrs = mesh->surface_get_arrays(0);
	Mesh::PrimitiveType ptype = mesh->surface_get_primitive_type(0);
	ERR_FAIL_COND_V(arrs.size() == 0, mesh);

	PackedVector3Array vertexes = arrs[ArrayMesh::ArrayType::ARRAY_VERTEX];
	PackedVector3Array normals = arrs[ArrayMesh::ArrayType::ARRAY_NORMAL];
	PackedFloat32Array custom0 = arrs[ArrayMesh::ArrayType::ARRAY_CUSTOM0];

	auto rotate_vec3 = [&axis, &angle](PackedVector3Array &arr) {
		for (auto &v : arr) {
			v.rotate(axis, angle);
		}
	};

	auto rotate_f32 = [&axis, &angle](PackedFloat32Array &arr) {
		for (int i = 0; i < arr.size(); i += 3) {
			Vector3 v = Vector3(arr[i + 0], arr[i + 1], arr[i + 2]).rotated(axis, angle);
			arr[i + 0] = v.x;
			arr[i + 1] = v.y;
			arr[i + 2] = v.z;
		}
	};

	rotate_vec3(vertexes);
	rotate_vec3(normals);
	rotate_f32(custom0);

	return CreateMesh(
			ptype,
			vertexes,
			arrs[ArrayMesh::ArrayType::ARRAY_INDEX],
			arrs[ArrayMesh::ArrayType::ARRAY_COLOR],
			normals,
			arrs[ArrayMesh::ArrayType::ARRAY_TEX_UV],
			custom0,
			ArrayMesh::ARRAY_CUSTOM_RGB_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT);
}

Ref<ArrayMesh> GeometryGenerator::ConvertWireframeToVolumetric(Ref<ArrayMesh> mesh, const bool &add_bevel, const bool &add_caps) {
	ZoneScoped;
	Array arrs = mesh->surface_get_arrays(0);
	ERR_FAIL_COND_V(arrs.size() == 0, mesh);

	PackedVector3Array vertexes = arrs[ArrayMesh::ArrayType::ARRAY_VERTEX];
	PackedVector3Array normals = arrs[ArrayMesh::ArrayType::ARRAY_NORMAL];
	PackedInt32Array indexes = arrs[ArrayMesh::ArrayType::ARRAY_INDEX];

	bool has_indexes = indexes.size();
	bool has_normals = normals.size();

	ERR_FAIL_COND_V(!has_indexes && vertexes.size() % 2 != 0, Ref<ArrayMesh>());
	ERR_FAIL_COND_V(has_indexes && indexes.size() % 2 != 0, Ref<ArrayMesh>());
	ERR_FAIL_COND_V(normals.size() && vertexes.size() != normals.size(), Ref<ArrayMesh>());

	PackedVector3Array res_vertexes;
	PackedVector3Array res_custom0;
	PackedInt32Array res_indexes;
	PackedVector2Array res_uv;

	if (has_indexes) {
		for (int i = 0; i < indexes.size(); i += 2) {
			Vector3 normal_a = has_normals ? normals[indexes[i]] : Vector3(0, 1, 0.0001f);

			if (add_bevel)
				GenerateVolumetricSegmentBevel(vertexes[indexes[i]], vertexes[indexes[i + 1]], normal_a, res_vertexes, res_custom0, res_indexes, res_uv, add_caps);
			else
				GenerateVolumetricSegment(vertexes[indexes[i]], vertexes[indexes[i + 1]], normal_a, res_vertexes, res_custom0, res_indexes, res_uv, add_caps);
		}
	} else {
		for (int i = 0; i < vertexes.size(); i += 2) {
			Vector3 normal_a = has_normals ? normals[i] : Vector3(0, 1, 0.0001f);

			if (add_bevel)
				GenerateVolumetricSegmentBevel(vertexes[i], vertexes[i + 1], normal_a, res_vertexes, res_custom0, res_indexes, res_uv, add_caps);
			else
				GenerateVolumetricSegment(vertexes[i], vertexes[i + 1], normal_a, res_vertexes, res_custom0, res_indexes, res_uv, add_caps);
		}
	}

	return CreateMesh(
			Mesh::PRIMITIVE_TRIANGLES,
			res_vertexes,
			res_indexes,
			PackedColorArray(),
			PackedVector3Array(),
			res_uv,
			Utils::convert_packed_array_to_diffrent_types<PackedFloat32Array>(res_custom0),
			ArrayMesh::ARRAY_CUSTOM_RGB_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT);
}

void GeometryGenerator::GenerateVolumetricSegment(const Vector3 &a, const Vector3 &b, const Vector3 &normal, PackedVector3Array &vertexes, PackedVector3Array &custom0, PackedInt32Array &indexes, PackedVector2Array &uv, const bool &add_caps) {
	ZoneScoped;
	bool debug_size = false;
	Vector3 debug_mult = debug_size ? Vector3_ONE * 0.5 : Vector3();
	Vector3 dir = (b - a).normalized();
	int64_t base_idx = vertexes.size();

	auto add_side = [&dir, &vertexes, &indexes, &custom0, &uv, &debug_mult](Vector3 pos_a, Vector3 pos_b, Vector3 normal, bool is_rotated) {
		int64_t start_idx = vertexes.size();

		Vector3 right_a = dir.cross(normal.rotated(dir, Math::deg_to_rad(is_rotated ? -45.f : 45.f))).normalized();
		Vector3 left_a = right_a * -1;

		Vector3 right_b = dir.cross(normal.rotated(dir, Math::deg_to_rad(is_rotated ? -45.f : 45.f))).normalized();
		Vector3 left_b = right_b * -1;

		right_a /= MathUtils::Sqrt2;
		left_a /= MathUtils::Sqrt2;
		right_b /= MathUtils::Sqrt2;
		left_b /= MathUtils::Sqrt2;

		vertexes.push_back(pos_a + right_a * debug_mult); // 0
		vertexes.push_back(pos_a + left_a * debug_mult); // 1
		vertexes.push_back(pos_b + right_b * debug_mult); // 2
		vertexes.push_back(pos_b + left_b * debug_mult); // 3

		indexes.append(start_idx + 0);
		indexes.append(start_idx + 1);
		indexes.append(start_idx + 2);

		indexes.append(start_idx + 1);
		indexes.append(start_idx + 3);
		indexes.append(start_idx + 2);

		if (is_rotated) {
			uv.push_back(Vector2(0, 0));
			uv.push_back(Vector2(1, 1));
			uv.push_back(Vector2(0, 0));
			uv.push_back(Vector2(1, 1));
		} else {
			uv.push_back(Vector2(1, 0));
			uv.push_back(Vector2(0, 1));
			uv.push_back(Vector2(1, 0));
			uv.push_back(Vector2(0, 1));
		}

		custom0.push_back(right_a);
		custom0.push_back(left_a);
		custom0.push_back(right_b);
		custom0.push_back(left_b);
	};

	add_side(a, b, normal, false);
	add_side(a, b, normal, true);

	if (add_caps) {
		// Start cap
		indexes.append(base_idx + 0);
		indexes.append(base_idx + 4);
		indexes.append(base_idx + 1);
		indexes.append(base_idx + 1);
		indexes.append(base_idx + 5);
		indexes.append(base_idx + 0);

		// End cap
		indexes.append(base_idx + 2);
		indexes.append(base_idx + 6);
		indexes.append(base_idx + 3);
		indexes.append(base_idx + 3);
		indexes.append(base_idx + 7);
		indexes.append(base_idx + 2);
	}
}

void GeometryGenerator::GenerateVolumetricSegmentBevel(const Vector3 &a, const Vector3 &b, const Vector3 &normal, PackedVector3Array &vertexes, PackedVector3Array &custom0, PackedInt32Array &indexes, PackedVector2Array &uv, const bool &add_caps) {
	ZoneScoped;
	bool debug_size = false;
	Vector3 debug_mult = debug_size ? Vector3_ONE * 0.5f : Vector3();
	real_t half_len = .5f;
	// real_t half_len = Math::clamp(len * .5f, .0f, 0.5f);
	Vector3 dir = (b - a).normalized();
	int64_t base_idx = vertexes.size();

	vertexes.push_back(a); // 0
	vertexes.push_back(b); // 1

	uv.push_back(Vector2(.5f, .5f));
	uv.push_back(Vector2(.5f, .5f));

	custom0.push_back(Vector3_ZERO);
	custom0.push_back(Vector3_ZERO);

	auto add_side = [&half_len, &dir, &base_idx, &vertexes, &indexes, &custom0, &uv, &debug_mult](Vector3 pos_a, Vector3 pos_b, Vector3 normal, real_t angle) {
		int64_t start_idx = vertexes.size();

		Vector3 right_a = dir.cross(normal.rotated(dir, Math::deg_to_rad(angle))).normalized();
		Vector3 left_a = right_a * -1;

		Vector3 right_b = dir.cross(normal.rotated(dir, Math::deg_to_rad(angle))).normalized();
		Vector3 left_b = right_b * -1;

		right_a /= MathUtils::Sqrt2;
		left_a /= MathUtils::Sqrt2;
		right_b /= MathUtils::Sqrt2;
		left_b /= MathUtils::Sqrt2;

		right_a += dir * half_len;
		left_a += dir * half_len;
		right_b -= dir * half_len;
		left_b -= dir * half_len;

		vertexes.push_back(pos_a + right_a * debug_mult); // global 2, local 0
		vertexes.push_back(pos_a + left_a * debug_mult); // global 3, local 1
		vertexes.push_back(pos_b + right_b * debug_mult); // global 4, local 2
		vertexes.push_back(pos_b + left_b * debug_mult); // global 5, local 3

		indexes.append(base_idx + 0);
		indexes.append(start_idx + 0);
		indexes.append(start_idx + 1);

		indexes.append(start_idx + 0);
		indexes.append(start_idx + 2);
		indexes.append(start_idx + 1);

		indexes.append(start_idx + 1);
		indexes.append(start_idx + 3);
		indexes.append(start_idx + 2);

		indexes.append(start_idx + 2);
		indexes.append(base_idx + 1);
		indexes.append(start_idx + 3);

		uv.push_back(Vector2(1, 1));
		uv.push_back(Vector2(0, 0));
		uv.push_back(Vector2(1, 1));
		uv.push_back(Vector2(0, 0));

		custom0.push_back(right_a);
		custom0.push_back(left_a);
		custom0.push_back(right_b);
		custom0.push_back(left_b);
	};

	add_side(a, b, normal, 45.f);
	add_side(a, b, normal, -45.f);

	if (add_caps) {
		// Start cap
		indexes.append(base_idx + 0);
		indexes.append(base_idx + 2);
		indexes.append(base_idx + 6);
		indexes.append(base_idx + 0);
		indexes.append(base_idx + 6);
		indexes.append(base_idx + 3);
		indexes.append(base_idx + 0);
		indexes.append(base_idx + 3);
		indexes.append(base_idx + 7);
		indexes.append(base_idx + 0);
		indexes.append(base_idx + 7);
		indexes.append(base_idx + 2);

		// End cap
		indexes.append(base_idx + 1);
		indexes.append(base_idx + 4);
		indexes.append(base_idx + 8);
		indexes.append(base_idx + 1);
		indexes.append(base_idx + 8);
		indexes.append(base_idx + 5);
		indexes.append(base_idx + 1);
		indexes.append(base_idx + 5);
		indexes.append(base_idx + 9);
		indexes.append(base_idx + 1);
		indexes.append(base_idx + 9);
		indexes.append(base_idx + 4);
	}
}

Ref<ArrayMesh> GeometryGenerator::CreateVolumetricArrowHead(const real_t &radius, const real_t &length, const real_t &offset_mult, const bool &add_bevel) {
	PackedVector3Array vertexes;
	PackedVector2Array uv;
	PackedVector3Array custom0;
	PackedInt32Array indexes;

	real_t front_offset = add_bevel ? .5f : 0;
	real_t square_diag_mult = MathUtils::Sqrt2;

	vertexes.push_back(Vector3(0, 0, 0)); // 0

	vertexes.push_back(Vector3(0, 0, -0.00001f).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult); // 1
	vertexes.push_back(Vector3(0, 0, -0.00001f).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult); // 2
	vertexes.push_back(Vector3(0, 0, -0.00001f).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult); // 3
	vertexes.push_back(Vector3(0, 0, -0.00001f).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult); // 4

	vertexes.push_back(Vector3(0, radius, length).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult); // 5
	vertexes.push_back(Vector3(0, -radius, length).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult); // 6
	vertexes.push_back(Vector3(radius, 0, length).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult); // 7
	vertexes.push_back(Vector3(-radius, 0, length).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult); // 8

	custom0.push_back(Vector3(0, 0, 0));
	custom0.push_back(Vector3(0, 1, 0).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult - Vector3_FORWARD * front_offset);
	custom0.push_back(Vector3(0, -1, 0).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult - Vector3_FORWARD * front_offset);
	custom0.push_back(Vector3(1, 0, 0).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult - Vector3_FORWARD * front_offset);
	custom0.push_back(Vector3(-1, 0, 0).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult - Vector3_FORWARD * front_offset);

	custom0.push_back(Vector3(0, 1 + radius * 2, offset_mult * length * 2).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult);
	custom0.push_back(Vector3(0, -(1 + radius * 2), offset_mult * length * 2).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult);
	custom0.push_back(Vector3(1 + radius * 2, 0, offset_mult * length * 2).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult);
	custom0.push_back(Vector3(-(1 + radius * 2), 0, offset_mult * length * 2).rotated(Vector3_FORWARD, Math::deg_to_rad(45.f)) / square_diag_mult);

	uv.push_back(Vector2(.5f, .5f));

	uv.push_back(Vector2(0, 0));
	uv.push_back(Vector2(0, 0));
	uv.push_back(Vector2(0, 0));
	uv.push_back(Vector2(0, 0));

	uv.push_back(Vector2(.5f, .5f));
	uv.push_back(Vector2(.5f, .5f));
	uv.push_back(Vector2(.5f, .5f));
	uv.push_back(Vector2(.5f, .5f));

	indexes.push_back(0);
	indexes.push_back(1);
	indexes.push_back(3);
	indexes.push_back(0);
	indexes.push_back(3);
	indexes.push_back(2);
	indexes.push_back(0);
	indexes.push_back(2);
	indexes.push_back(4);
	indexes.push_back(0);
	indexes.push_back(4);
	indexes.push_back(1);

	indexes.push_back(1);
	indexes.push_back(3);
	indexes.push_back(5);
	indexes.push_back(3);
	indexes.push_back(7);
	indexes.push_back(5);

	indexes.push_back(1);
	indexes.push_back(5);
	indexes.push_back(8);
	indexes.push_back(1);
	indexes.push_back(8);
	indexes.push_back(4);

	indexes.push_back(2);
	indexes.push_back(6);
	indexes.push_back(8);
	indexes.push_back(2);
	indexes.push_back(8);
	indexes.push_back(4);

	indexes.push_back(3);
	indexes.push_back(7);
	indexes.push_back(6);
	indexes.push_back(3);
	indexes.push_back(6);
	indexes.push_back(2);

	return CreateMesh(
			Mesh::PRIMITIVE_TRIANGLES,
			vertexes,
			indexes,
			PackedColorArray(),
			PackedVector3Array(),
			uv,
			Utils::convert_packed_array_to_diffrent_types<PackedFloat32Array>(custom0),
			ArrayMesh::ARRAY_CUSTOM_RGB_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT);
}

Ref<ArrayMesh> GeometryGenerator::CreateCameraFrustumLines(const std::array<Plane, 6> &frustum) {
	ZoneScoped;
	std::vector<Vector3> res;
	CreateCameraFrustumLinesWireframe(frustum, res);

	return CreateMeshNative(
			Mesh::PRIMITIVE_LINES,
			res,
			std::array<int, 0>(),
			std::array<Color, 0>(),
			std::array<Vector3, 0>());
}

void GeometryGenerator::CreateCameraFrustumLinesWireframe(const std::array<Plane, 6> &frustum, std::vector<Vector3> &vertexes) {
	ZoneScoped;
	vertexes.resize(CubeIndexes.size());
	CreateCameraFrustumLinesWireframe(frustum, vertexes.data());
}

void GeometryGenerator::CreateCameraFrustumLinesWireframe(const std::array<Plane, 6> &frustum, Vector3 *vertexes) {
	ZoneScoped;

	auto cube = MathUtils::get_frustum_cube(frustum);
	for (int i = 0; i < CubeIndexes.size(); i++)
		vertexes[i] = cube[CubeIndexes[i]];
}

Ref<ArrayMesh> GeometryGenerator::CreateLinesFromPath(const PackedVector3Array &path) {
	ZoneScoped;
	std::vector<Vector3> vertexes;
	CreateLinesFromPathWireframe(path, vertexes);

	return CreateMeshNative(
			Mesh::PRIMITIVE_TRIANGLES,
			vertexes,
			std::array<int, 0>(),
			std::array<Color, 0>(),
			std::array<Vector3, 0>());
}

void GeometryGenerator::CreateLinesFromPathWireframe(const PackedVector3Array &path, std::vector<Vector3> &vertexes) {
	ZoneScoped;
	vertexes.resize(((size_t)path.size() - 1) * 2);

	for (size_t i = 0; i < (size_t)path.size() - 1; i++) {
		vertexes[i * 2 + 0] = path[(int)i];
		vertexes[i * 2 + 1] = path[(int)i + 1];
	}
}

void GeometryGenerator::CreateLinesFromPathWireframe(const PackedVector3Array &path, Vector3 *vertexes) {
	ZoneScoped;

	for (size_t i = 0; i < (size_t)path.size() - 1; i++) {
		vertexes[i * 2 + 0] = path[(int)i];
		vertexes[i * 2 + 1] = path[(int)i + 1];
	}
}

void GeometryGenerator::ConvertTriIndexesToWireframe(const PackedInt32Array &tri_indexes, std::vector<int> &indexes) {
	ZoneScoped;
	indexes.resize((size_t)tri_indexes.size() * 2);

	for (size_t i = 0; i < (size_t)tri_indexes.size() / 3; i++) {
		indexes[i * 6 + 0] = tri_indexes[(int)i * 3 + 0];
		indexes[i * 6 + 1] = tri_indexes[(int)i * 3 + 1];
		indexes[i * 6 + 2] = tri_indexes[(int)i * 3 + 1];
		indexes[i * 6 + 3] = tri_indexes[(int)i * 3 + 2];
		indexes[i * 6 + 4] = tri_indexes[(int)i * 3 + 2];
		indexes[i * 6 + 5] = tri_indexes[(int)i * 3 + 0];
	}
}

void GeometryGenerator::ConvertTriIndexesToWireframe(const PackedInt32Array &tri_indexes, int *indexes) {
	ZoneScoped;

	for (size_t i = 0; i < (size_t)tri_indexes.size() / 3; i++) {
		indexes[i * 6 + 0] = tri_indexes[(int)i * 3 + 0];
		indexes[i * 6 + 1] = tri_indexes[(int)i * 3 + 1];
		indexes[i * 6 + 2] = tri_indexes[(int)i * 3 + 1];
		indexes[i * 6 + 3] = tri_indexes[(int)i * 3 + 2];
		indexes[i * 6 + 4] = tri_indexes[(int)i * 3 + 2];
		indexes[i * 6 + 5] = tri_indexes[(int)i * 3 + 0];
	}
}

GeometryGenerator::IcosphereTriMesh GeometryGenerator::MakeIcosphereTriMesh(const real_t &radius, const int &resolution) {
	// https://winter.dev/projects/mesh/icosphere
	const float Z = (1.0f + Math::sqrt(5.0f)) / 2.0f; // Golden ratio

	const int IcoVertexCount = 22;
	const int IcoIndexCount = 60;

	const Vector3 IcoVerts[] = {
		Vector3(0, -1, -Z), Vector3(-1, -Z, 0), Vector3(Z, 0, -1), Vector3(1, -Z, 0),
		Vector3(1, Z, 0), Vector3(-1, -Z, 0), Vector3(Z, 0, 1), Vector3(0, -1, Z),
		Vector3(1, Z, 0), Vector3(-1, -Z, 0), Vector3(0, 1, Z), Vector3(-Z, 0, 1),
		Vector3(1, Z, 0), Vector3(-1, -Z, 0), Vector3(-1, Z, 0), Vector3(-Z, 0, -1),
		Vector3(1, Z, 0), Vector3(-1, -Z, 0), Vector3(0, 1, -Z), Vector3(0, -1, -Z),
		Vector3(1, Z, 0), Vector3(Z, 0, -1)
	};

	const int IcoIndex[] = {
		2, 6, 4, // Top
		6, 10, 8,
		10, 14, 12,
		14, 18, 16,
		18, 21, 20,

		0, 3, 2, // Middle
		2, 3, 6,
		3, 7, 6,
		6, 7, 10,
		7, 11, 10,
		10, 11, 14,
		11, 15, 14,
		14, 15, 18,
		15, 19, 18,
		18, 19, 21,

		0, 1, 3, // Bottom
		3, 5, 7,
		7, 9, 11,
		11, 13, 15,
		15, 17, 19
	};

	const int rn = (int)Math::pow(4.f, (float)resolution);
	const int totalIndexCount = IcoIndexCount * rn;
	const int totalVertexCount = IcoVertexCount + IcoIndexCount * (1 - rn) / (1 - 4);

	IcosphereTriMesh sphere;

	sphere.indexes.resize(totalIndexCount);
	sphere.vertexes.resize(totalVertexCount);
	sphere.normals.resize(totalVertexCount);

	for (int i = 0; i < IcoVertexCount; i++) { // Copy in initial mesh
		sphere.vertexes[i] = IcoVerts[i];
		sphere.normals[i] = IcoVerts[i].normalized();
	}

	for (int i = 0; i < IcoIndexCount; i++) {
		sphere.indexes[i] = IcoIndex[i];
	}

	int currentIndexCount = IcoIndexCount;
	int currentVertCount = IcoVertexCount;

	for (int r = 0; r < resolution; r++) {
		// Now split the triangles.
		// This can be done in place, but needs to keep track of the unique triangles

		std::unordered_map<uint64_t, int> triangleFromEdge;
		int indexCount = currentIndexCount;

		for (int t = 0; t < indexCount; t += 3) {
			int midpoints[3] = {};

			for (int e = 0; e < 3; e++) {
				int first = sphere.indexes[t + e];
				int second = sphere.indexes[t + (t + e + 1) % 3];

				if (first > second) {
					std::swap(first, second);
				}

				uint64_t hash = (uint64_t)first | (uint64_t)second << (sizeof(uint32_t) * 8);

				auto [triangle, wasNewEdge] = triangleFromEdge.insert({ hash, currentVertCount });

				if (wasNewEdge) {
					sphere.vertexes[currentVertCount] = (sphere.vertexes[first] + sphere.vertexes[second]) / 2.0f;
					sphere.normals[currentVertCount] = sphere.vertexes[currentVertCount].normalized();

					currentVertCount += 1;
				}

				midpoints[e] = triangle->second;
			}

			int mid0 = midpoints[0];
			int mid1 = midpoints[1];
			int mid2 = midpoints[2];

			sphere.indexes[currentIndexCount++] = sphere.indexes[t];
			sphere.indexes[currentIndexCount++] = mid0;
			sphere.indexes[currentIndexCount++] = mid2;

			sphere.indexes[currentIndexCount++] = sphere.indexes[t + 1];
			sphere.indexes[currentIndexCount++] = mid1;
			sphere.indexes[currentIndexCount++] = mid0;

			sphere.indexes[currentIndexCount++] = sphere.indexes[t + 2];
			sphere.indexes[currentIndexCount++] = mid2;
			sphere.indexes[currentIndexCount++] = mid1;

			sphere.indexes[t] = mid0; // Overwrite the original triangle with the 4th new triangle
			sphere.indexes[t + 1] = mid1;
			sphere.indexes[t + 2] = mid2;
		}
	}

	sphere.vertexes.resize(currentVertCount);
	sphere.normals.resize(currentVertCount);
	sphere.indexes.resize(currentIndexCount);

	// Normalize all the positions to create the sphere
	for (int i = 0; i < sphere.vertexes.size(); i++) {
		sphere.vertexes[i] = sphere.vertexes[i].normalized() * radius;
	}

#if false
	for (size_t i = 0; i < sphere.vertexes.size(); i++) {
		for (size_t j = 0; j < sphere.vertexes.size(); j++) {
			if (i != j && sphere.vertexes[i].is_equal_approx(sphere.vertexes[j])) {
				PRINT("Duplicate: {0} {1} {2}", i, j, sphere.vertexes[i]);
			}
		}
	}
#endif

	return sphere;
}

Ref<ArrayMesh> GeometryGenerator::CreateIcosphereLines(const real_t &radius, const int &depth) {
	auto res = MakeIcosphereTriMesh(radius, depth);

	// PRINT("{0} vertexes, {1} indexes", res.vertexes.size(), res.indexes.size());

	PackedInt32Array indexes_lines;
	indexes_lines.resize(res.indexes.size() * 2);
	ConvertTriIndexesToWireframe(res.indexes, indexes_lines.ptrw());

	return CreateMesh(
			Mesh::PRIMITIVE_LINES,
			res.vertexes,
			indexes_lines,
			PackedColorArray(),
			res.normals);
}

Ref<ArrayMesh> GeometryGenerator::CreateSphereLines(const int &_lats, const int &_lons, const real_t &radius, const int &subdivide) {
	ZoneScoped;
	int lats = _lats * subdivide;
	int lons = _lons * subdivide;

	if (lats < 2)
		lats = 2;
	if (lons < 4)
		lons = 4;

	PackedVector3Array vertexes;
	vertexes.resize((size_t)lats * (size_t)lons * 6);

	PackedVector3Array normals;
	normals.resize((size_t)lats * (size_t)lons * 6);

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

			Vector3 v[3]{
				Vector3(x1 * zr0, z0, y1 * zr0) * radius,
				Vector3(x1 * zr1, z1, y1 * zr1) * radius,
				Vector3(x0 * zr0, z0, y0 * zr0) * radius
			};

			if (j % subdivide == 0) {
				normals[total] = v[0].normalized();
				vertexes[total++] = v[0];

				normals[total] = v[1].normalized();
				vertexes[total++] = v[1];
			}

			if (i % subdivide == 0) {
				normals[total] = v[2].normalized();
				vertexes[total++] = v[2];

				normals[total] = v[0].normalized();
				vertexes[total++] = v[0];
			}
		}
	}

	return CreateMesh(
			Mesh::PRIMITIVE_LINES,
			vertexes,
			PackedInt32Array(),
			PackedColorArray(),
			normals);
}

Ref<ArrayMesh> GeometryGenerator::CreateCylinderLines(const int &edges, const real_t &radius, const real_t &height, const int &subdivide) {
	ZoneScoped;
	real_t angle = 360.f / edges;

	PackedVector3Array vertexes;
	PackedVector3Array normals;
	// vertexes.reserve(4 * (size_t)edges + (((size_t)edges * (size_t)subdivide) * 2));

	Vector3 helf_height = Vector3(0, height * 0.5f, 0);
	for (int i = 0; i < edges; i++) {
		float ra = Math::deg_to_rad(i * angle);
		float rb = Math::deg_to_rad((i + 1) * angle);
		Vector3 center_current = Vector3(sin(ra), 0, cos(ra)) * radius;
		Vector3 center_next = Vector3(sin(rb), 0, cos(rb)) * radius;

		// Top
		vertexes.push_back(center_current + helf_height);
		vertexes.push_back(center_next + helf_height);

		// Bottom
		vertexes.push_back(center_current - helf_height);
		vertexes.push_back(center_next - helf_height);

		// Repeat for Top and Bottom
		for (int i = 0; i < 2; i++) {
			normals.push_back(center_current.normalized());
			normals.push_back(center_next.normalized());
		}

		// Edge
		if (i % subdivide == 0) {
			vertexes.push_back(center_current + helf_height);
			vertexes.push_back(center_current - helf_height);

			normals.push_back(center_current.normalized());
			normals.push_back(center_current.normalized());
		}
	}

	return CreateMesh(
			Mesh::PRIMITIVE_LINES,
			vertexes,
			PackedInt32Array(),
			PackedColorArray(),
			normals);
}
