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
	Vector3(-0.5f, 0.5f, 0.5f),
};

const std::array<Vector3, 8> GeometryGenerator::CubeVertexes{
	Vector3(0, 0, 0), // 0
	Vector3(1, 0, 0), // 1
	Vector3(1, 0, 1), // 2
	Vector3(0, 0, 1), // 3
	Vector3(0, 1, 0), // 4
	Vector3(1, 1, 0), // 5
	Vector3(1, 1, 1), // 6
	Vector3(0, 1, 1), //  7
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
	1, 6
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

Ref<ArrayMesh> GeometryGenerator::CreateArrayMesh(Mesh::PrimitiveType type, const PackedVector3Array &vertexes, const PackedInt32Array &indexes, const PackedColorArray &colors, const PackedVector3Array &normals, const PackedVector2Array &uv, const PackedFloat32Array &custom0, BitField<Mesh::ArrayFormat> flags) {
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

Ref<ArrayMesh> GeometryGenerator::RotatedArrayMesh(const Ref<ArrayMesh> mesh, const Vector3 &axis, const float &angle) {
	// TODO: rotate GeometryGenerator::GeneratedMeshData if it is ever needed.
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
		for (int64_t i = 0; i < arr.size(); i += 3) {
			reinterpret_cast<Vector3 *>(arr.ptrw() + i)->rotate(axis, angle);
			//Vector3 v = Vector3(arr[i + 0], arr[i + 1], arr[i + 2]).rotated(axis, angle);
			//arr[i + 0] = (float)v.x;
			//arr[i + 1] = (float)v.y;
			//arr[i + 2] = (float)v.z;
		}
	};

	rotate_vec3(vertexes);
	rotate_vec3(normals);
	rotate_f32(custom0);

	return CreateArrayMesh(
			ptype,
			vertexes,
			arrs[ArrayMesh::ArrayType::ARRAY_INDEX],
			arrs[ArrayMesh::ArrayType::ARRAY_COLOR],
			normals,
			arrs[ArrayMesh::ArrayType::ARRAY_TEX_UV],
			custom0,
			ArrayMesh::ARRAY_CUSTOM_RGB_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT);
}

GeometryGenerator::GeneratedMeshData GeometryGenerator::ConvertWireframeToVolumetric(GeometryGenerator::GeneratedMeshData mesh_data, const bool &add_bevel, const bool &add_caps, const bool &smoothed_segments) {
	ZoneScoped;
	Ref<ArrayMesh> mesh = mesh_data.mesh;
	std::vector<Plane> lines_planes = mesh_data.lines_planes;
	std::vector<bool> lines_bevels = mesh_data.lines_bevels;

	Array arrs = mesh->surface_get_arrays(0);
	ERR_FAIL_COND_V(arrs.size() == 0, mesh_data);

	PackedVector3Array vertexes = arrs[ArrayMesh::ArrayType::ARRAY_VERTEX];
	PackedVector3Array normals = arrs[ArrayMesh::ArrayType::ARRAY_NORMAL];
	PackedInt32Array indexes = arrs[ArrayMesh::ArrayType::ARRAY_INDEX];

	bool has_indexes = indexes.size();
	bool has_normals = normals.size();
	bool has_bevels = lines_bevels.size();
	bool has_planes = lines_planes.size();

	ERR_FAIL_COND_V(!has_indexes && vertexes.size() % 2 != 0, GeneratedMeshData());
	ERR_FAIL_COND_V(has_indexes && indexes.size() % 2 != 0, GeneratedMeshData());
	ERR_FAIL_COND_V(normals.size() && vertexes.size() != normals.size(), GeneratedMeshData());

	PackedVector3Array res_vertexes;
	PackedVector3Array res_custom0;
	PackedInt32Array res_indexes;
	PackedVector2Array res_uv;

	if (has_indexes) {
		for (int64_t i = 0; i < indexes.size(); i += 2) {
			Vector3 normal_a = has_normals ? normals[indexes[i]] : Vector3(0, 1, 0.0001f);
			Vector3 normal_b = has_normals ? normals[indexes[i + 1]] : Vector3(0, 1, 0.0001f);

			int64_t line_index = i / 2;
			Plane line_plane = has_planes ? lines_planes[line_index] : Plane();
			bool actual_add_bevel = has_bevels ? lines_bevels[line_index] : add_bevel;

			if (actual_add_bevel)
				GenerateVolumetricSegmentBevel(vertexes[indexes[i]], vertexes[indexes[i + 1]], normal_a, normal_b, line_plane, add_caps, smoothed_segments, res_vertexes, res_custom0, res_indexes, res_uv);
			else
				GenerateVolumetricSegment(vertexes[indexes[i]], vertexes[indexes[i + 1]], normal_a, normal_b, line_plane, add_caps, smoothed_segments, res_vertexes, res_custom0, res_indexes, res_uv);
		}
	} else {
		for (int64_t i = 0; i < vertexes.size(); i += 2) {
			Vector3 normal_a = has_normals ? normals[i] : Vector3(0, 1, 0.0001f);
			Vector3 normal_b = has_normals ? normals[i + 1] : Vector3(0, 1, 0.0001f);

			int64_t line_index = i / 2;
			Plane line_plane = has_planes ? lines_planes[line_index] : Plane();
			bool actual_add_bevel = has_bevels ? lines_bevels[line_index] : add_bevel;

			if (actual_add_bevel)
				GenerateVolumetricSegmentBevel(vertexes[i], vertexes[i + 1], normal_a, normal_b, line_plane, add_caps, smoothed_segments, res_vertexes, res_custom0, res_indexes, res_uv);
			else
				GenerateVolumetricSegment(vertexes[i], vertexes[i + 1], normal_a, normal_b, line_plane, add_caps, smoothed_segments, res_vertexes, res_custom0, res_indexes, res_uv);
		}
	}

	GeneratedMeshData data;
	data.mesh = CreateArrayMesh(
			Mesh::PRIMITIVE_TRIANGLES,
			res_vertexes,
			res_indexes,
			PackedColorArray(),
			PackedVector3Array(),
			res_uv,
			Utils::convert_packed_vector3_to_packed_float(res_custom0),
			ArrayMesh::ARRAY_CUSTOM_RGB_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT);

	return data;
}

void GeometryGenerator::GetExtendedVertexes(Vector3 &right_a, Vector3 &left_a, Vector3 &right_b, Vector3 &left_b, const real_t &angle, const Vector3 &dir, const Vector3 &normal_a, const Vector3 &normal_b, const bool &has_plane, const Plane &line_plane, const bool &smoothed_segments) {
	if (smoothed_segments) {
		Vector3 rotation_axis_a, rotation_axis_b;

		if (has_plane) {
			rotation_axis_a = line_plane.normal.cross(normal_a).normalized();
			rotation_axis_b = line_plane.normal.cross(normal_b).normalized();
		} else {
			rotation_axis_a = normal_a.cross(normal_a.cross(dir)).normalized();
			rotation_axis_b = normal_b.cross(normal_b.cross(dir)).normalized();
		}

		right_a = normal_a.rotated(rotation_axis_a, Math::deg_to_rad(angle));
		left_a = right_a * -1;

		right_b = normal_b.rotated(rotation_axis_b, Math::deg_to_rad(angle));
		left_b = right_b * -1;
	} else {
		right_a = dir.cross(normal_a.rotated(dir, Math::deg_to_rad(angle))).normalized();
		left_a = right_a * -1;

		right_b = dir.cross(normal_b.rotated(dir, Math::deg_to_rad(angle))).normalized();
		left_b = right_b * -1;
	}
}

void GeometryGenerator::GenerateVolumetricSegment(const Vector3 &a, const Vector3 &b, const Vector3 &normal_a, const Vector3 &normal_b, const Plane &line_plane, const bool &add_caps, const bool &smoothed_segments, PackedVector3Array &vertexes, PackedVector3Array &custom0, PackedInt32Array &indexes, PackedVector2Array &uv) {
	ZoneScoped;
	const bool debug_size = false;
	Vector3 debug_mult = debug_size ? Vector3_ONE * 0.5 : Vector3();
	Vector3 dir = (b - a).normalized();
	int64_t base_idx = vertexes.size();
	const bool has_plane = line_plane != Plane();

	auto add_side = [&](Vector3 pos_a, Vector3 pos_b, bool is_rotated) {
		int64_t start_idx = vertexes.size();

		Vector3 right_a, left_a, right_b, left_b;
		GetExtendedVertexes(right_a, left_a, right_b, left_b, is_rotated ? -45.f : 45.f, dir, normal_a, normal_b, has_plane, line_plane, smoothed_segments);

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

	add_side(a, b, false);
	add_side(a, b, true);

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

void GeometryGenerator::GenerateVolumetricSegmentBevel(const Vector3 &a, const Vector3 &b, const Vector3 &normal_a, const Vector3 &normal_b, const Plane &line_plane, const bool &add_caps, const bool &smoothed_segments, PackedVector3Array &vertexes, PackedVector3Array &custom0, PackedInt32Array &indexes, PackedVector2Array &uv) {
	ZoneScoped;
	bool debug_size = false;
	Vector3 debug_mult = debug_size ? Vector3_ONE * 0.5f : Vector3();
	real_t half_len = .5f;
	// real_t half_len = Math::clamp(len * .5f, .0f, 0.5f);
	Vector3 dir = (b - a).normalized();
	int64_t base_idx = vertexes.size();
	const bool has_plane = line_plane != Plane();

	vertexes.push_back(a); // 0
	vertexes.push_back(b); // 1

	uv.push_back(Vector2(.5f, .5f));
	uv.push_back(Vector2(.5f, .5f));

	custom0.push_back(Vector3_ZERO);
	custom0.push_back(Vector3_ZERO);

	auto add_side = [&](Vector3 pos_a, Vector3 pos_b, real_t angle) {
		int64_t start_idx = vertexes.size();

		Vector3 right_a, left_a, right_b, left_b;
		GetExtendedVertexes(right_a, left_a, right_b, left_b, angle, dir, normal_a, normal_b, has_plane, line_plane, smoothed_segments);

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

	add_side(a, b, 45.f);
	add_side(a, b, -45.f);

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

GeometryGenerator::GeneratedMeshData GeometryGenerator::CreateVolumetricArrowHead(const float &radius, const float &length, const float &offset_mult, const bool &add_bevel) {
	PackedVector3Array vertexes;
	PackedVector2Array uv;
	PackedVector3Array custom0;
	PackedInt32Array indexes;

	float front_offset = add_bevel ? .5f : 0;
	float square_diag_mult = MathUtils::Sqrt2;

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

	GeneratedMeshData data;
	data.mesh = CreateArrayMesh(
			Mesh::PRIMITIVE_TRIANGLES,
			vertexes,
			indexes,
			PackedColorArray(),
			PackedVector3Array(),
			uv,
			Utils::convert_packed_vector3_to_packed_float(custom0),
			ArrayMesh::ARRAY_CUSTOM_RGB_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT);
	return data;
}

GeometryGenerator::GeneratedMeshData GeometryGenerator::CreateCameraFrustumLines(const std::array<Plane, 6> &frustum) {
	ZoneScoped;
	std::vector<Vector3> vertexes;
	vertexes.resize(CubeIndexes.size());
	CreateCameraFrustumLinesWireframe(frustum, vertexes.data());

	return CreateMeshNative(
			Mesh::PRIMITIVE_LINES,
			vertexes,
			std::array<int, 0>(),
			std::array<Color, 0>(),
			std::array<Vector3, 0>());
}

void GeometryGenerator::CreateCameraFrustumLinesWireframe(const std::array<Plane, 6> &frustum, Vector3 *vertexes) {
	ZoneScoped;
	CreateCameraFrustumLinesWireframe(frustum.data(), frustum.size(), vertexes);
}

void GeometryGenerator::CreateCameraFrustumLinesWireframe(const godot::Plane *frustum_data, const uint64_t frustum_size, Vector3 *vertexes) {
	ZoneScoped;
	auto cube = MathUtils::get_frustum_cube(frustum_data, frustum_size);
	for (int i = 0; i < CubeIndexes.size(); i++)
		vertexes[i] = cube[CubeIndexes[i]];
}

void GeometryGenerator::CreateLinesFromPathWireframe(const Vector3 *path_data, const uint64_t path_size, std::vector<Vector3> &vertexes) {
	ZoneScoped;
	vertexes.resize((path_size - 1) * 2);
	CreateLinesFromPathWireframe(path_data, path_size, vertexes.data());
}

void GeometryGenerator::CreateLinesFromPathWireframe(const Vector3 *path_data, const uint64_t path_size, Vector3 *vertexes) {
	ZoneScoped;
	for (int64_t i = 0; i < path_size - 1; i++) {
		vertexes[i * 2 + 0] = path_data[i];
		vertexes[i * 2 + 1] = path_data[i + 1];
	}
}

void GeometryGenerator::ConvertTriIndexesToWireframe(const PackedInt32Array &tri_indexes, std::vector<int> &indexes) {
	ZoneScoped;
	indexes.resize(tri_indexes.size() * 2);

	for (int64_t i = 0; i < tri_indexes.size() / 3; i++) {
		indexes[i * 6 + 0] = tri_indexes[i * 3 + 0];
		indexes[i * 6 + 1] = tri_indexes[i * 3 + 1];
		indexes[i * 6 + 2] = tri_indexes[i * 3 + 1];
		indexes[i * 6 + 3] = tri_indexes[i * 3 + 2];
		indexes[i * 6 + 4] = tri_indexes[i * 3 + 2];
		indexes[i * 6 + 5] = tri_indexes[i * 3 + 0];
	}
}

void GeometryGenerator::ConvertTriIndexesToWireframe(const PackedInt32Array &tri_indexes, int *indexes) {
	ZoneScoped;

	for (int64_t i = 0; i < tri_indexes.size() / 3; i++) {
		indexes[i * 6 + 0] = tri_indexes[i * 3 + 0];
		indexes[i * 6 + 1] = tri_indexes[i * 3 + 1];
		indexes[i * 6 + 2] = tri_indexes[i * 3 + 1];
		indexes[i * 6 + 3] = tri_indexes[i * 3 + 2];
		indexes[i * 6 + 4] = tri_indexes[i * 3 + 2];
		indexes[i * 6 + 5] = tri_indexes[i * 3 + 0];
	}
}

GeometryGenerator::IcosphereTriMesh GeometryGenerator::MakeIcosphereTriMesh(const float &radius, const int &resolution) {
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

		for (int64_t t = 0; t < indexCount; t += 3) {
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

GeometryGenerator::GeneratedMeshData GeometryGenerator::CreateIcosphereLines(const float &radius, const int &depth) {
	auto res = MakeIcosphereTriMesh(radius, depth);

	// PRINT("{0} vertexes, {1} indexes", res.vertexes.size(), res.indexes.size());

	PackedInt32Array indexes_lines;
	indexes_lines.resize(res.indexes.size() * 2);
	ConvertTriIndexesToWireframe(res.indexes, indexes_lines.ptrw());

	GeneratedMeshData data;
	data.mesh = CreateArrayMesh(
			Mesh::PRIMITIVE_LINES,
			res.vertexes,
			indexes_lines,
			PackedColorArray(),
			res.normals);
	return data;
}

GeometryGenerator::GeneratedMeshData GeometryGenerator::CreateSphereLines(const int &_lats, const int &_lons, const float &radius, const int &subdivide) {
	ZoneScoped;
	int lats = _lats * subdivide;
	int lons = _lons * subdivide;

	if (lats < 2)
		lats = 2;
	if (lons < 4)
		lons = 4;

	PackedVector3Array vertexes;
	PackedVector3Array normals;
	std::vector<Plane> lines_planes;

	int total = 0;
	real_t lats_step = Math_TAU / lats;
	real_t lons_step = Math_PI / lons;
	for (int j = 0; j < lons; j++) {
		real_t lon1_a = Math_PI / 2 - j * lons_step;
		real_t lon2_a = Math_PI / 2 - (j + 1) * lons_step;

		for (int i = 0; i < lats; i++) {
			real_t lat1_a = i * lats_step;
			real_t lat2_a = (i + 1) * lats_step;

			Vector3 pc = {
				radius * Math::cos(lon1_a) * Math::cos(lat1_a),
				radius * Math::sin(lon1_a),
				radius * Math::cos(lon1_a) * Math::sin(lat1_a),
			};

			Vector3 pv = {
				radius * Math::cos(lon2_a) * Math::cos(lat1_a),
				radius * Math::sin(lon2_a),
				radius * Math::cos(lon2_a) * Math::sin(lat1_a),
			};

			Vector3 ph = {
				radius * Math::cos(lon1_a) * Math::cos(lat2_a),
				radius * Math::sin(lon1_a),
				radius * Math::cos(lon1_a) * Math::sin(lat2_a),
			};

			// Vertical lines
			if (i % subdivide == 0 && true) {
				lines_planes.push_back(Plane(Vector3(0, 0, 0), pc, pv));

				vertexes.push_back(pc);
				vertexes.push_back(pv);

				normals.push_back(pc.normalized());
				normals.push_back(pv.normalized());
			}

			// Horizontal lines
			if ((j + subdivide / 2) % subdivide == 0 && true) {
				lines_planes.push_back(Plane(Vector3(0, pc.y, 0), pc, ph));

				vertexes.push_back(pc);
				vertexes.push_back(ph);

				normals.push_back(pc.normalized());
				normals.push_back(ph.normalized());
			}
		}
	}

	GeneratedMeshData data;
	data.lines_planes = lines_planes;
	data.mesh = CreateArrayMesh(
			Mesh::PRIMITIVE_LINES,
			vertexes,
			PackedInt32Array(),
			PackedColorArray(),
			normals);
	return data;
}

GeometryGenerator::GeneratedMeshData GeometryGenerator::CreateCylinderLines(const int &edges, const float &radius, const float &height, const int &subdivide) {
	ZoneScoped;
	real_t angle = 360.f / edges;

	PackedVector3Array vertexes;
	PackedVector3Array normals;
	std::vector<bool> bevels;
	// vertexes.reserve(4 * (size_t)edges + (((size_t)edges * (size_t)subdivide) * 2));

	Vector3 half_height = Vector3(0, height * 0.5f, 0);
	for (int i = 0; i < edges; i++) {
		real_t ra = Math::deg_to_rad(i * angle);
		real_t rb = Math::deg_to_rad((i + 1) * angle);
		Vector3 an = Vector3(Math::sin(ra), 0, Math::cos(ra));
		Vector3 bn = Vector3(Math::sin(rb), 0, Math::cos(rb));
		Vector3 a = an * radius;
		Vector3 b = bn * radius;

		// Top
		vertexes.push_back(a + half_height);
		vertexes.push_back(b + half_height);
		bevels.push_back(false);

		// Bottom
		vertexes.push_back(a - half_height);
		vertexes.push_back(b - half_height);
		bevels.push_back(false);

		// Repeat for Top and Bottom normals
		normals.push_back(an);
		normals.push_back(bn);

		normals.push_back(an);
		normals.push_back(bn);

		// Edge
		if (i % subdivide == 0) {
			vertexes.push_back(a + half_height);
			vertexes.push_back(a - half_height);
			bevels.push_back(true);

			normals.push_back(an);
			normals.push_back(an);
		}
	}

	GeneratedMeshData data;
	data.lines_bevels = bevels;
	data.mesh = CreateArrayMesh(
			Mesh::PRIMITIVE_LINES,
			vertexes,
			PackedInt32Array(),
			PackedColorArray(),
			normals);
	return data;
}

GeometryGenerator::GeneratedMeshData GeometryGenerator::CreateCapsuleCapLines(const int &edges, const float &radius) {
	ZoneScoped;

	PackedVector3Array vertexes;
	PackedVector3Array normals;

	real_t angle_step = 180.f / edges;
	for (int i = 0; i < edges; i++) {
		real_t a1 = Math::deg_to_rad((real_t)(i * angle_step));
		real_t a2 = Math::deg_to_rad((real_t)((i + 1) * angle_step));

		Vector2 p1 = Vector2(Math::cos(a1), Math::sin(a1));
		Vector2 p2 = Vector2(Math::cos(a2), Math::sin(a2));
		Vector2 pn1 = Vector2(p1.x, p1.y).normalized();
		Vector2 pn2 = Vector2(p2.x, p2.y).normalized();

		// Horizontal circle
		vertexes.push_back(Vector3(p1.x, 0, p1.y));
		vertexes.push_back(Vector3(p2.x, 0, p2.y));

		vertexes.push_back(Vector3(-p1.x, 0, -p1.y));
		vertexes.push_back(Vector3(-p2.x, 0, -p2.y));

		normals.push_back(Vector3(pn1.x, 0, pn1.y));
		normals.push_back(Vector3(pn2.x, 0, pn2.y));

		normals.push_back(Vector3(-pn1.x, 0, -pn1.y));
		normals.push_back(Vector3(-pn2.x, 0, -pn2.y));

		// X
		vertexes.push_back(Vector3(p1.x, p1.y, 0));
		vertexes.push_back(Vector3(p2.x, p2.y, 0));

		normals.push_back(Vector3(pn1.x, pn1.y, 0));
		normals.push_back(Vector3(pn2.x, pn2.y, 0));

		// Z
		vertexes.push_back(Vector3(0, p1.y, p1.x));
		vertexes.push_back(Vector3(0, p2.y, p2.x));

		normals.push_back(Vector3(0, pn1.y, pn1.x));
		normals.push_back(Vector3(0, pn2.y, pn2.x));
	}

	GeneratedMeshData data;
	data.mesh = CreateArrayMesh(
			Mesh::PRIMITIVE_LINES,
			vertexes,
			PackedInt32Array(),
			PackedColorArray(),
			normals);
	return data;
}

GeometryGenerator::GeneratedMeshData GeometryGenerator::CreateCapsuleEdgeLines(const float &radius, const float &height) {
	ZoneScoped;

	PackedVector3Array vertexes;
	PackedVector3Array normals;

	real_t half_height = height * 0.5f;
	for (int i = 0; i < 360; i += 90) {
		real_t angle = Math::deg_to_rad((real_t)i);
		Vector2 a = Vector2(Math::sin(angle), Math::cos(angle));
		Vector3 an = Vector3(a.x, 0, a.y).normalized();

		vertexes.push_back(Vector3(a.x, half_height, a.y));
		vertexes.push_back(Vector3(a.x, -half_height, a.y));

		normals.push_back(an);
		normals.push_back(an);
	}

	GeneratedMeshData data;
	data.mesh = CreateArrayMesh(
			Mesh::PRIMITIVE_LINES,
			vertexes,
			PackedInt32Array(),
			PackedColorArray(),
			normals);
	return data;
}
