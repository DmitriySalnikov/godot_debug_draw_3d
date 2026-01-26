#include "debug_draw_3d.h"

#include "config_3d.h"
#include "debug_draw_manager.h"
#include "debug_geometry_container.h"
#include "gen/shared_resources.gen.h"
#include "geometry_generators.h"
#include "nodes_container.h"
#include "stats_3d.h"
#include "utils/utils.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/world3d.hpp>

#ifndef DISABLE_DEBUG_RENDERING
#include <godot_cpp/classes/rendering_server.hpp>

// save meshes
#if !defined(DISABLE_DEBUG_RENDERING) && defined(DEV_ENABLED)
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#endif

#endif
GODOT_WARNING_RESTORE()

#define NEED_LEAVE (!_is_enabled_override())

#ifndef DISABLE_DEBUG_RENDERING
void _DD3D_WorldWatcher::_process(double p_delta) {
	set_process(false);
	if (!m_owner) {
		PRINT_WARNING("{0} is an internal DebugDraw3D node. Remove it from your scene to avoid crashes in the future.", get_name());
	}
}

void _DD3D_WorldWatcher::_notification(int p_what) {
	if ((p_what == NOTIFICATION_EXIT_WORLD || p_what == NOTIFICATION_EXIT_TREE) && m_owner) {
		DEV_PRINT_STD_F(NAMEOF(_DD3D_WorldWatcher) " (notif): World3D (%" PRIu64 "), What: %s\n", m_world_id, p_what == NOTIFICATION_EXIT_WORLD ? NAMEOF(NOTIFICATION_EXIT_WORLD) : NAMEOF(NOTIFICATION_EXIT_TREE));
		m_owner->_remove_debug_container(m_world_id);
		m_owner = nullptr;

		if (!is_queued_for_deletion()) {
			if (auto *p = get_parent(); p) {
				p->call_deferred(NAMEOF(remove_child), this);
			}
			queue_free();
		}
	}
}

Node *_DD3D_WorldWatcher::get_nodes_root() const {
	return m_nodes_root;
}

_DD3D_WorldWatcher::_DD3D_WorldWatcher(DebugDraw3D *p_root, uint64_t p_world_id) {
	m_owner = p_root;
	m_world_id = p_world_id;
	m_nodes_root = memnew(Node);

	call_deferred("add_child", m_nodes_root);
}

DebugDraw3D::ViewportToDebugContainerItem::ViewportToDebugContainerItem() :
		world_id(0), world_watcher(nullptr), dgcs(), ncs() {
}

DebugDraw3D::ViewportToDebugContainerItem::ViewportToDebugContainerItem(ViewportToDebugContainerItem &&other) noexcept
		: world_id(std::exchange(other.world_id, 0)),
		  world_watcher(nullptr) {
	for (size_t i = 0; i < (int)MeshMaterialVariant::MAX; i++) {
		dgcs[i] = std::move(other.dgcs[i]);
		ncs[i] = std::move(other.ncs[i]);
	}
}

DebugDraw3D::ViewportToDebugContainerItem::~ViewportToDebugContainerItem() {
	for (auto &i : dgcs)
		i.reset();

	for (auto &i : ncs)
		i.reset();
}
#endif

DebugDraw3D *DebugDraw3D::singleton = nullptr;

void DebugDraw3D::_bind_methods() {
#define REG_CLASS_NAME DebugDraw3D

#pragma region Parameters

	// HACK PROPERTY_USAGE_NONE disable "Instantiated OBJECT used as default value" warning
	REG_PROP(empty_color, Variant::COLOR, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);
	REG_PROP_BOOL(debug_enabled, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);

	REG_PROP(config, Variant::OBJECT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);

#pragma endregion

#if !defined(DISABLE_DEBUG_RENDERING) && defined(DEV_ENABLED)
	ClassDB::bind_method(D_METHOD(NAMEOF(_save_generated_meshes)), &DebugDraw3D::_save_generated_meshes);
#endif

#pragma region Draw Functions
	ClassDB::bind_method(D_METHOD(NAMEOF(regenerate_geometry_meshes)), &DebugDraw3D::regenerate_geometry_meshes);
	ClassDB::bind_method(D_METHOD(NAMEOF(clear_all)), &DebugDraw3D::clear_all);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_sphere), "position", "radius", "color", "duration"), &DebugDraw3D::draw_sphere, 0.5f, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_sphere_xf), "transform", "color", "duration"), &DebugDraw3D::draw_sphere_xf, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_cylinder), "transform", "color", "duration"), &DebugDraw3D::draw_cylinder, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_cylinder_ab), "a", "b", "radius", "color", "duration"), &DebugDraw3D::draw_cylinder_ab, 0.5f, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_box), "position", "rotation", "size", "color", "is_box_centered", "duration"), &DebugDraw3D::draw_box, Colors::empty_color, false, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_box_ab), "a", "b", "up", "color", "is_ab_diagonal", "duration"), &DebugDraw3D::draw_box_ab, Vector3(0, 1, 0), Colors::empty_color, true, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_box_xf), "transform", "color", "is_box_centered", "duration"), &DebugDraw3D::draw_box_xf, Colors::empty_color, true, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_aabb), "aabb", "color", "duration"), &DebugDraw3D::draw_aabb, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_aabb_ab), "a", "b", "color", "duration"), &DebugDraw3D::draw_aabb_ab, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_line_hit), "start", "end", "hit", "is_hit", "hit_size", "hit_color", "after_hit_color", "duration"), &DebugDraw3D::draw_line_hit, 0.25f, Colors::empty_color, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_line_hit_offset), "start", "end", "is_hit", "unit_offset_of_hit", "hit_size", "hit_color", "after_hit_color", "duration"), &DebugDraw3D::draw_line_hit_offset, 0.5f, 0.25f, Colors::empty_color, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_line), "a", "b", "color", "duration"), &DebugDraw3D::draw_line, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_lines), "lines", "color", "duration"), &DebugDraw3D::draw_lines, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_ray), "origin", "direction", "length", "color", "duration"), &DebugDraw3D::draw_ray, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_line_path), "path", "color", "duration"), &DebugDraw3D::draw_line_path, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_arrowhead), "transform", "color", "duration"), &DebugDraw3D::draw_arrowhead, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_arrow), "a", "b", "color", "arrow_size", "is_absolute_size", "duration"), &DebugDraw3D::draw_arrow, Colors::empty_color, 0.5f, false, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_arrow_ray), "origin", "direction", "length", "color", "arrow_size", "is_absolute_size", "duration"), &DebugDraw3D::draw_arrow_ray, Colors::empty_color, 0.5f, false, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_arrow_path), "path", "color", "arrow_size", "is_absolute_size", "duration"), &DebugDraw3D::draw_arrow_path, Colors::empty_color, 0.75f, true, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_point_path), "path", "type", "size", "points_color", "lines_color", "duration"), &DebugDraw3D::draw_point_path, PointType::POINT_TYPE_SQUARE, 0.25f, Colors::empty_color, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_square), "position", "size", "color", "duration"), &DebugDraw3D::draw_square, 0.2f, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_plane), "plane", "color", "anchor_point", "duration"), &DebugDraw3D::draw_plane, Colors::empty_color, Vector3_INF, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_points), "points", "type", "size", "color", "duration"), &DebugDraw3D::draw_points, PointType::POINT_TYPE_SQUARE, 0.2f, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_camera_frustum), "camera", "color", "duration"), &DebugDraw3D::draw_camera_frustum, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_camera_frustum_planes), "camera_frustum", "color", "duration"), &DebugDraw3D::draw_camera_frustum_planes, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_position), "transform", "color", "duration"), &DebugDraw3D::draw_position, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_gizmo), "transform", "color", "is_centered", "duration"), &DebugDraw3D::draw_gizmo, Colors::empty_color, false, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_grid), "origin", "x_size", "y_size", "subdivision", "color", "is_centered", "duration"), &DebugDraw3D::draw_grid, Colors::empty_color, true, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_grid_xf), "transform", "subdivision", "color", "is_centered", "duration"), &DebugDraw3D::draw_grid_xf, Colors::empty_color, true, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_text), "position", "text", "size", "color", "duration"), &DebugDraw3D::draw_text, 32, Colors::empty_color, 0);

#pragma endregion // Draw Functions

	REG_METHOD(get_render_stats);
	REG_METHOD(get_render_stats_for_world, "viewport");
	REG_METHOD(new_scoped_config);
	REG_METHOD(scoped_config);

#undef REG_CLASS_NAME

	BIND_ENUM_CONSTANT(POINT_TYPE_SQUARE);
	BIND_ENUM_CONSTANT(POINT_TYPE_SPHERE);
}

DebugDraw3D::DebugDraw3D() {
	ASSIGN_SINGLETON(DebugDraw3D);
}

void DebugDraw3D::init(DebugDrawManager *p_root) {
	ZoneScoped;
	root_node = p_root;
	set_config(nullptr);

	root_settings_section = String(Utils::root_settings_section) + "3d/";
	DEFINE_SETTING(root_settings_section + s_add_bevel_to_volumetric, true, Variant::BOOL);
	DEFINE_SETTING(root_settings_section + s_use_icosphere, false, Variant::BOOL);
	DEFINE_SETTING(root_settings_section + s_use_icosphere_hd, true, Variant::BOOL);
	DEFINE_SETTING_AND_GET_HINT(real_t def_frustum_scale, root_settings_section + s_default_frustum_scale, 0.5f, Variant::FLOAT, PROPERTY_HINT_RANGE, "0,1,0.0001");

	DEFINE_SETTING_AND_GET_HINT(real_t def_thickness, root_settings_section + s_default_thickness, 0.05f, Variant::FLOAT, PROPERTY_HINT_RANGE, "0,100,0.0001,or_greater");
	DEFINE_SETTING_AND_GET_HINT(real_t def_brightness, root_settings_section + s_default_center_brightness, 0.8f, Variant::FLOAT, PROPERTY_HINT_RANGE, "0,1,0.0001");
	DEFINE_SETTING_AND_GET(real_t def_hd_sphere, root_settings_section + s_default_hd_spheres, false, Variant::BOOL);
	DEFINE_SETTING_AND_GET_HINT(real_t def_plane_size, root_settings_section + s_default_plane_size, 0, Variant::FLOAT, PROPERTY_HINT_RANGE, "0,10000,0.001,or_greater");

	DEFINE_SETTING(root_settings_section + s_render_priority, 0, Variant::INT);
	DEFINE_SETTING_HINT(root_settings_section + s_render_mode, 0, Variant::INT, PROPERTY_HINT_ENUM, "Default,Forced Transparent,Forced Opaque");
	DEFINE_SETTING(root_settings_section + s_render_fog_disabled, true, Variant::BOOL);

	default_scoped_config.instantiate();

	config->set_frustum_length_scale(def_frustum_scale);

	default_scoped_config->set_thickness(def_thickness);
	default_scoped_config->set_center_brightness(def_brightness);
	default_scoped_config->set_hd_sphere(def_hd_sphere);
	default_scoped_config->set_plane_size(def_plane_size == 0 ? INFINITY : def_plane_size);

	_load_materials();
}

DebugDraw3D::~DebugDraw3D() {
	ZoneScoped;
	UNASSIGN_SINGLETON(DebugDraw3D);

	root_node = nullptr;
}

void DebugDraw3D::process_start(double delta) {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	LOCK_GUARD(datalock);

#endif
}

void DebugDraw3D::process_end(double p_delta) {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	FrameMarkStart("3D Update");
	LOCK_GUARD(datalock);

	// Update 3D debug
	for (const auto &p : debug_containers) {
		for (const auto &dgc : p.second.dgcs) {
			if (dgc) {
				dgc->update_geometry(p_delta);
			}
		}
		for (const auto &nc : p.second.ncs) {
			if (nc) {
				nc->update_geometry(p_delta);
			}
		}
	}

	_clear_scoped_configs();
	// Reset viewport cache after frame
	viewport_to_world_cache.clear();
	FrameMarkEnd("3D Update");
#endif
}

void DebugDraw3D::physics_process_start(double p_delta) {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	FrameMarkStart("3D Physics Step");

	LOCK_GUARD(datalock);
	for (const auto &p : debug_containers) {
		for (const auto &dgc : p.second.dgcs) {
			if (dgc) {
				dgc->update_geometry_physics_start(p_delta);
			}
		}
		for (const auto &nc : p.second.ncs) {
			if (nc) {
				nc->update_geometry_physics_start(p_delta);
			}
		}
	}
#endif
}

void DebugDraw3D::physics_process_end(double p_delta) {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	LOCK_GUARD(datalock);

	for (const auto &p : debug_containers) {
		for (const auto &dgc : p.second.dgcs) {
			if (dgc) {
				dgc->update_geometry_physics_end(p_delta);
			}
		}
		for (const auto &nc : p.second.ncs) {
			if (nc) {
				nc->update_geometry_physics_end(p_delta);
			}
		}
	}

	FrameMarkEnd("3D Physics Step");
#endif
}

#ifndef DISABLE_DEBUG_RENDERING
const DebugDraw3DScopeConfig::Data *DebugDraw3D::scoped_config_for_current_thread() {
	ZoneScoped;
	LOCK_GUARD(datalock);
	uint64_t thread = OS::get_singleton()->get_thread_caller_id();

	if (const auto &it = cached_scoped_configs.find(thread); it != cached_scoped_configs.cend()) {
		return it->second.get();
	}

	if (const auto &it = scoped_configs.find(thread); it != scoped_configs.cend()) {
		const auto &cfgs = it->second;
		if (!cfgs.empty()) {
			DebugDraw3DScopeConfig *tmp = cfgs.back().scfg;
			cached_scoped_configs[thread] = tmp->data;
			return tmp->data.get();
		}
	}

	cached_scoped_configs[thread] = default_scoped_config.ptr()->data;
	return default_scoped_config.ptr()->data.get();
}

void DebugDraw3D::_register_scoped_config(uint64_t p_thread_id, uint64_t p_guard_id, DebugDraw3DScopeConfig *p_cfg) {
	ZoneScoped;
	LOCK_GUARD(datalock);

	uint64_t thread = OS::get_singleton()->get_thread_caller_id();
	scoped_configs[p_thread_id].push_back(ScopedPairIdConfig(p_guard_id, p_cfg));

	// Update cached value
	cached_scoped_configs[thread] = p_cfg->data;
}

void DebugDraw3D::_unregister_scoped_config(uint64_t thread_id, uint64_t guard_id) {
	ZoneScoped;
	LOCK_GUARD(datalock);

	auto &cfgs = scoped_configs[thread_id];
	auto res = std::find_if(cfgs.rbegin(), cfgs.rend(), [&guard_id](const ScopedPairIdConfig &i) { return i.id == guard_id; });

	if (res != cfgs.rend()) {
		cfgs.erase(--res.base());

		// Update cached value
		if (!cfgs.empty()) {
			cached_scoped_configs[thread_id] = cfgs.back().scfg->data;
		} else {
			cached_scoped_configs[thread_id] = default_scoped_config.ptr()->data;
		}
	}
}

void DebugDraw3D::_clear_scoped_configs() {
	ZoneScoped;
	LOCK_GUARD(datalock);

	uint64_t orphans = 0;
	for (auto &p : scoped_configs) {
		orphans += p.second.size();
	}

	scoped_stats_3d.created = created_scoped_configs;
	scoped_stats_3d.orphans = orphans;

	created_scoped_configs = 0;

	cached_scoped_configs.clear();
	scoped_configs.clear();

	if (orphans)
		PRINT_ERROR("{0} scoped configs weren't freed. Do not save scoped configurations anywhere other than function bodies.", orphans);
}

Node *DebugDraw3D::get_root_node() {
	return root_node;
}

std::array<Ref<ArrayMesh>, 2> *DebugDraw3D::get_shared_meshes() {
	LOCK_GUARD(datalock);
	if (!shared_generated_meshes.size()) {
		bool p_add_bevel = PS()->get_setting(root_settings_section + s_add_bevel_to_volumetric);
		bool p_use_icosphere = PS()->get_setting(root_settings_section + s_use_icosphere);
		bool p_use_icosphere_hd = PS()->get_setting(root_settings_section + s_use_icosphere_hd);

		shared_generated_meshes.resize((int)InstanceType::MAX);
		for (int i = 0; i < (int)MeshMaterialVariant::MAX; i++) {
			MeshMaterialType mat_type = MeshMaterialType::Wireframe;

#define GEN_MESH(_type, _gen)                      \
	shared_generated_meshes[(int)_type][i] = _gen; \
	shared_generated_meshes[(int)_type][i]->surface_set_material(0, get_material_variant(mat_type, (MeshMaterialVariant)i));

			// WIREFRAME

			mat_type = MeshMaterialType::Wireframe;
			GEN_MESH(InstanceType::CUBE, GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CubeVertexes, GeometryGenerator::CubeIndexes));
			GEN_MESH(InstanceType::CUBE_CENTERED, GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CenteredCubeVertexes, GeometryGenerator::CubeIndexes));
			GEN_MESH(InstanceType::ARROWHEAD, GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::ArrowheadVertexes, GeometryGenerator::ArrowheadIndexes));
			GEN_MESH(InstanceType::POSITION, GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::PositionVertexes, GeometryGenerator::PositionIndexes));
			GEN_MESH(InstanceType::SPHERE, p_use_icosphere ? GeometryGenerator::CreateIcosphereLines(0.5f, 1) : GeometryGenerator::CreateSphereLines(8, 8, 0.5f, 2));
			GEN_MESH(InstanceType::SPHERE_HD, p_use_icosphere_hd ? GeometryGenerator::CreateIcosphereLines(0.5f, 2) : GeometryGenerator::CreateSphereLines(16, 16, 0.5f, 2));
			GEN_MESH(InstanceType::CYLINDER, GeometryGenerator::CreateCylinderLines(32, 1, 1, 4));

			// VOLUMETRIC

			mat_type = MeshMaterialType::Extendable;
			GEN_MESH(InstanceType::LINE_VOLUMETRIC, GeometryGenerator::ConvertWireframeToVolumetric(GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::LineVertexes), p_add_bevel));
			GEN_MESH(InstanceType::CUBE_VOLUMETRIC, GeometryGenerator::ConvertWireframeToVolumetric(shared_generated_meshes[(int)InstanceType::CUBE][i], p_add_bevel));
			GEN_MESH(InstanceType::CUBE_CENTERED_VOLUMETRIC, GeometryGenerator::ConvertWireframeToVolumetric(shared_generated_meshes[(int)InstanceType::CUBE_CENTERED][i], p_add_bevel));
			GEN_MESH(InstanceType::ARROWHEAD_VOLUMETRIC, GeometryGenerator::CreateVolumetricArrowHead(.25f, 1.f, 1.f, p_add_bevel));
			GEN_MESH(InstanceType::POSITION_VOLUMETRIC, GeometryGenerator::ConvertWireframeToVolumetric(shared_generated_meshes[(int)InstanceType::POSITION][i], p_add_bevel));
			GEN_MESH(InstanceType::SPHERE_VOLUMETRIC, GeometryGenerator::ConvertWireframeToVolumetric(shared_generated_meshes[(int)InstanceType::SPHERE][i], false));
			GEN_MESH(InstanceType::SPHERE_HD_VOLUMETRIC, GeometryGenerator::ConvertWireframeToVolumetric(shared_generated_meshes[(int)InstanceType::SPHERE_HD][i], false));
			GEN_MESH(InstanceType::CYLINDER_VOLUMETRIC, GeometryGenerator::ConvertWireframeToVolumetric(shared_generated_meshes[(int)InstanceType::CYLINDER][i], false));

			// SOLID

			mat_type = MeshMaterialType::Billboard;
			GEN_MESH(InstanceType::BILLBOARD_SQUARE, GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES, GeometryGenerator::CenteredSquareVertexes, GeometryGenerator::SquareBackwardsIndexes));

			mat_type = MeshMaterialType::Plane;
			GEN_MESH(InstanceType::PLANE, GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES, GeometryGenerator::CenteredSquareVertexes, GeometryGenerator::SquareIndexes));
#undef GEN_MESH
		}
	}

	return shared_generated_meshes.data();
}

DebugDraw3D::ViewportToDebugContainerItem *DebugDraw3D::get_debug_container(const DebugDraw3DScopeConfig::DebugContainerDependent &p_dgcd, const bool p_generate_new_container) {
	ZoneScoped;
	LOCK_GUARD(datalock);

	if (!p_dgcd.viewport) {
		return nullptr;
	}

	int dgc_depth = !!p_dgcd.no_depth_test;

	if (const auto &cached_world = viewport_to_world_cache.find(p_dgcd.viewport);
			cached_world != viewport_to_world_cache.end() && cached_world->second->dgcs[dgc_depth]) {
		return cached_world->second;
	}

	Ref<World3D> vp_world;

	// find_world_3d is not available from user threads. Make a deferred request and just wait.
	if (auto *os = OS::get_singleton(); os->get_thread_caller_id() != os->get_main_thread_id()) {
		const auto &p = world3ds_found_for_threads_cache.find(p_dgcd.viewport_id);
		if (p == world3ds_found_for_threads_cache.end()) {
			// PRINT_WARNING("DebugDraw3D cannot search for World3D outside of the main thread.\nAn attempt will be made to search for World3D using deferred call.");
			world3ds_found_for_threads_cache[p_dgcd.viewport_id] = Ref<World3D>();
			callable_mp(this, &DebugDraw3D::_deferred_find_world_in_viewport).call_deferred(p_dgcd.viewport->get_instance_id());
			return nullptr;
		} else {
			if (p->second.is_null()) {
				return nullptr;
			} else {
				vp_world = p->second;
			}
		}
	} else {
		vp_world = p_dgcd.viewport->find_world_3d();
	}

	if (vp_world.is_null()) {
		PRINT_WARNING("DebugDraw3D cannot find World3D. It is not possible to draw a debugging geometry.");
		return nullptr;
	}

	uint64_t vp_world_id = vp_world->get_instance_id();

	if (const auto &dgc_pair = debug_containers.find(vp_world_id);
			dgc_pair != debug_containers.end() && dgc_pair->second.dgcs[dgc_depth]) {
		viewport_to_world_cache[p_dgcd.viewport] = &dgc_pair->second;
		return &dgc_pair->second;
	}

	if (!p_generate_new_container) {
		return nullptr;
	}

	// create or update container storage
	auto &c = debug_containers[vp_world_id];

	// on new storage created
	if (!c.world_watcher) {
		// remove cached references to avoid crashes in case of invalidation of `debug_containers`
		viewport_to_world_cache.clear();

		// _DD3D_WorldWatcher has to be cleared by the SceenTree or manually in case of an error.
		c.world_watcher = memnew(_DD3D_WorldWatcher(this, vp_world_id));

		callable_mp(this, &DebugDraw3D::_register_viewport_world_deferred)
				.call_deferred(p_dgcd.viewport_id, vp_world_id, c.world_watcher);
	}

	c.world_id = vp_world_id;
	c.dgcs[dgc_depth] = std::make_unique<DebugGeometryContainer>(this, p_dgcd.no_depth_test);
	c.dgcs[dgc_depth]->set_world(vp_world);

	c.ncs[dgc_depth] = std::make_unique<NodesContainer>(this, c.world_watcher->get_nodes_root(), p_dgcd.no_depth_test);

	viewport_to_world_cache[p_dgcd.viewport] = &c;

	return &c;
}

void DebugDraw3D::_deferred_find_world_in_viewport(uint64_t p_viewport_id) {
	const Viewport *vp = Object::cast_to<Viewport>(ObjectDB::get_instance(p_viewport_id));
	if (vp) {
		world3ds_found_for_threads_cache[p_viewport_id] = vp->find_world_3d();
	}
}

void DebugDraw3D::_register_viewport_world_deferred(uint64_t /*Viewport * */ p_viewport_id, const uint64_t p_world_id, _DD3D_WorldWatcher *watcher) {
	ZoneScoped;

	Node *scene_root = root_node->get_current_scene();
	Viewport *viewport = cast_to<Viewport>(ObjectDB::get_instance(p_viewport_id));
	Node *parent_node = _get_root_world_node(scene_root, viewport);

	// something failed. need to remove container
	if (!parent_node || parent_node->is_queued_for_deletion() || !watcher) {
		DEV_PRINT_STD_F(NAMEOF(_DD3D_WorldWatcher) " (register): Failed to register WorldWatcher for World3D (%" PRIu64 ").\n", p_world_id);
		_remove_debug_container(p_world_id);
		if (watcher) {
			watcher->queue_free();
		}
		return;
	}

	DEV_PRINT_STD_F(NAMEOF(_DD3D_WorldWatcher) " (register): Registered WorldWatcher for World3D (%" PRIu64 ").\n", p_world_id);
	parent_node->add_child(watcher);
	parent_node->move_child(watcher, 0);
}

Node *DebugDraw3D::_get_root_world_node(Node *p_scene_root, Viewport *p_vp) {
	Node *parent = p_vp->get_parent();
	if (!parent) {
		goto end;
	}

	{
		Viewport *parent_vp = parent->get_viewport();
		if (!parent_vp) {
			goto end;
		}

		if (p_vp->find_world_3d() == parent_vp->find_world_3d()) {
			return _get_root_world_node(p_scene_root, parent_vp);
		}
	}

end:
	/*
	 * Godot 4.3 in the editor will crash when you select an area with text in the viewport, if the node is added to the root of the editor.
	 * Caused by [#92188](https://github.com/godotengine/godot/pull/92188) because it does not check whether the node has an owner.
	 * Fixed by [#95420](https://github.com/godotengine/godot/pull/95420) in Godot 4.3.1
	 *
	 * But this code should fix the problem in any version of the engine by attaching WorldWatcher to the current scene.
	 */

#ifdef TOOLS_ENABLED
	if (IS_EDITOR_HINT()) {
		if (!p_scene_root) {
			return nullptr;
		}

		if (!p_scene_root->is_ancestor_of(p_vp)) {
			DEV_PRINT_STD_F(NAMEOF(_DD3D_WorldWatcher) " (get_root): Viewport with World3D (%" PRIu64 ") does not belong to the current scene: %s\nUsing the root of the current scene: %s\n",
					p_vp->find_world_3d()->get_instance_id(),
					String("\n\t").join(String(p_vp->get_path()).split("/")).utf8().get_data(),
					String("\n\t").join(String(p_scene_root->get_path()).split("/")).utf8().get_data());
			return p_scene_root;
		}
	}
#endif

	DEV_PRINT_STD_F(NAMEOF(_DD3D_WorldWatcher) " (get_root): This Viewport with World3D (%" PRIu64 ") will be used as the parent for the WorldWatcher: %s\n",
			p_vp->find_world_3d()->get_instance_id(),
			String("\n\t").join(String(p_vp->get_path()).split("/")).utf8().get_data());
	return p_vp;
}

void DebugDraw3D::_remove_debug_container(const uint64_t &p_world_id) {
	ZoneScoped;
	LOCK_GUARD(datalock);

	if (const auto &dgc = debug_containers.find(p_world_id);
			dgc != debug_containers.end()) {
		DEV_PRINT_STD_F(NAMEOF(_DD3D_WorldWatcher) " (remove): World3D (%" PRIu64 ") is no longer in use, its storage will be deleted.\n", p_world_id);
		debug_containers.erase(dgc);

		// remove cached references to avoid crashes in case of invalidation of `debug_containers`
		viewport_to_world_cache.clear();
		// also clear the world3d cache for threads
		world3ds_found_for_threads_cache.clear();
	}
}

#endif

Ref<DebugDraw3DScopeConfig> DebugDraw3D::new_scoped_config() {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	LOCK_GUARD(datalock);
	static uint64_t create_counter = 0;
	create_counter++;

	uint64_t thread = OS::get_singleton()->get_thread_caller_id();
	auto unreg_func = [this](const uint64_t &p_thread_id, const uint64_t &p_guard_id) {
		_unregister_scoped_config(p_thread_id, p_guard_id);
	};
	Ref<DebugDraw3DScopeConfig> res(memnew(
			DebugDraw3DScopeConfig(
					thread,
					create_counter,
					scoped_config_for_current_thread(),
					unreg_func)));

	_register_scoped_config(thread, create_counter, res.ptr());
	created_scoped_configs++;
	return res;
#else
	return default_scoped_config;
#endif
}

Ref<DebugDraw3DScopeConfig> DebugDraw3D::scoped_config() {
	return default_scoped_config;
}

void DebugDraw3D::_load_materials() {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
#define LOAD_SHADER(mat, source)                   \
	{                                              \
		Ref<Shader> code;                          \
		code.instantiate();                        \
		code->set_code(source);                    \
		mat.instantiate();                         \
		mat->set_shader(code);                     \
		mat->set_render_priority(render_priority); \
	}

	int render_priority = PS()->get_setting(root_settings_section + s_render_priority);
	int render_mode = PS()->get_setting(root_settings_section + s_render_mode); // default, transparent, opaque
	bool fog_disabled = (bool)PS()->get_setting(root_settings_section + s_render_fog_disabled);

	for (int variant = 0; variant < (int)MeshMaterialVariant::MAX; variant++) {
		String prefix = "";
		if (variant == (int)MeshMaterialVariant::NoDepth) {
			prefix += "#define NO_DEPTH\n";
		}

		switch (render_mode) {
			case 0:
				break;
			case 1:
				prefix += "#define FORCED_TRANSPARENT\n";
				break;
			case 2:
				prefix += "#define FORCED_OPAQUE\n";
				break;
		}

		if (fog_disabled) {
			prefix += "#define FOG_DISABLED\n";
		}

#ifdef DISABLE_SHADER_WORLD_COORDS
		prefix += "#define NO_WORLD_COORD\n";
#endif

		LOAD_SHADER(mesh_shaders[(int)MeshMaterialType::Wireframe][variant], prefix + DD3DResources::src_resources_wireframe_unshaded_gdshader);
		LOAD_SHADER(mesh_shaders[(int)MeshMaterialType::Billboard][variant], prefix + DD3DResources::src_resources_billboard_unshaded_gdshader);
		LOAD_SHADER(mesh_shaders[(int)MeshMaterialType::Plane][variant], prefix + DD3DResources::src_resources_plane_unshaded_gdshader);
		LOAD_SHADER(mesh_shaders[(int)MeshMaterialType::Extendable][variant], prefix + DD3DResources::src_resources_extendable_meshes_gdshader);
	}
#undef LOAD_SHADER
#endif
}

bool DebugDraw3D::_is_enabled_override() const {
	return debug_enabled && DebugDrawManager::get_singleton()->is_debug_enabled();
}

void DebugDraw3D::set_custom_editor_viewport(std::vector<SubViewport *> viewports) {
	custom_editor_viewports = viewports;
}

std::vector<SubViewport *> DebugDraw3D::get_custom_editor_viewports() {
	return custom_editor_viewports;
}

Ref<ShaderMaterial> DebugDraw3D::get_material_variant(MeshMaterialType p_type, MeshMaterialVariant p_var) {
#ifndef DISABLE_DEBUG_RENDERING
	return mesh_shaders[(int)p_type][(int)p_var];
#else
	return Ref<ShaderMaterial>();
#endif
}

Color DebugDraw3D::get_empty_color() const {
	return Colors::empty_color;
}

#pragma region Exposed Parameters

void DebugDraw3D::set_debug_enabled(const bool &state) {
	if (debug_enabled != state && !state) {
		clear_all();
	}

	debug_enabled = state;
}

bool DebugDraw3D::is_debug_enabled() const {
	return debug_enabled;
}

void DebugDraw3D::set_config(Ref<DebugDraw3DConfig> cfg) {
	if (cfg.is_valid()) {
		config = cfg;
	} else {
		config = Ref<DebugDraw3DConfig>();
		config.instantiate();
	}
}

Ref<DebugDraw3DConfig> DebugDraw3D::get_config() const {
	return config;
}

#pragma endregion

#pragma region Draw Functions

Ref<DebugDraw3DStats> DebugDraw3D::get_render_stats() {
	Ref<DebugDraw3DStats> res;
	res.instantiate();
#ifndef DISABLE_DEBUG_RENDERING
	LOCK_GUARD(datalock);

	Ref<DebugDraw3DStats> stats_3d;
	stats_3d.instantiate();

	for (const auto &p : debug_containers) {
		for (const auto &dgc : p.second.dgcs) {
			if (dgc) {
				dgc->get_render_stats(stats_3d);
				res->combine_with(stats_3d);
			}
		}
	}

	for (const auto &p : debug_containers) {
		for (const auto &nc : p.second.ncs) {
			if (nc) {
				nc->get_render_stats(stats_3d);
				res->combine_with(stats_3d);
			}
		}
	}

	res->set_scoped_config_stats(scoped_stats_3d.created, scoped_stats_3d.orphans);
#endif
	return res;
}

Ref<DebugDraw3DStats> DebugDraw3D::get_render_stats_for_world(Viewport *viewport) {
	Ref<DebugDraw3DStats> res;
	res.instantiate();
#ifndef DISABLE_DEBUG_RENDERING
	LOCK_GUARD(datalock);

	Ref<DebugDraw3DStats> stats_3d;
	stats_3d.instantiate();

	for (int i = 0; i < 2; i++) {
		auto vdc = get_debug_container(DebugDraw3DScopeConfig::DebugContainerDependent{ viewport, viewport ? viewport->get_instance_id() : 0, i > 0 }, false);
		if (!vdc)
			continue;

		auto dgc = vdc->dgcs[!!(i > 0)].get();
		if (dgc) {
			dgc->get_render_stats(stats_3d);
			res->combine_with(stats_3d);
		}
	}
#endif
	return res;
}

void DebugDraw3D::regenerate_geometry_meshes() {
#ifndef DISABLE_DEBUG_RENDERING
	LOCK_GUARD(datalock);

	// Reload materials
	_load_materials();

	// Force regenerate meshes
	shared_generated_meshes.clear();

	for (auto &p : debug_containers) {
		for (int i = 0; i < 2; i++) {
			auto &dgc = p.second.dgcs[i];
			if (dgc) {
				Ref<World3D> old_world = dgc->get_world();
				bool is_no_depth = dgc->is_no_depth_test();

				dgc = std::make_unique<DebugGeometryContainer>(this, is_no_depth);
				dgc->set_world(old_world);
			}
		}
	}
#endif
}

void DebugDraw3D::clear_all() {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	LOCK_GUARD(datalock);

	for (auto &p : debug_containers) {
		// p.second.world_watcher &&
		if (!p.second.world_watcher->is_queued_for_deletion()) {
			// must be freed in anyway
			p.second.world_watcher->queue_free();

			if (Node *parent = p.second.world_watcher->get_parent(); parent) {
				// should be auto freed on exit_tree notif
				// must be deferred to avoid `debug_containers` invalidation
				parent->call_deferred(NAMEOF(remove_child), p.second.world_watcher);
			}
		}
	}

	debug_containers.clear();
	viewport_to_world_cache.clear();
	world3ds_found_for_threads_cache.clear();
#else
	return;
#endif
}

#ifndef DISABLE_DEBUG_RENDERING
#define IS_DEFAULT_COLOR(name) (name == Colors::empty_color)
#define CHECK_BEFORE_CALL()                          \
	if (NEED_LEAVE || config->is_freeze_3d_render()) \
		return;

#define GET_SCOPED_CFG_AND_VDC()                     \
	auto scfg = scoped_config_for_current_thread();  \
	auto vdc = get_debug_container(scfg->dcd, true); \
	if (!vdc)                                        \
		return;

#define GET_SCOPED_CFG_AND_DGC()                           \
	GET_SCOPED_CFG_AND_VDC();                              \
	auto dgc = vdc->dgcs[!!scfg->dcd.no_depth_test].get(); \
	if (!dgc)                                              \
		return;

#define GET_SCOPED_CFG_AND_NC()                          \
	GET_SCOPED_CFG_AND_VDC();                            \
	auto nc = vdc->ncs[!!scfg->dcd.no_depth_test].get(); \
	if (!nc)                                             \
		return;

#ifdef DEV_ENABLED
void DebugDraw3D::_save_generated_meshes() {
	if (!shared_generated_meshes.size())
		return;

	for (int i = 0; i < 2; i++) {
		for (int type = 0; type < (int)InstanceType::MAX; type++) {
			Ref<ArrayMesh> mesh = shared_generated_meshes[type][i];
			String dir_path = FMT_STR("res://debug_meshes/{0}", i == 0 ? "normal" : "no_depth");
			DirAccess::make_dir_recursive_absolute(dir_path);
			ResourceSaver::get_singleton()->save(mesh, FMT_STR("{0}/{1}.mesh", dir_path, type), ResourceSaver::SaverFlags::FLAG_BUNDLE_RESOURCES | ResourceSaver::SaverFlags::FLAG_REPLACE_SUBRESOURCE_PATHS);
		}
	}
}
#endif

Vector3 DebugDraw3D::get_up_vector(const Vector3 &p_dir) {
	Vector3 n = p_dir.abs().normalized();
	Vector3::Axis mi = n.max_axis_index();

	if (n[mi] == 0)
		return Vector3_UP;

	switch (mi) {
		case Vector3::Axis::AXIS_X:
		case Vector3::Axis::AXIS_Z:
			return Vector3_UP;
		case Vector3::Axis::AXIS_Y:
			return Vector3_FORWARD;
	}

	return Vector3_UP;
}

void DebugDraw3D::add_or_update_line_with_thickness(real_t p_exp_time, const Vector3 *p_lines, const size_t p_line_count, const Color &p_col, const std::function<void(DelayedRendererLine *)> p_custom_upd) {
	ZoneScoped;

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_DGC();

	if (!scfg->thickness) {
		AABB aabb = MathUtils::calculate_vertex_bounds(p_lines, p_line_count);
		dgc->geometry_pool.add_or_update_line(
				scfg,
				p_exp_time,
				p_lines,
				p_line_count,
				p_col,
				aabb);
	} else {
		for (int i = 0; i < p_line_count; i += 2) {
			ZoneScopedN("Convert AB to xf");
			Vector3 a = p_lines[i];
			Vector3 diff = p_lines[i + 1] - a;
			real_t len = diff.length();

			if (Math::is_zero_approx(len))
				continue;

			dgc->geometry_pool.add_or_update_instance(
					scfg,
					InstanceType::LINE_VOLUMETRIC,
					p_exp_time,
					Transform3D(Basis().looking_at(diff, get_up_vector(diff)).scaled(VEC3_ONE(len)), a), // slow
					p_col,
					SphereBounds(a + diff * .5f, len * .5f));
		}
	}
}

#pragma region Spheres

void DebugDraw3D::draw_sphere_base(const Transform3D &transform, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_DGC();

	dgc->geometry_pool.add_or_update_instance(
			scfg,
			ConvertableInstanceType::SPHERE,
			duration,
			transform,
			IS_DEFAULT_COLOR(color) ? Colors::chartreuse : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * 0.5f));
}

void DebugDraw3D::draw_sphere(const Vector3 &position, const real_t &radius, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();
	real_t scale = radius * 2;
	draw_sphere_base(Transform3D(Basis().scaled(VEC3_ONE(scale)), position), color, duration);
}

void DebugDraw3D::draw_sphere_xf(const Transform3D &transform, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();
	draw_sphere_base(transform, color, duration);
}

#pragma endregion // Spheres
#pragma region Cylinders

void DebugDraw3D::draw_cylinder(const Transform3D &transform, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_DGC();

	dgc->geometry_pool.add_or_update_instance(
			scfg,
			ConvertableInstanceType::CYLINDER,
			duration,
			transform,
			IS_DEFAULT_COLOR(color) ? Colors::forest_green : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::CylinderRadiusForSphere));
}

void DebugDraw3D::draw_cylinder_ab(const Vector3 &a, const Vector3 &b, const real_t &radius, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	// TODO: maybe someone knows a better way to solve it?
	Vector3 diff = b - a;
	real_t len = diff.length();
	if (Math::is_zero_approx(len))
		return;

	Vector3 up = get_up_vector(diff);
	// Rotate between a and b, then rotate the vertical cylinder 90 degs around the X axis.
	Transform3D t = Transform3D(Basis().looking_at(diff, up).rotated(diff.cross(up).normalized(), Math::deg_to_rad(90.f)).scaled_local(Vector3(radius, len, radius)), a + diff * .5f);

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_DGC();

	dgc->geometry_pool.add_or_update_instance(
			scfg,
			ConvertableInstanceType::CYLINDER,
			duration,
			t,
			IS_DEFAULT_COLOR(color) ? Colors::forest_green : color,
			SphereBounds(t.origin, MathUtils::get_max_basis_length(t.basis) * MathUtils::CylinderRadiusForSphere));
}

#pragma endregion // Cylinders
#pragma region Boxes

void DebugDraw3D::draw_box(const Vector3 &position, const Quaternion &rotation, const Vector3 &size, const Color &color, const bool &is_box_centered, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();
	draw_box_xf(Transform3D(Basis(rotation, size), position), color, is_box_centered, duration);
}

void DebugDraw3D::draw_box_ab(const Vector3 &a, const Vector3 &b, const Vector3 &up, const Color &color, const bool &is_ab_diagonal, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	Vector3 diff = b - a;

	if (up.is_zero_approx())
		return;

	// TODO: maybe someone knows a better way to solve it?
	if (is_ab_diagonal) {
		ZoneScopedN("From diagonals");
		Vector3 up_n = up.normalized();
		Vector3 center_orig = diff * .5f;
		Vector3 center = center_orig.rotated(up_n, Math::deg_to_rad(45.f));
		Vector3 center_side = center / MathUtils::Sqrt2;
		Vector3 front = center_side.cross(up_n);

		Transform3D t(center.project(up_n) * 2, center_side.project(front.cross(up_n)) * 2, front * 2, a);

		// copied from draw_box_xf
		SphereBounds sb(t.origin + center_orig, MathUtils::get_max_basis_length(t.basis) * MathUtils::CubeRadiusForSphere);

		LOCK_GUARD(datalock);
		GET_SCOPED_CFG_AND_DGC();

		dgc->geometry_pool.add_or_update_instance(
				scfg,
				ConvertableInstanceType::CUBE,
				duration,
				t,
				IS_DEFAULT_COLOR(color) ? Colors::forest_green : color,
				sb);
	} else {
		ZoneScopedN("From edges");
		Vector3 center = diff * .5f;
		Vector3 front = diff.cross(up.normalized());

		Transform3D t(center.project(up) * 2, center.project(front.cross(up)) * 2, front, a + center);
		draw_box_xf(t, color, true, duration);
	}
}

void DebugDraw3D::draw_box_xf(const Transform3D &transform, const Color &color, const bool &is_box_centered, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	/// It's possible to use less space to contain box, but this method works better in more cases
	SphereBounds sb(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::CubeRadiusForSphere);

	if (!is_box_centered) {
		sb.position = transform.origin + (transform.basis[0] + transform.basis[1] + transform.basis[2]) * 0.5f;
	}

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_DGC();

	dgc->geometry_pool.add_or_update_instance(
			scfg,
			is_box_centered ? ConvertableInstanceType::CUBE_CENTERED : ConvertableInstanceType::CUBE,
			duration,
			transform,
			IS_DEFAULT_COLOR(color) ? Colors::forest_green : color,
			sb);
}

void DebugDraw3D::draw_aabb(const AABB &aabb, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	AABB a(aabb.abs());
	draw_box_xf(Transform3D(Basis().scaled(a.size), a.position), color, false, duration);
}

void DebugDraw3D::draw_aabb_ab(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	Vector3 bottom, top, diag;
	MathUtils::get_diagonal_vectors(a, b, bottom, top, diag);
	draw_box_xf(Transform3D(Basis().scaled(diag), bottom), color, false, duration);
}

#pragma endregion // Boxes
#pragma region Lines

void DebugDraw3D::draw_line_hit(const Vector3 &start, const Vector3 &end, const Vector3 &hit, const bool &is_hit, const real_t &hit_size, const Color &hit_color, const Color &after_hit_color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	LOCK_GUARD(datalock);
	if (is_hit) {
		if (!start.is_equal_approx(hit)) {
			Vector3 first[2] = { start, hit };
			add_or_update_line_with_thickness(duration, first, 2, IS_DEFAULT_COLOR(hit_color) ? config->get_line_hit_color() : hit_color);
		}

		if (!hit.is_equal_approx(end)) {
			Vector3 second[2] = { hit, end };
			add_or_update_line_with_thickness(duration, second, 2, IS_DEFAULT_COLOR(after_hit_color) ? config->get_line_after_hit_color() : after_hit_color);
		}

		GET_SCOPED_CFG_AND_DGC();

		dgc->geometry_pool.add_or_update_instance(
				scfg,
				InstanceType::BILLBOARD_SQUARE,
				duration,
				Transform3D(Basis().scaled(VEC3_ONE(hit_size)), hit),
				IS_DEFAULT_COLOR(hit_color) ? config->get_line_hit_color() : hit_color,
				SphereBounds(hit, MathUtils::CubeRadiusForSphere * hit_size),
				&Colors::empty_color);
	} else {
		Vector3 line[2] = { start, end };
		add_or_update_line_with_thickness(duration, line, 2, IS_DEFAULT_COLOR(hit_color) ? config->get_line_hit_color() : hit_color);
	}
}

void DebugDraw3D::draw_line_hit_offset(const Vector3 &start, const Vector3 &end, const bool &is_hit, const real_t &unit_offset_of_hit, const real_t &hit_size, const Color &hit_color, const Color &after_hit_color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	real_t offset = Math::clamp(unit_offset_of_hit, (real_t)0, (real_t)1);

	if (is_hit) {
		auto diff = end - start;
		draw_line_hit(start, end, (diff.normalized() * (diff.length() * offset) + start), is_hit, hit_size, hit_color, after_hit_color, duration);
	} else {
		draw_line_hit(start, end, {}, is_hit, hit_size, hit_color, after_hit_color, duration);
	}
}

#pragma region Normal

void DebugDraw3D::draw_line(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	Vector3 line[2] = { a, b };
	add_or_update_line_with_thickness(duration, line, 2, IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

void DebugDraw3D::draw_lines(const PackedVector3Array &lines, const Color &color, const real_t &duration) {
	ZoneScoped;
	draw_lines_c(lines.ptr(), lines.size(), color, duration);
}

void DebugDraw3D::draw_lines_c(const Vector3 *lines_data, const uint64_t &lines_size, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();
	if (!lines_data || lines_size == 0)
		return;

	ERR_FAIL_COND_MSG(lines_size % 2 != 0, "The size of the lines array must be even. " + String::num_int64(lines_size) + " is not even.");

	add_or_update_line_with_thickness(duration, lines_data, lines_size, IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

void DebugDraw3D::draw_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	Vector3 line[2] = { origin, origin + direction * length };
	add_or_update_line_with_thickness(duration, line, 2, IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

void DebugDraw3D::draw_line_path(const PackedVector3Array &path, const Color &color, const real_t &duration) {
	ZoneScoped;
	draw_line_path_c(path.ptr(), path.size(), color, duration);
}

void DebugDraw3D::draw_line_path_c(const godot::Vector3 *path_data, const uint64_t &path_size, const godot::Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	if (!path_data || path_size == 0)
		return;

	if (path_size == 1) {
		auto scfg = scoped_config_for_current_thread();
		draw_square(path_data[0], scfg->thickness > 0 ? scfg->thickness * 2 : 0.25f, color, duration);
		return;
	}

	size_t lines_size = (path_size - 1) * 2;
	ADD_THREAD_LOCAL_BUFFER(buffer, Vector3, lines_size, 10000);
	GeometryGenerator::CreateLinesFromPathWireframe(path_data, path_size, buffer.get());

	add_or_update_line_with_thickness(duration, buffer.get(), lines_size, IS_DEFAULT_COLOR(color) ? Colors::light_green : color);
}

#pragma endregion // Normal
#pragma region Arrows

void DebugDraw3D::create_arrow(const Vector3 &p_a, const Vector3 &p_b, const Color &p_color, const real_t &p_arrow_size, const bool &p_is_absolute_size, const real_t &p_duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	Vector3 diff = (p_b - p_a);
	real_t len = diff.length();

	if (Math::is_zero_approx(len))
		return;

	real_t size = (p_is_absolute_size ? p_arrow_size : len * p_arrow_size) * 2;
	Transform3D t = Transform3D(Basis().looking_at(diff, get_up_vector(diff)).scaled(VEC3_ONE(size)), p_b);

	GET_SCOPED_CFG_AND_DGC();

	LOCK_GUARD(datalock);
	dgc->geometry_pool.add_or_update_instance(
			scfg,
			ConvertableInstanceType::ARROWHEAD,
			p_duration,
			t,
			IS_DEFAULT_COLOR(p_color) ? Colors::light_green : p_color,
			SphereBounds(t.origin + t.basis.get_column(2) * 0.5f, MathUtils::ArrowRadiusForSphere * size));
}

void DebugDraw3D::draw_arrowhead(const Transform3D &transform, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_DGC();

	dgc->geometry_pool.add_or_update_instance(
			scfg,
			ConvertableInstanceType::ARROWHEAD,
			duration,
			transform,
			IS_DEFAULT_COLOR(color) ? Colors::light_green : color,
			SphereBounds(transform.origin + transform.basis.get_column(2) * 0.5f, MathUtils::ArrowRadiusForSphere * MathUtils::get_max_basis_length(transform.basis)));
}

void DebugDraw3D::draw_arrow(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	LOCK_GUARD(datalock);

	Vector3 line[2] = { a, b };
	add_or_update_line_with_thickness(duration, line, 2, IS_DEFAULT_COLOR(color) ? Colors::light_green : color);
	create_arrow(a, b, color, arrow_size, is_absolute_size, duration);
}

void DebugDraw3D::draw_arrow_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();
	draw_arrow(origin, origin + direction * length, color, arrow_size, is_absolute_size, duration);
}

void DebugDraw3D::draw_arrow_path(const PackedVector3Array &path, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	ZoneScoped;
	draw_arrow_path_c(path.ptr(), path.size(), color, arrow_size, is_absolute_size, duration);
}

void DebugDraw3D::draw_arrow_path_c(const godot::Vector3 *path_data, const uint64_t &path_size, const godot::Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	if (!path_data || path_size == 0)
		return;

	if (path_size == 1) {
		draw_square(path_data[0], arrow_size, color, duration);
		return;
	}

	size_t lines_size = (path_size - 1) * 2;
	ADD_THREAD_LOCAL_BUFFER(buffer, Vector3, lines_size, 10000);
	GeometryGenerator::CreateLinesFromPathWireframe(path_data, path_size, buffer.get());

	LOCK_GUARD(datalock);
	add_or_update_line_with_thickness(duration, buffer.get(), lines_size, IS_DEFAULT_COLOR(color) ? Colors::light_green : color);

	for (int64_t i = 0; i < path_size - 1; i++) {
		create_arrow(path_data[i], path_data[i + 1], color, arrow_size, is_absolute_size, duration);
	}
}

#pragma endregion // Arrows
#pragma region Points

void DebugDraw3D::draw_point_path(const PackedVector3Array &path, const PointType type, const real_t &size, const Color &points_color, const Color &lines_color, const real_t &duration) {
	ZoneScoped;
	draw_point_path_c(path.ptr(), path.size(), type, size, points_color, lines_color, duration);
}

void DebugDraw3D::draw_point_path_c(const godot::Vector3 *path_data, const uint64_t &path_size, const DebugDraw3D::PointType type, const real_t &size, const godot::Color &points_color, const godot::Color &lines_color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	if (!path_data || path_size == 0)
		return;

	if (path_size == 1) {
		switch (type) {
			case PointType::POINT_TYPE_SQUARE:
				draw_square(path_data[0], size, points_color, duration);
				break;
			case PointType::POINT_TYPE_SPHERE:
				draw_sphere(path_data[0], size, points_color, duration);
				break;
		}
		return;
	}

	LOCK_GUARD(datalock);
	draw_points_c(path_data, path_size, type, size, IS_DEFAULT_COLOR(points_color) ? Colors::red : points_color, duration);
	draw_line_path_c(path_data, path_size, IS_DEFAULT_COLOR(lines_color) ? Colors::green : lines_color, duration);
}

#pragma endregion // Points
#pragma endregion // Lines

#pragma region Misc

void DebugDraw3D::draw_square(const Vector3 &position, const real_t &size, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_DGC();

	dgc->geometry_pool.add_or_update_instance(
			scfg,
			InstanceType::BILLBOARD_SQUARE,
			duration,
			Transform3D(Basis().scaled(VEC3_ONE(size)), position),
			IS_DEFAULT_COLOR(color) ? Colors::red : color,
			SphereBounds(position, MathUtils::CubeRadiusForSphere * size),
			&Colors::empty_color);
}

void DebugDraw3D::draw_plane(const Plane &plane, const Color &color, const Vector3 &anchor_point, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	Color front_color = IS_DEFAULT_COLOR(color) ? Colors::plane_light_sky_blue : color;

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_DGC();

	Camera3D *cam = scfg->dcd.viewport ? scfg->dcd.viewport->get_camera_3d() : nullptr;

	Vector3 center_pos = plane.project(anchor_point == Vector3_INF ? (cam ? cam->get_global_position() : Vector3()) : anchor_point);
	real_t plane_size = scfg->plane_size != INFINITY ? scfg->plane_size : (cam ? (real_t)cam->get_far() : 1000);
	Transform3D t(Basis(), center_pos);
	t = t.looking_at(center_pos + plane.normal, get_up_vector(plane.normal)).scaled_local(VEC3_ONE(plane_size));
	Color custom_col = Color::from_hsv(front_color.get_h(), Math::clamp(front_color.get_s() - 0.25f, 0.f, 1.f), Math::clamp(front_color.get_v() - 0.25f, 0.f, 1.f), front_color.a);

	dgc->geometry_pool.add_or_update_instance(
			scfg,
			InstanceType::PLANE,
			duration,
			t,
			front_color,
			SphereBounds(center_pos, MathUtils::CubeRadiusForSphere * plane_size),
			&custom_col);
}

void DebugDraw3D::draw_points(const PackedVector3Array &points, const PointType type, const real_t &size, const Color &color, const real_t &duration) {
	ZoneScoped;
	draw_points_c(points.ptr(), points.size(), type, size, color, duration);
}

void DebugDraw3D::draw_points_c(const godot::Vector3 *points_data, const uint64_t &points_size, const DebugDraw3D::PointType type, const real_t &size, const godot::Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	if (!points_data || points_size == 0)
		return;

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_DGC();

	switch (type) {
		case PointType::POINT_TYPE_SQUARE:
			for (int i = 0; i < points_size; i++) {
				dgc->geometry_pool.add_or_update_instance(
						scfg,
						InstanceType::BILLBOARD_SQUARE,
						duration,
						Transform3D(Basis().scaled(VEC3_ONE(size)), points_data[i]),
						IS_DEFAULT_COLOR(color) ? Colors::red : color,
						SphereBounds(points_data[i], MathUtils::CubeRadiusForSphere * size),
						&Colors::empty_color);
			}
			break;
		case PointType::POINT_TYPE_SPHERE:
			for (int i = 0; i < points_size; i++) {
				auto transform = Transform3D(Basis().scaled(VEC3_ONE(size * 2)), points_data[i]);

				dgc->geometry_pool.add_or_update_instance(
						scfg,
						ConvertableInstanceType::SPHERE,
						duration,
						transform,
						IS_DEFAULT_COLOR(color) ? Colors::chartreuse : color,
						SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * 0.5f));
			}
			break;
	}
}

void DebugDraw3D::draw_position(const Transform3D &transform, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_DGC();

	dgc->geometry_pool.add_or_update_instance(
			scfg,
			ConvertableInstanceType::POSITION,
			duration,
			transform,
			IS_DEFAULT_COLOR(color) ? Colors::crimson : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::AxisRadiusForSphere));
}

void DebugDraw3D::draw_gizmo(const Transform3D &transform, const Color &color, const bool &is_centered, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	bool is_color_empty = IS_DEFAULT_COLOR(color);
#define COLOR(axis) is_color_empty ? Colors::axis_##axis : color
#define MINUS(axis) transform.origin - transform.basis.get_column(axis)
#define PLUS(axis) transform.origin + transform.basis.get_column(axis)

	LOCK_GUARD(datalock);
	if (is_centered) {
		draw_arrow(MINUS(0 /** 0.5f*/), PLUS(0 /** 0.5f*/), COLOR(x), 0.1f, true, duration);
		draw_arrow(MINUS(1 /** 0.5f*/), PLUS(1 /** 0.5f*/), COLOR(y), 0.1f, true, duration);
		draw_arrow(MINUS(2 /** 0.5f*/), PLUS(2 /** 0.5f*/), COLOR(z), 0.1f, true, duration);
	} else {
		draw_arrow(transform.origin, PLUS(0), COLOR(x), 0.15f, true, duration);
		draw_arrow(transform.origin, PLUS(1), COLOR(y), 0.15f, true, duration);
		draw_arrow(transform.origin, PLUS(2), COLOR(z), 0.15f, true, duration);
	}

#undef COLOR
#undef MINUS
#undef PLUS
}

void DebugDraw3D::draw_grid(const Vector3 &origin, const Vector3 &x_size, const Vector3 &y_size, const Vector2i &subdivision, const Color &color, const bool &is_centered, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();
	draw_grid_xf(Transform3D(Basis(x_size, y_size.cross(x_size).normalized(), y_size), origin),
			subdivision, color, is_centered, duration);
}

void DebugDraw3D::draw_grid_xf(const Transform3D &transform, const Vector2i &p_subdivision, const Color &color, const bool &is_centered, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	const int MAX_SUBDIVISIONS = 1024 * 1024;
	ERR_FAIL_COND(p_subdivision.x > MAX_SUBDIVISIONS);
	ERR_FAIL_COND(p_subdivision.y > MAX_SUBDIVISIONS);

	Vector2i subdivision = p_subdivision.abs();
	subdivision = Vector2i(Math::clamp(subdivision.x, 1, MAX_SUBDIVISIONS), Math::clamp(subdivision.y, 1, MAX_SUBDIVISIONS));
	Vector3 x_axis = transform.basis.get_column(0);
	Vector3 z_axis = transform.basis.get_column(2);
	Vector3 x_d = x_axis / (real_t)subdivision.x;
	Vector3 z_d = z_axis / (real_t)subdivision.y;

#undef MAX_SUBDIVISIONS

	Vector3 origin = is_centered ? transform.origin - x_d * (real_t)subdivision.x * 0.5 - z_d * (real_t)subdivision.y * 0.5 : transform.origin;

	size_t lines_size_x = (subdivision.x + 1) * 2;
	size_t lines_size_y = (subdivision.y + 1) * 2;
	size_t lines_size = lines_size_x + lines_size_y;

	ADD_THREAD_LOCAL_BUFFER(buffer, Vector3, lines_size, 10000);
	Vector3 *lines = buffer.get();

	for (int x = 0; x < subdivision.x + 1; x++) {
		lines[x * 2] = origin + x_d * (real_t)x;
		lines[x * 2 + 1] = origin + x_d * (real_t)x + z_axis;
	}

	for (int y = 0; y < subdivision.y + 1; y++) {
		lines[lines_size_x + y * 2] = origin + z_d * (real_t)y;
		lines[lines_size_x + y * 2 + 1] = origin + z_d * (real_t)y + x_axis;
	}

	draw_lines_c(lines, lines_size, IS_DEFAULT_COLOR(color) ? Colors::white : color, duration);
}

#pragma region Camera Frustum

void DebugDraw3D::draw_camera_frustum(const Camera3D *camera, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();
	ERR_FAIL_COND(!camera);
	draw_camera_frustum_planes(camera->get_frustum(), color, duration);
}

void DebugDraw3D::draw_camera_frustum_planes(const Array &camera_frustum, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	thread_local static std::array<Plane, 6> planes;

	if (camera_frustum.size() == 6) {
		for (int i = 0; i < camera_frustum.size(); i++) {
			planes[i] = camera_frustum[i];
		}
	} else {
		ERR_FAIL_MSG("Camera frustum requires an array of 6 planes. Recieved " + String::num_int64(camera_frustum.size()));
	}

	draw_camera_frustum_planes_c(planes.data(), planes.size(), color, duration);
}

void DebugDraw3D::draw_camera_frustum_planes_c(const godot::Plane *camera_frustum_data, const uint64_t camera_frustum_size, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	if (!camera_frustum_data || camera_frustum_size == 0)
		return;

	ERR_FAIL_COND(camera_frustum_size != 6);

	thread_local static Vector3 lines[GeometryGenerator::CubeIndexes.size()];
	GeometryGenerator::CreateCameraFrustumLinesWireframe(camera_frustum_data, camera_frustum_size, lines);

	LOCK_GUARD(datalock);
	add_or_update_line_with_thickness(duration, lines, GeometryGenerator::CubeIndexes.size(), IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

#pragma endregion // Camera Frustum

#pragma region Text
void DebugDraw3D::draw_text(const Vector3 &position, const String text, const int size, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_NC();

	nc->add_or_update_text(
			scfg,
			position,
			text,
			size,
			IS_DEFAULT_COLOR(color) ? Colors::white : color,
			duration);
}

void DebugDraw3D::draw_text_c(const godot::Vector3 &position, const char *text_string, const int size, const godot::Color &color, const real_t &duration) {
	ZoneScoped;
	draw_text(position, String::utf8(text_string), size, color, duration);
}

#pragma endregion // Text

#pragma endregion // Misc
#endif

#undef IS_DEFAULT_COLOR
#undef CHECK_BEFORE_CALL
#undef NEED_LEAVE
#undef GET_SCOPED_CFG_AND_VDC
#undef GET_SCOPED_CFG_AND_DGC
#undef GET_SCOPED_CFG_AND_NC
