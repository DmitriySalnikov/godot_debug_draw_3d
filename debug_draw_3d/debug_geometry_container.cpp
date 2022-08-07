#include "debug_geometry_container.h"
#include "debug_draw.h"

#include <Engine.hpp>
#include <Texture.hpp>

using namespace godot;

#define GET_INSTANCES(type) mmi_with_values[type].second

DebugGeometryContainer::DebugGeometryContainer(class DebugDraw3D *root) {
	owner = root;

	// Create wireframe mesh drawer
	_immediateGeometry = ImmediateGeometry::_new();
	{
		_immediateGeometry->set_name(TEXT(_immediateGeometry));
		_immediateGeometry->set_cast_shadows_setting(GeometryInstance::ShadowCastingSetting::SHADOW_CASTING_SETTING_OFF);
		_immediateGeometry->set_flag(GeometryInstance::Flags::FLAG_USE_BAKED_LIGHT, false);

		Ref<SpatialMaterial> mat;
		mat.instance();
		mat->set_flag(SpatialMaterial::Flags::FLAG_UNSHADED, true);
		mat->set_flag(SpatialMaterial::Flags::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

		_immediateGeometry->set_material_override(mat);
	}

	root->add_child(_immediateGeometry);

	// Create node with material and MultiMesh. Add to tree. Create array of instances
	MultiMeshInstance *mmi_cubes = CreateMMI(root, InstanceType::CUBES, TEXT(mmi_cubes));
	MultiMeshInstance *mmi_cubes_centered = CreateMMI(root, InstanceType::CUBES_CENTERED, TEXT(mmi_cubes_centered));
	MultiMeshInstance *mmi_arrowheads = CreateMMI(root, InstanceType::ARROWHEADS, TEXT(mmi_arrowheads));
	MultiMeshInstance *mmi_billboard_squares = CreateMMI(root, InstanceType::BILLBOARD_SQUARES, TEXT(mmi_billboard_squares));
	MultiMeshInstance *mmi_positions = CreateMMI(root, InstanceType::POSITIONS, TEXT(mmi_positions));
	MultiMeshInstance *mmi_spheres = CreateMMI(root, InstanceType::SPHERES, TEXT(mmi_spheres));
	MultiMeshInstance *mmi_spheres_hd = CreateMMI(root, InstanceType::SPHERES_HD, TEXT(mmi_spheres_hd));
	MultiMeshInstance *mmi_cylinders = CreateMMI(root, InstanceType::CYLINDERS, TEXT(mmi_cylinders));

	// Customize parameters
	Ref<SpatialMaterial> sq_mat = mmi_billboard_squares->get_material_override();
	if (sq_mat.is_valid()) {
		sq_mat->set_billboard_mode(SpatialMaterial::BillboardMode::BILLBOARD_ENABLED);
		sq_mat->set_flag(SpatialMaterial::Flags::FLAG_BILLBOARD_KEEP_SCALE, true);
	}

	// Create Meshes

	mmi_cubes->get_multimesh()->set_mesh(CreateMesh(
			Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::CubeVertices, C_ARR_SIZE(GeometryGenerator::CubeVertices), GeometryGenerator::CubeIndices, C_ARR_SIZE(GeometryGenerator::CubeIndices)));

	mmi_cubes_centered->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::CenteredCubeVertices, C_ARR_SIZE(GeometryGenerator::CenteredCubeVertices), GeometryGenerator::CubeIndices, C_ARR_SIZE(GeometryGenerator::CubeIndices)));

	mmi_arrowheads->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::ArrowheadVertices, C_ARR_SIZE(GeometryGenerator::ArrowheadVertices), GeometryGenerator::ArrowheadIndices, C_ARR_SIZE(GeometryGenerator::ArrowheadIndices)));

	mmi_billboard_squares->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES,
			GeometryGenerator::CenteredSquareVertices, C_ARR_SIZE(GeometryGenerator::CenteredSquareVertices), GeometryGenerator::SquareIndices, C_ARR_SIZE(GeometryGenerator::SquareIndices)));

	mmi_positions->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::PositionVertices, C_ARR_SIZE(GeometryGenerator::PositionVertices), GeometryGenerator::PositionIndices, C_ARR_SIZE(GeometryGenerator::PositionIndices)));

	mmi_spheres->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::CreateSphereLines(8, 8, 0.5f, Vector3::ZERO)));

	mmi_spheres_hd->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::CreateSphereLines(16, 16, 0.5f, Vector3::ZERO)));

	mmi_cylinders->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::CreateCylinderLines(52, 0.5f, 1, Vector3::ZERO, 4)));
}

DebugGeometryContainer::~DebugGeometryContainer() {
	LOCK_GUARD_REC(datalock);

	geometry_pool.clear_pool();
	geometry_pool.clear_pool();
}

MultiMeshInstance *DebugGeometryContainer::CreateMMI(Node *root, InstanceType type, String name) {
	MultiMeshInstance *mmi = MultiMeshInstance::_new();
	mmi->set_name(name);
	mmi->set_cast_shadows_setting(GeometryInstance::ShadowCastingSetting::SHADOW_CASTING_SETTING_OFF);
	mmi->set_flag(GeometryInstance::Flags::FLAG_USE_BAKED_LIGHT, false);

	Ref<SpatialMaterial> new_mat;
	new_mat.instance();
	new_mat->set_flag(SpatialMaterial::Flags::FLAG_UNSHADED, true);
	new_mat->set_flag(SpatialMaterial::Flags::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

	mmi->set_material_override(new_mat);

	Ref<MultiMesh> new_mm;
	new_mm.instance();
	new_mm->set_name(String::num_int64(type));

	new_mm->set_transform_format(MultiMesh::TransformFormat::TRANSFORM_3D);
	new_mm->set_color_format(MultiMesh::ColorFormat::COLOR_FLOAT);
	new_mm->set_custom_data_format(MultiMesh::CustomDataFormat::CUSTOM_DATA_NONE);

	mmi->set_multimesh(new_mm);

	root->add_child(mmi);
	mmi_with_values[type] = mmi;
	return mmi;
}

Ref<ArrayMesh> DebugGeometryContainer::CreateMesh(Mesh::PrimitiveType type, const Vector3 *vertices, const size_t vertices_size, const int *indices, const size_t indices_size, const Color *colors, const size_t colors_size) {
	PoolVector3Array pool_vertices = Utils::convert_to_pool_array<PoolVector3Array>(vertices, vertices_size);
	PoolIntArray pool_indices = Utils::convert_to_pool_array<PoolIntArray>(indices, indices_size);
	PoolColorArray pool_colors = Utils::convert_to_pool_array<PoolColorArray>(colors, colors_size);
	return CreateMesh(type, pool_vertices, pool_indices, pool_colors);
}

Ref<ArrayMesh> DebugGeometryContainer::CreateMesh(Mesh::PrimitiveType type, const std::vector<Vector3> vertices, const std::vector<int> indices, const std::vector<Color> colors) {
	PoolVector3Array pool_vertices = Utils::convert_to_pool_array<PoolVector3Array>(vertices);
	PoolIntArray pool_indices = Utils::convert_to_pool_array<PoolIntArray>(indices);
	PoolColorArray pool_colors = Utils::convert_to_pool_array<PoolColorArray>(colors);
	return CreateMesh(type, pool_vertices, pool_indices, pool_colors);
}

Ref<ArrayMesh> DebugGeometryContainer::CreateMesh(Mesh::PrimitiveType type, const PoolVector3Array &vertices, const PoolIntArray &indices, const PoolColorArray &colors) {
	Ref<ArrayMesh> mesh;
	mesh.instance();
	Array a;
	a.resize((int)ArrayMesh::ArrayType::ARRAY_MAX);

	a[(int)ArrayMesh::ArrayType::ARRAY_VERTEX] = vertices;
	if (indices.size())
		a[(int)ArrayMesh::ArrayType::ARRAY_INDEX] = indices;
	if (colors.size())
		a[(int)ArrayMesh::ArrayType::ARRAY_COLOR] = colors;

	mesh->add_surface_from_arrays(type, a);

	return mesh;
}

void DebugGeometryContainer::update_geometry(real_t delta) {
	LOCK_GUARD_REC(datalock);

	// Don't clear geometry if freezed
	if (owner->is_freeze_3d_render())
		return;

	_immediateGeometry->clear();

	// Return if nothing to do
	if (!owner->is_debug_enabled()) {
		for (auto item : mmi_with_values) {
			item->get_multimesh()->set_visible_instance_count(0);
		}
		geometry_pool.reset_counter(delta);
		geometry_pool.reset_visible_objects();
		return;
	}

	// Get camera frustum
	Camera *vp_cam = owner->get_viewport()->get_camera();

	std::vector<std::vector<Plane> > f;
	if (owner->is_use_frustum_culling()) {
		std::vector<Viewport *> editor_viewports = owner->get_custom_editor_viewport();
		std::vector<Array> frustum_arrays;

		frustum_arrays.reserve(1);
		if (owner->is_force_use_camera_from_scene() || (!editor_viewports.size() && !owner->get_custom_viewport())) {
			frustum_arrays.push_back(vp_cam->get_frustum());
		} else if (owner->get_custom_viewport()) {
			frustum_arrays.push_back(owner->get_custom_viewport()->get_camera()->get_frustum());
		} else if (editor_viewports.size() > 0) {
			for (auto vp : editor_viewports) {
				if (vp->get_update_mode() == Viewport::UpdateMode::UPDATE_ALWAYS) {
					frustum_arrays.push_back(vp->get_camera()->get_frustum());
				}
			}
		} else {
			PRINT_WARNING("No camera found to perform frustum culling for DebugDraw");
		}

		// Convert frustum to vector
		f.reserve(1);

		for (auto &arr : frustum_arrays) {
			if (arr.size() == 6) {
				std::vector<Plane> a(6);
				for (int i = 0; i < arr.size(); i++)
					a[i] = (Plane)arr[i];
				f.push_back(a);
			}
		}
	}

	// Update visibility
	geometry_pool.update_visibility(f);

	// Debug bounds of instances and lines
	if (owner->is_draw_instance_bounds()) {
		// std::vector<Plane> new_instances(geometry_pool.get_used_instances());
		std::vector<std::pair<Vector3, real_t> > new_instances;

		auto draw_instance_spheres = [&new_instances](DelayedRendererInstance *o) {
			if (!o->is_visible || o->is_expired())
				return;
			new_instances.push_back({ o->bounds.Position, o->bounds.Radius });
		};

		geometry_pool.for_each_instance(draw_instance_spheres);

		// Draw custom sphere for 1 frame
		for (auto &i : new_instances) {
			geometry_pool.add_or_update_instance(
					InstanceType::SPHERES_HD,
					0,
					true,
					Transform(Basis().scaled(Vector3::ONE * i.second * 2), i.first),
					Colors::debug_bounds,
					SphereBounds(i.first, i.second),
					[](auto d) { d->is_used_one_time = true; });
		}

		geometry_pool.for_each_line([this](DelayedRendererLine *o) {
			if (!o->is_visible || o->is_expired())
				return;

			Vector3 bottom, top, diag;
			MathUtils::get_diagonal_vectors(o->bounds.position, o->bounds.position + o->bounds.size, bottom, top, diag);

			geometry_pool.add_or_update_instance(
					InstanceType::CUBES,
					0,
					true,
					Transform(Basis().scaled(diag), bottom),
					Colors::debug_bounds,
					SphereBounds(bottom + diag * 0.5f, abs(diag.length() * 0.5f)),
					[](auto d) { d->is_used_one_time = true; });
		});

	}

	// Draw immediate lines
	geometry_pool.fill_lines_data(_immediateGeometry);

	// Update MultiMeshInstances
	Ref<MultiMesh> mms[InstanceType::ALL];
	for (int i = 0; i < InstanceType::ALL; i++) {
		Ref<MultiMesh> mms = mmi_with_values[i]->get_multimesh();
		mms->set_visible_instance_count(-1);

		auto a = geometry_pool.get_raw_data((InstanceType)i);
		mms->set_instance_count(a.size() / INSTANCE_DATA_FLOAT_COUNT);
		if (a.size()) {
			mms->set_as_bulk_array(a);
		}
	}

	geometry_pool.scan_visible_instances();
	geometry_pool.update_expiration(delta);
	geometry_pool.reset_counter(delta);
}

Dictionary DebugGeometryContainer::get_rendered_primitives_count() {
	LOCK_GUARD_REC(datalock);
	return Dictionary::make(
			"instances", geometry_pool.get_used_instances_total(),
			"visible_instances", geometry_pool.get_visible_instances(),
			"wireframes", geometry_pool.get_used_lines_total(),
			"visible_wireframes", geometry_pool.get_visible_lines(),
			"total", geometry_pool.get_used_instances_total() + geometry_pool.get_used_lines_total(),
			"total_visible", geometry_pool.get_visible_instances() + geometry_pool.get_visible_lines());
}

void DebugGeometryContainer::create_arrow(Vector3 a, Vector3 b, Color color, real_t arrow_size, bool absolute_size, float duration) {
	LOCK_GUARD_REC(datalock);
	Vector3 dir = (b - a);
	real_t size = (absolute_size ? arrow_size : dir.length() * arrow_size) * 2;
	Vector3 pos = b - dir.normalized() * size;

	const Vector3 UP = Vector3(0.0000000001f, 1.00000001f, 0);
	Transform t = Transform();
	t.set_look_at(pos, b, UP);
	t.scale(Vector3::ONE * (size));
	t.origin = pos;

	geometry_pool.add_or_update_instance(
			InstanceType::ARROWHEADS,
			duration,
			true,
			t,
			color == Colors::empty_color ? Colors::light_green : color,
			SphereBounds(t.origin - t.basis.z * 0.5f, MathUtils::ArrowRadiusForSphere * size));
}

#pragma region Draw Functions

void DebugGeometryContainer::clear_3d_objects() {
	for (auto s : mmi_with_values) {
		s->get_multimesh()->set_instance_count(0);
	}
	_immediateGeometry->clear();
}

#pragma region 3D
#pragma region Spheres

void DebugGeometryContainer::draw_sphere(Vector3 position, float radius, Color color, float duration, bool hd) {
	Transform t(Basis(), position);
	t.basis.scale(Vector3::ONE * (radius * 2));

	draw_sphere_xf(t, color, duration, hd);
}

void DebugGeometryContainer::draw_sphere_xf(Transform transform, Color color, float duration, bool hd) {
	LOCK_GUARD_REC(datalock);
	geometry_pool.add_or_update_instance(
			hd ? InstanceType::SPHERES_HD : InstanceType::SPHERES,
			duration,
			true,
			transform,
			color == Colors::empty_color ? Colors::chartreuse : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * 0.51f));
}

void DebugGeometryContainer::draw_sphere_hd(Vector3 position, float radius, Color color, float duration) {
	draw_sphere(position, radius, color, duration, true);
}

void DebugGeometryContainer::draw_sphere_hd_xf(Transform transform, Color color, float duration) {
	draw_sphere_xf(transform, color, duration, true);
}

#pragma endregion // Spheres
#pragma region Cylinders

void DebugGeometryContainer::draw_cylinder(Vector3 position, Quat rotation, float radius, float height, Color color, float duration) {
	Transform t(rotation, position);
	t.basis.scale(Vector3(radius * 2, height, radius * 2));

	draw_cylinder_xf(t, color, duration);
}

void DebugGeometryContainer::draw_cylinder_xf(Transform transform, Color color, float duration) {
	LOCK_GUARD_REC(datalock);
	geometry_pool.add_or_update_instance(
			InstanceType::CYLINDERS,
			duration,
			true,
			transform,
			color == Colors::empty_color ? Colors::forest_green : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::CylinderRadiusForSphere));
}

#pragma endregion // Cylinders
#pragma region Boxes

void DebugGeometryContainer::draw_box(Vector3 position, Quat rotation, Vector3 size, Color color, bool is_box_centered, float duration) {
	Transform t(rotation, position);
	t.basis.scale(size);

	draw_box_xf(t, color, is_box_centered, duration);
}

void DebugGeometryContainer::draw_box_xf(Transform transform, Color color, bool is_box_centered, float duration) {
	LOCK_GUARD_REC(datalock);

	/// It's possible to use less space to contain box, but this method works better in more cases
	SphereBounds sb(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::CubeRadiusForSphere);

	if (!is_box_centered) {
		sb.Position = transform.origin + transform.basis.get_scale() * 0.5f;
	}
	geometry_pool.add_or_update_instance(
			is_box_centered ? InstanceType::CUBES_CENTERED : InstanceType::CUBES,
			duration,
			true,
			transform,
			color == Colors::empty_color ? Colors::forest_green : color,
			sb);
}

void DebugGeometryContainer::draw_aabb(AABB aabb, Color color, float duration) {
	Vector3 bottom, top, diag;
	MathUtils::get_diagonal_vectors(aabb.position, aabb.position + aabb.size, bottom, top, diag);
	draw_box(bottom, Quat(), diag, color, false, duration);
}

void DebugGeometryContainer::draw_aabb_ab(Vector3 a, Vector3 b, Color color, float duration) {
	Vector3 bottom, top, diag;
	MathUtils::get_diagonal_vectors(a, b, bottom, top, diag);
	draw_box(bottom, Quat(), diag, color, false, duration);
}

#pragma endregion // Boxes
#pragma region Lines

void DebugGeometryContainer::draw_line_3d_hit(Vector3 start, Vector3 end, Vector3 hit, bool is_hit, float hit_size, Color hit_color, Color after_hit_color, float duration) {
	LOCK_GUARD_REC(datalock);
	if (is_hit) {
		geometry_pool.add_or_update_line(duration, true, { start, hit }, hit_color == Colors::empty_color ? owner->get_line_hit_color() : hit_color);
		geometry_pool.add_or_update_line(duration, true, { hit, end }, after_hit_color == Colors::empty_color ? owner->get_line_after_hit_color() : after_hit_color);

		geometry_pool.add_or_update_instance(
				InstanceType::BILLBOARD_SQUARES,
				duration,
				true,
				Transform(Basis().scaled(Vector3::ONE * hit_size), hit),
				hit_color == Colors::empty_color ? owner->get_line_hit_color() : hit_color,
				SphereBounds(hit, MathUtils::CubeRadiusForSphere * hit_size));
	} else {
		geometry_pool.add_or_update_line(duration, true, { start, end }, hit_color == Colors::empty_color ? owner->get_line_hit_color() : hit_color);
	}
}

void DebugGeometryContainer::draw_line_3d_hit_offset(Vector3 start, Vector3 end, bool is_hit, float unit_offset_of_hit, float hit_size, Color hit_color, Color after_hit_color, float duration) {
	LOCK_GUARD_REC(datalock);
	if (is_hit && unit_offset_of_hit >= 0 && unit_offset_of_hit <= 1) {
		draw_line_3d_hit(start, end, ((end - start).normalized() * start.distance_to(end) * unit_offset_of_hit + start), is_hit, hit_size, hit_color, after_hit_color, duration);
	} else {
		draw_line_3d_hit(start, end, {}, is_hit, hit_size, hit_color, after_hit_color, duration);
	}
}

#pragma region Normal

void DebugGeometryContainer::draw_line_3d(Vector3 a, Vector3 b, Color color, float duration) {
	LOCK_GUARD_REC(datalock);
	geometry_pool.add_or_update_line(duration, true, { a, b }, color == Colors::empty_color ? Colors::red : color);
}

void DebugGeometryContainer::draw_lines_3d(PoolVector3Array lines, Color color, float duration) {
	LOCK_GUARD_REC(datalock);

	if (lines.size() % 2 != 0) {
		PRINT_ERROR("The size of the lines array must be even. " + String::num_int64(lines.size()) + " is not even.");
		return;
	}

	std::vector<Vector3> l(lines.size());
	{
		auto r = lines.read();
		memcpy(l.data(), r.ptr(), (size_t)lines.size() * sizeof(Vector3));
	}

	geometry_pool.add_or_update_line(duration, true, l, color == Colors::empty_color ? Colors::red : color);
}

void DebugGeometryContainer::draw_ray_3d(Vector3 origin, Vector3 direction, float length, Color color, float duration) {
	LOCK_GUARD_REC(datalock);
	geometry_pool.add_or_update_line(duration, true, { origin, origin + direction * length }, color == Colors::empty_color ? Colors::red : color);
}

void DebugGeometryContainer::draw_line_path_3d(PoolVector3Array path, Color color, float duration) {
	if (path.size() < 2) {
		PRINT_ERROR("Line path must contains at least 2 points. " + String::num_int64(path.size()) + " is not enough.");
		return;
	}

	LOCK_GUARD_REC(datalock);
	geometry_pool.add_or_update_line(duration, true, GeometryGenerator::CreateLinesFromPath(path), color == Colors::empty_color ? Colors::light_green : color);
}

#pragma endregion // Normal
#pragma region Arrows

void DebugGeometryContainer::draw_arrow_line_3d(Vector3 a, Vector3 b, Color color, float arrow_size, bool is_absolute_size, float duration) {
	LOCK_GUARD_REC(datalock);
	geometry_pool.add_or_update_line(duration, true, { a, b }, color == Colors::empty_color ? Colors::light_green : color);

	create_arrow(a, b, color, arrow_size, is_absolute_size, duration);
}

void DebugGeometryContainer::draw_arrow_ray_3d(Vector3 origin, Vector3 direction, float length, Color color, float arrow_size, bool is_absolute_size, float duration) {
	draw_arrow_line_3d(origin, origin + direction * length, color, arrow_size, is_absolute_size, duration);
}

void DebugGeometryContainer::draw_arrow_path_3d(PoolVector3Array path, Color color, float arrow_size, bool is_absolute_size, float duration) {
	LOCK_GUARD_REC(datalock);
	geometry_pool.add_or_update_line(duration, true, GeometryGenerator::CreateLinesFromPath(path), color == Colors::empty_color ? Colors::light_green : color);

	for (int i = 0; i < path.size() - 1; i++) {
		create_arrow(path[i], path[i + 1], color, arrow_size, is_absolute_size, duration);
	}
}

#pragma endregion // Arrows
#pragma endregion // Lines

#pragma region Misc

void DebugGeometryContainer::draw_billboard_square(Vector3 position, float size, Color color, float duration) {
	Transform t(Basis(), position);
	t.basis.scale(Vector3::ONE * size);

	LOCK_GUARD_REC(datalock);
	geometry_pool.add_or_update_instance(
			InstanceType::BILLBOARD_SQUARES,
			duration,
			true,
			t,
			color == Colors::empty_color ? Colors::red : color,
			SphereBounds(position, MathUtils::CubeRadiusForSphere * size));
}

#pragma region Camera Frustum

void DebugGeometryContainer::draw_camera_frustum(Camera *camera, Color color, float duration) {
	draw_camera_frustum_planes(camera->get_frustum(), color, duration);
}

void DebugGeometryContainer::draw_camera_frustum_planes(Array camera_frustum, Color color, float duration) {
	Plane planes[6] = { Plane() };

	if (camera_frustum.size() == 6) {
		for (int i = 0; i < camera_frustum.size(); i++) {
			planes[i] = camera_frustum[i];
		}
	} else {
		PRINT_ERROR("Camera frustum requires an array of 6 planes. Recieved " + String::num_int64(camera_frustum.size()));
	}

	draw_camera_frustum_planes_c(planes, color, duration);
}

void DebugGeometryContainer::draw_camera_frustum_planes_c(Plane planes[6], Color color, float duration) {
	auto lines = GeometryGenerator::CreateCameraFrustumLines(planes);

	LOCK_GUARD_REC(datalock);
	geometry_pool.add_or_update_line(duration, true, lines, color == Colors::empty_color ? Colors::red : color);
}

#pragma endregion // Camera Frustum

void DebugGeometryContainer::draw_position_3d(Vector3 position, Quat rotation, Vector3 scale, Color color, float duration) {
	Transform t(rotation, position);
	t.basis.scale(scale);

	draw_position_3d_xf(t, color, duration);
}

void DebugGeometryContainer::draw_position_3d_xf(Transform transform, Color color, float duration) {
	LOCK_GUARD_REC(datalock);
	geometry_pool.add_or_update_instance(
			InstanceType::POSITIONS,
			duration,
			true,
			transform,
			color == Colors::empty_color ? Colors::crimson : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::AxisRadiusForSphere));
}

void DebugGeometryContainer::draw_gizmo_3d(Vector3 position, Quat rotation, Vector3 scale, bool is_centered, float duration) {
	Transform t(rotation, position);
	t.basis.scale(scale);

	draw_gizmo_3d_xf(t, is_centered, duration);
}

void DebugGeometryContainer::draw_gizmo_3d_xf(Transform transform, bool is_centered, float duration) {
	LOCK_GUARD_REC(datalock);

	if (is_centered) {
		draw_arrow_line_3d(transform.origin - transform.basis.x /** 0.5f*/, transform.origin + transform.basis.x /** 0.5f*/, Colors::axis_x, 0.1f, true, duration);
		draw_arrow_line_3d(transform.origin - transform.basis.y /** 0.5f*/, transform.origin + transform.basis.y /** 0.5f*/, Colors::axis_y, 0.1f, true, duration);
		draw_arrow_line_3d(transform.origin - transform.basis.z /** 0.5f*/, transform.origin + transform.basis.z /** 0.5f*/, Colors::axis_z, 0.1f, true, duration);
	} else {
		draw_arrow_line_3d(transform.origin, transform.origin + transform.basis.x, Colors::axis_x, 0.15f, true, duration);
		draw_arrow_line_3d(transform.origin, transform.origin + transform.basis.y, Colors::axis_y, 0.15f, true, duration);
		draw_arrow_line_3d(transform.origin, transform.origin + transform.basis.z, Colors::axis_z, 0.15f, true, duration);
	}
}

#pragma endregion // Misc
#pragma endregion // 3D
#pragma endregion // Draw Functions
