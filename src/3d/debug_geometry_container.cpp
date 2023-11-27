#include "debug_geometry_container.h"

#ifndef DISABLE_DEBUG_RENDERING
#include "config_3d.h"
#include "debug_draw_3d.h"
#include "geometry_generators.h"
#include "stats_3d.h"

#include <array>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/world3d.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

DebugGeometryContainer::DebugGeometryContainer(class DebugDraw3D *root) {
	owner = root;
	RenderingServer *rs = RenderingServer::get_singleton();

	// Create wireframe mesh drawer
	{
		Ref<ArrayMesh> _array_mesh;
		_array_mesh.instantiate();
		RID _immediate_instance = rs->instance_create();

		rs->instance_set_base(_immediate_instance, _array_mesh->get_rid());
		rs->instance_geometry_set_cast_shadows_setting(_immediate_instance, RenderingServer::SHADOW_CASTING_SETTING_OFF);
		rs->instance_geometry_set_flag(_immediate_instance, RenderingServer::INSTANCE_FLAG_USE_DYNAMIC_GI, false);
		rs->instance_geometry_set_flag(_immediate_instance, RenderingServer::INSTANCE_FLAG_USE_BAKED_LIGHT, false);

		Ref<ShaderMaterial> mat = owner->get_basic_unshaded_material();
		rs->instance_geometry_set_material_override(_immediate_instance, mat->get_rid());

		immediate_mesh_storage.instance = _immediate_instance;
		immediate_mesh_storage.material = mat;
		immediate_mesh_storage.mesh = _array_mesh;
	}

	// Create node with material and MultiMesh. Add to tree. Create array of instances
	{
		auto array_mesh_cube = GeometryGenerator::CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CubeVertices, GeometryGenerator::CubeIndices);
		CreateMMI(InstanceType::CUBE, UsingShaderType::Wireframe, NAMEOF(mmi_cubes), array_mesh_cube);

		auto array_mesh_cube_center = GeometryGenerator::CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CenteredCubeVertices, GeometryGenerator::CubeIndices);
		CreateMMI(InstanceType::CUBE_CENTERED, UsingShaderType::Wireframe, NAMEOF(mmi_cubes_centered), array_mesh_cube_center);

		auto array_mesh_arrow_head = GeometryGenerator::CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::ArrowheadVertices, GeometryGenerator::ArrowheadIndices);
		CreateMMI(InstanceType::ARROWHEAD, UsingShaderType::Wireframe, NAMEOF(mmi_arrowheads), array_mesh_arrow_head);

		auto array_mesh_pos = GeometryGenerator::CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::PositionVertices, GeometryGenerator::PositionIndices);
		CreateMMI(InstanceType::POSITION, UsingShaderType::Wireframe, NAMEOF(mmi_positions), array_mesh_pos);

		auto array_mesh_sphere = GeometryGenerator::CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CreateSphereLines(8, 8, 0.5f, Vector3_ZERO, 2));
		CreateMMI(InstanceType::SPHERE, UsingShaderType::Wireframe, NAMEOF(mmi_spheres), array_mesh_sphere);

		auto array_mesh_sphere_hd = GeometryGenerator::CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CreateSphereLines(16, 16, 0.5f, Vector3_ZERO, 2));
		CreateMMI(InstanceType::SPHERE_HD, UsingShaderType::Wireframe, NAMEOF(mmi_spheres_hd), array_mesh_sphere_hd);

		auto array_mesh_cylinder = GeometryGenerator::CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CreateCylinderLines(14, 1, 1, Vector3_ZERO, 2));
		CreateMMI(InstanceType::CYLINDER, UsingShaderType::Wireframe, NAMEOF(mmi_cylinders), array_mesh_cylinder);

		// VOLUMETRIC

		auto array_mesh_line_volumetric = GeometryGenerator::CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::LineVertices);
		CreateMMI(InstanceType::LINE_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_cubes_volumetric), array_mesh_line_volumetric);

		auto array_mesh_cube_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_cube);
		CreateMMI(InstanceType::CUBE_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_cubes_volumetric), array_mesh_cube_volumetric);

		auto array_mesh_cube_centered_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_cube_center);
		CreateMMI(InstanceType::CUBE_CENTERED_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_cubes_centered_volumetric), array_mesh_cube_centered_volumetric);

		auto array_mesh_arrow_head_simple = GeometryGenerator::CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::ArrowheadVertices, GeometryGenerator::ArrowheadIndicesSimplified);
		auto array_mesh_arrow_head_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_arrow_head_simple);
		CreateMMI(InstanceType::ARROWHEAD_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_arrowheads_volumetric), array_mesh_arrow_head_volumetric);

		auto array_mesh_pos_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_pos);
		CreateMMI(InstanceType::POSITION_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_positions_volumetric), array_mesh_pos_volumetric);

		auto array_mesh_sphere_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_sphere, false);
		CreateMMI(InstanceType::SPHERE_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_spheres_volumetric), array_mesh_sphere_volumetric);

		auto array_mesh_sphere_hd_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_sphere_hd, false);
		CreateMMI(InstanceType::SPHERE_HD_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_spheres_hd_volumetric), array_mesh_sphere_hd_volumetric);

		auto array_mesh_cylinder_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_cylinder, false);
		CreateMMI(InstanceType::CYLINDER_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_cylinders_volumetric), array_mesh_cylinder_volumetric);

		// SOLID

		auto array_mesh_billboard = GeometryGenerator::CreateMesh(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES, GeometryGenerator::CenteredSquareVertices, GeometryGenerator::SquareIndices);
		CreateMMI(InstanceType::BILLBOARD_SQUARE, UsingShaderType::Billboard, NAMEOF(mmi_billboard_squares), array_mesh_billboard);

		set_render_layer_mask(1);
	}
}

DebugGeometryContainer::~DebugGeometryContainer() {
	LOCK_GUARD(owner->datalock);

	geometry_pool.clear_pool();
	geometry_pool.clear_pool();
}

void DebugGeometryContainer::CreateMMI(InstanceType type, UsingShaderType shader, const String &name, Ref<ArrayMesh> mesh) {
	RenderingServer *rs = RenderingServer::get_singleton();

	RID mmi = rs->instance_create();

	Ref<MultiMesh> new_mm;
	new_mm.instantiate();
	new_mm->set_name(String::num_int64((int)type));

	new_mm->set_transform_format(MultiMesh::TransformFormat::TRANSFORM_3D);
	new_mm->set_use_colors(true);
	new_mm->set_transform_format(MultiMesh::TRANSFORM_3D);
	new_mm->set_use_custom_data(true);
	new_mm->set_mesh(mesh);

	rs->instance_set_base(mmi, new_mm->get_rid());

	Ref<ShaderMaterial> mat;
	switch (shader) {
		case UsingShaderType::Wireframe:
			mat = owner->get_basic_unshaded_material();
			break;
		case UsingShaderType::Billboard:
			mat = owner->get_billboard_unshaded_material();
			break;
		case UsingShaderType::Expandable:
			mat = owner->get_extendable_material();
			break;
	}

	mesh->surface_set_material(0, mat);

	rs->instance_geometry_set_cast_shadows_setting(mmi, RenderingServer::SHADOW_CASTING_SETTING_OFF);
	rs->instance_geometry_set_flag(mmi, RenderingServer::INSTANCE_FLAG_USE_DYNAMIC_GI, false);
	rs->instance_geometry_set_flag(mmi, RenderingServer::INSTANCE_FLAG_USE_BAKED_LIGHT, false);

	multi_mesh_storage[(int)type].instance = mmi;
	multi_mesh_storage[(int)type].mesh = new_mm;
}

void DebugGeometryContainer::set_world(Node *new_world) {
	scene_world_node = new_world;
	RenderingServer *rs = RenderingServer::get_singleton();
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

// TODO add mark_dirty for 3d to reduce editor updates if only delayed shapes are displayed.
void DebugGeometryContainer::update_geometry(double delta) {
	LOCK_GUARD(owner->datalock);

	// Don't clear geometry if frozen
	if (owner->get_config()->is_freeze_3d_render())
		return;

	if (immediate_mesh_storage.mesh->get_surface_count())
		immediate_mesh_storage.mesh->clear_surfaces();

	// Return if nothing to do
	if (!owner->is_debug_enabled()) {
		for (auto &item : multi_mesh_storage) {
			if (item.mesh->get_visible_instance_count())
				item.mesh->set_visible_instance_count(0);
		}
		geometry_pool.reset_counter(delta);
		geometry_pool.reset_visible_objects();
		return;
	}

	// Update render layers
	if (render_layers != owner->get_config()->get_geometry_render_layers()) {
		set_render_layer_mask(owner->get_config()->get_geometry_render_layers());
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
		if ((owner->get_config()->is_force_use_camera_from_scene() || (!editor_viewports.size() && !owner->get_custom_viewport())) && vp_cam) {
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
		if (owner->get_config()->is_use_frustum_culling() && frustum_arrays.size()) {
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
			GeometryPoolDistanceCullingData(owner->get_config()->get_cull_by_distance(), cameras_positions));

	// Debug bounds of instances and lines
	if (owner->get_config()->is_visible_instance_bounds()) {
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
					InstanceType::SPHERE,
					0,
					Transform3D(Basis().scaled(Vector3_ONE * i.second * 2), i.first),
					Colors::debug_bounds,
					Color(),
					SphereBounds(i.first, i.second),
					[](auto d) { d->is_used_one_time = true; });
		}

		geometry_pool.for_each_line([this](DelayedRendererLine *o) {
			if (!o->is_visible || o->is_expired())
				return;

			Vector3 bottom, top, diag;
			MathUtils::get_diagonal_vectors(o->bounds.position, o->bounds.position + o->bounds.size, bottom, top, diag);

			geometry_pool.add_or_update_instance(
					InstanceType::CUBE,
					0,
					Transform3D(Basis().scaled(diag), bottom),
					Colors::debug_bounds,
					Color(),
					SphereBounds(bottom + diag * 0.5f, abs(diag.length() * 0.5f)),
					[](auto d) { d->is_used_one_time = true; });
		});
	}

	// Draw immediate lines
	geometry_pool.fill_lines_data(immediate_mesh_storage.mesh);

	// Update MultiMeshInstances
	std::array<Ref<MultiMesh> *, (int)InstanceType::ALL> meshes;
	for (int i = 0; i < (int)InstanceType::ALL; i++) {
		meshes[i] = &multi_mesh_storage[i].mesh;
	}

	geometry_pool.fill_instance_data(meshes);

	geometry_pool.scan_visible_instances();
	geometry_pool.update_expiration(delta);
	geometry_pool.reset_counter(delta);
}

Ref<DebugDrawStats3D> DebugGeometryContainer::get_render_stats() {
	LOCK_GUARD(owner->datalock);
	return geometry_pool.get_stats();
}

void DebugGeometryContainer::set_render_layer_mask(int32_t layers) {
	LOCK_GUARD(owner->datalock);
	if (render_layers != layers) {
		RenderingServer *rs = RenderingServer::get_singleton();
		for (auto &mmi : multi_mesh_storage)
			rs->instance_set_layer_mask(mmi.instance, layers);

		rs->instance_set_layer_mask(immediate_mesh_storage.instance, layers);
		render_layers = layers;
	}
}

int32_t DebugGeometryContainer::get_render_layer_mask() const {
	return render_layers;
}

void DebugGeometryContainer::clear_3d_objects() {
	LOCK_GUARD(owner->datalock);
	for (auto &s : multi_mesh_storage) {
		s.mesh->set_instance_count(0);
	}
	immediate_mesh_storage.mesh->clear_surfaces();

	geometry_pool.clear_pool();
}

#endif