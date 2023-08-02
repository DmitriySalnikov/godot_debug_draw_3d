#include "debug_geometry_container.h"

#ifndef DISABLE_DEBUG_RENDERING
#include "debug_draw_3d.h"
#include "debug_draw_config_3d.h"
#include "draw_stats.h"
#include "utils.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/main_loop.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/world3d.hpp>
GODOT_WARNING_RESTORE()

#include <array>

using namespace godot;

DebugGeometryContainer::DebugGeometryContainer(class DebugDraw3D *root) {
	owner = root;
	RenderingServer *rs = RS();

	// Create wireframe mesh drawer
	{
		Ref<ArrayMesh> _array_mesh;
		_array_mesh.instantiate();
		RID _immediate_instance = rs->instance_create();

		rs->instance_set_base(_immediate_instance, _array_mesh->get_rid());
		rs->instance_geometry_set_cast_shadows_setting(_immediate_instance, RenderingServer::SHADOW_CASTING_SETTING_OFF);
		rs->instance_geometry_set_flag(_immediate_instance, RenderingServer::INSTANCE_FLAG_USE_DYNAMIC_GI, false);
		rs->instance_geometry_set_flag(_immediate_instance, RenderingServer::INSTANCE_FLAG_USE_BAKED_LIGHT, false);

		Ref<StandardMaterial3D> mat;
		mat.instantiate();
		mat->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
		mat->set_flag(StandardMaterial3D::Flags::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

		rs->instance_geometry_set_material_override(_immediate_instance, mat->get_rid());

		immediate_mesh_storage.instance = _immediate_instance;
		immediate_mesh_storage.material = mat;
		immediate_mesh_storage.mesh = _array_mesh;
	}

	// Create node with material and MultiMesh. Add to tree. Create array of instances
	{
		CreateMMI(InstanceType::CUBES, NAMEOF(mmi_cubes), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CubeVertices, GeometryGenerator::CubeIndices));
		CreateMMI(InstanceType::CUBES_CENTERED, NAMEOF(mmi_cubes_centered), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CenteredCubeVertices, GeometryGenerator::CubeIndices));
		CreateMMI(InstanceType::ARROWHEADS, NAMEOF(mmi_arrowheads), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::ArrowheadVertices, GeometryGenerator::ArrowheadIndices));
		CreateMMI(InstanceType::BILLBOARD_SQUARES, NAMEOF(mmi_billboard_squares), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES, GeometryGenerator::CenteredSquareVertices, GeometryGenerator::SquareIndices));
		CreateMMI(InstanceType::POSITIONS, NAMEOF(mmi_positions), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::PositionVertices, GeometryGenerator::PositionIndices));
		CreateMMI(InstanceType::SPHERES, NAMEOF(mmi_spheres), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CreateSphereLines(8, 8, 0.5f, Vector3_ZERO)));
		CreateMMI(InstanceType::SPHERES_HD, NAMEOF(mmi_spheres_hd), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CreateSphereLines(16, 16, 0.5f, Vector3_ZERO)));
		CreateMMI(InstanceType::CYLINDERS, NAMEOF(mmi_cylinders), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CreateCylinderLines(52, 1, 1, Vector3_ZERO, 4)));

		set_render_layer_mask(1);
	}
}

DebugGeometryContainer::~DebugGeometryContainer() {
	LOCK_GUARD(datalock);

	geometry_pool.clear_pool();
	geometry_pool.clear_pool();

	RenderingServer *rs = RS();
	rs->free_rid(immediate_mesh_storage.instance);
	immediate_mesh_storage.mesh.unref();
	immediate_mesh_storage.material.unref();

	for (auto &i : multi_mesh_storage) {
		rs->free_rid(i.instance);
		i.mesh.unref();
	}
}

void DebugGeometryContainer::CreateMMI(InstanceType type, const String &name, Ref<ArrayMesh> mesh) {
	RenderingServer *rs = RS();

	RID mmi = rs->instance_create();

	Ref<MultiMesh> new_mm;
	new_mm.instantiate();
	new_mm->set_name(String::num_int64(type));

	new_mm->set_transform_format(MultiMesh::TransformFormat::TRANSFORM_3D);
	new_mm->set_use_colors(true);
	new_mm->set_transform_format(MultiMesh::TRANSFORM_3D);
	new_mm->set_use_custom_data(false);
	new_mm->set_mesh(mesh);

	rs->instance_set_base(mmi, new_mm->get_rid());

	Ref<StandardMaterial3D> new_mat;
	new_mat.instantiate();
	new_mat->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
	new_mat->set_flag(StandardMaterial3D::Flags::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

	if (type == InstanceType::BILLBOARD_SQUARES) {
		new_mat->set_billboard_mode(StandardMaterial3D::BillboardMode::BILLBOARD_ENABLED);
		new_mat->set_flag(StandardMaterial3D::Flags::FLAG_BILLBOARD_KEEP_SCALE, true);
	}

	mesh->surface_set_material(0, new_mat);

	rs->instance_geometry_set_cast_shadows_setting(mmi, RenderingServer::SHADOW_CASTING_SETTING_OFF);
	rs->instance_geometry_set_flag(mmi, RenderingServer::INSTANCE_FLAG_USE_DYNAMIC_GI, false);
	rs->instance_geometry_set_flag(mmi, RenderingServer::INSTANCE_FLAG_USE_BAKED_LIGHT, false);

	multi_mesh_storage[type].instance = mmi;
	multi_mesh_storage[type].mesh = new_mm;
}

Ref<ArrayMesh> DebugGeometryContainer::CreateMesh(Mesh::PrimitiveType type, const PackedVector3Array &vertices, const PackedInt32Array &indices, const PackedColorArray &colors) {
	Ref<ArrayMesh> mesh;
	mesh.instantiate();
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

void DebugGeometryContainer::set_world(Node *new_world) {
	scene_world_node = new_world;
	RenderingServer *rs = RS();
	RID scenario;
	Viewport *viewport = Object::cast_to<Viewport>(new_world);
	if (viewport) {
		scenario = viewport->get_world_3d()->get_scenario();
	} else {
		scenario = new_world->get_viewport()->get_world_3d()->get_scenario();
	}

	for (auto &s : multi_mesh_storage) {
		rs->instance_set_scenario(s.instance, scenario);
	}
	rs->instance_set_scenario(immediate_mesh_storage.instance, scenario);
}

Node *DebugGeometryContainer::get_world() {
	return scene_world_node;
}

void DebugGeometryContainer::update_geometry(double delta) {
	LOCK_GUARD(datalock);

	// Don't clear geometry if frozen
	if (owner->get_config_3d()->is_freeze_3d_render())
		return;

	immediate_mesh_storage.mesh->clear_surfaces();

	// Return if nothing to do
	if (!owner->is_debug_enabled()) {
		for (auto &item : multi_mesh_storage) {
			item.mesh->set_visible_instance_count(0);
		}
		geometry_pool.reset_counter(delta);
		geometry_pool.reset_visible_objects();
		return;
	}

	// Update render layers
	if (render_layers != owner->get_config_3d()->get_geometry_render_layers()) {
		set_render_layer_mask(owner->get_config_3d()->get_geometry_render_layers());
	}

	// TODO: try to get all active cameras inside scene to properly calculate visibilty for SubViewports

	// Get camera frustum
	Camera3D *vp_cam = owner->get_root_node()->get_viewport()->get_camera_3d();
	if (IS_EDITOR_HINT()) {
		auto s_root = SCENE_TREE()->get_edited_scene_root();
		if (s_root) {
			vp_cam = s_root->get_viewport()->get_camera_3d();
		}
	}

	// Collect frustums and camera positions
	std::vector<std::vector<Plane> > frustum_planes;
	std::vector<Vector3> cameras_positions;
	{
		std::vector<SubViewport *> editor_viewports = owner->get_custom_editor_viewports();
		std::vector<Array> frustum_arrays;

		frustum_arrays.reserve(1);
		cameras_positions.reserve(1);
		if ((owner->get_config_3d()->is_force_use_camera_from_scene() || (!editor_viewports.size() && !owner->get_custom_viewport())) && vp_cam) {
			frustum_arrays.push_back(vp_cam->get_frustum());
			cameras_positions.push_back(vp_cam->get_position());
		} else if (owner->get_custom_viewport()) {
			auto c = owner->get_custom_viewport()->get_camera_3d();
			frustum_arrays.push_back(c->get_frustum());
			cameras_positions.push_back(c->get_position());
		} else if (editor_viewports.size() > 0) {
			for (auto vp : editor_viewports) {
				if (vp->get_update_mode() == SubViewport::UpdateMode::UPDATE_ALWAYS) {
					auto c = vp->get_camera_3d();
					frustum_arrays.push_back(c->get_frustum());
					cameras_positions.push_back(c->get_position());
				}
			}
		}

		// Convert frustum to vector
		if (owner->get_config_3d()->is_use_frustum_culling() && frustum_arrays.size()) {
			frustum_planes.reserve(frustum_arrays.size());

			for (auto &arr : frustum_arrays) {
				if (arr.size() == 6) {
					std::vector<Plane> a(6);
					for (int i = 0; i < arr.size(); i++)
						a[i] = (Plane)arr[i];
					frustum_planes.push_back(a);
				}
			}
		}
	}

	// Update visibility
	geometry_pool.update_visibility(
			frustum_planes,
			GeometryPoolDistanceCullingData(owner->get_config_3d()->get_cull_by_distance(), cameras_positions));

	// Debug bounds of instances and lines
	if (owner->get_config_3d()->is_visible_instance_bounds()) {
		std::vector<std::pair<Vector3, real_t> > new_instances;

		auto draw_instance_spheres = [&new_instances](DelayedRendererInstance *o) {
			if (!o->is_visible || o->is_expired())
				return;
			new_instances.push_back({ o->bounds.position, o->bounds.Radius });
		};

		geometry_pool.for_each_instance(draw_instance_spheres);

		// Draw custom sphere for 1 frame
		for (auto &i : new_instances) {
			geometry_pool.add_or_update_instance(
					InstanceType::SPHERES_HD,
					0,
					Transform3D(Basis().scaled(Vector3_ONE * i.second * 2), i.first),
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
					Transform3D(Basis().scaled(diag), bottom),
					Colors::debug_bounds,
					SphereBounds(bottom + diag * 0.5f, abs(diag.length() * 0.5f)),
					[](auto d) { d->is_used_one_time = true; });
		});
	}

	// Draw immediate lines
	geometry_pool.fill_lines_data(immediate_mesh_storage.mesh);

	// Update MultiMeshInstances
	std::array<Ref<MultiMesh> *, InstanceType::ALL> meshes;
	for (int i = 0; i < InstanceType::ALL; i++) {
		meshes[i] = &multi_mesh_storage[i].mesh;
	}

	geometry_pool.fill_instance_data(meshes);

	geometry_pool.scan_visible_instances();
	geometry_pool.update_expiration(delta);
	geometry_pool.reset_counter(delta);
}

Ref<DebugDrawStats> DebugGeometryContainer::get_render_stats() {
	LOCK_GUARD(datalock);
	return geometry_pool.get_stats();
}

void DebugGeometryContainer::set_render_layer_mask(int32_t layers) {
	LOCK_GUARD(datalock);
	RenderingServer *rs = RS();
	for (auto &mmi : multi_mesh_storage)
		rs->instance_set_layer_mask(mmi.instance, layers);

	rs->instance_set_layer_mask(immediate_mesh_storage.instance, layers);
	render_layers = layers;
}

int32_t DebugGeometryContainer::get_render_layer_mask() const {
	return render_layers;
}

void DebugGeometryContainer::clear_3d_objects() {
	for (auto &s : multi_mesh_storage) {
		s.mesh->set_instance_count(0);
	}
	immediate_mesh_storage.mesh->clear_surfaces();

	geometry_pool.clear_pool();
}

#endif