#include "debug_draw_3d.h"

#include "config_3d.h"
#include "debug_draw_manager.h"
#include "debug_geometry_container.h"
#include "gen/shared_resources.gen.h"
#include "geometry_generators.h"
#include "stats_3d.h"
#include "utils/utils.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/world3d.hpp>
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
		m_owner->_remove_debug_container(m_world_id);
		m_owner = nullptr;

		if (!is_queued_for_deletion()) {
			if (get_parent()) {
				get_parent()->call_deferred(NAMEOF(remove_child), this);
			}
			queue_free();
		}
	}
}

_DD3D_WorldWatcher::_DD3D_WorldWatcher(DebugDraw3D *p_root, uint64_t p_world_id) {
	m_owner = p_root;
	m_world_id = p_world_id;
}
#endif

DebugDraw3D *DebugDraw3D::singleton = nullptr;
const char *DebugDraw3D::s_use_icosphere = "use_icosphere";
const char *DebugDraw3D::s_use_icosphere_hd = "use_icosphere_for_hd";
const char *DebugDraw3D::s_add_bevel_to_volumetric = "add_bevel_to_volumetric_geometry";
const char *DebugDraw3D::s_default_frustum_scale = "defaults/frustum_length_scale";

const char *DebugDraw3D::s_default_thickness = "volumetric_defaults/thickness";
const char *DebugDraw3D::s_default_center_brightness = "volumetric_defaults/center_brightness";
const char *DebugDraw3D::s_default_hd_spheres = "volumetric_defaults/hd_spheres";
const char *DebugDraw3D::s_default_plane_size = "volumetric_defaults/plane_size";

void DebugDraw3D::_bind_methods() {
#define REG_CLASS_NAME DebugDraw3D

#pragma region Parameters

	// HACK PROPERTY_USAGE_NONE disable "Instantiated OBJECT used as default value" warning
	REG_PROP(empty_color, Variant::COLOR, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);
	REG_PROP_BOOL(debug_enabled, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);

	REG_PROP(config, Variant::OBJECT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);

#pragma endregion

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

#pragma endregion // Draw Functions

	REG_METHOD(get_render_stats);
	REG_METHOD(get_render_stats_for_world, "viewport");
	REG_METHOD(new_scoped_config);
	REG_METHOD(scoped_config);

#ifndef DISABLE_DEBUG_RENDERING
	REG_METHOD(_register_viewport_world_deferred);
#endif

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

	DEFINE_SETTING_AND_GET_HINT(real_t def_thickness, root_settings_section + s_default_thickness, 0.05f, Variant::FLOAT, PROPERTY_HINT_RANGE, "0,100,0.0001");
	DEFINE_SETTING_AND_GET_HINT(real_t def_brightness, root_settings_section + s_default_center_brightness, 0.8f, Variant::FLOAT, PROPERTY_HINT_RANGE, "0,1,0.0001");
	DEFINE_SETTING_AND_GET(real_t def_hd_sphere, root_settings_section + s_default_hd_spheres, false, Variant::BOOL);
	DEFINE_SETTING_AND_GET_HINT(real_t def_plane_size, root_settings_section + s_default_plane_size, 0, Variant::FLOAT, PROPERTY_HINT_RANGE, "0,10000,0.001");

	default_scoped_config.instantiate();

	config->set_frustum_length_scale(def_frustum_scale);

	default_scoped_config->set_thickness(def_thickness);
	default_scoped_config->set_center_brightness(def_brightness);
	default_scoped_config->set_hd_sphere(def_hd_sphere);
	default_scoped_config->set_plane_size(def_plane_size == 0 ? INFINITY : def_plane_size);

	_load_materials();

	// Need to call_deferred `regenerate_geometry_meshes` outside this methods in Manager
}

DebugDraw3D::~DebugDraw3D() {
	ZoneScoped;
	UNASSIGN_SINGLETON(DebugDraw3D);

	root_node = nullptr;
}

void DebugDraw3D::process(double p_delta) {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	FrameMarkStart("3D Update");

	// Update 3D debug
	for (const auto &p : debug_containers) {
		p.second->update_geometry(p_delta);
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

	for (const auto &p : debug_containers) {
		p.second->update_geometry_physics_start(p_delta);
	}
#endif
}

void DebugDraw3D::physics_process_end(double p_delta) {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	for (const auto &p : debug_containers) {
		p.second->update_geometry_physics_end(p_delta);
	}

	FrameMarkEnd("3D Physics Step");
#endif
}

#ifndef DISABLE_DEBUG_RENDERING
const std::shared_ptr<DebugDraw3DScopeConfig::Data> DebugDraw3D::scoped_config_for_current_thread() {
	ZoneScoped;
	LOCK_GUARD(datalock);
	uint64_t thread = OS::get_singleton()->get_thread_caller_id();

	const auto &it = cached_scoped_configs.find(thread);
	if (it != cached_scoped_configs.cend()) {
		return it->second;
	}

	const auto &it_v = scoped_configs.find(thread);
	if (it_v != scoped_configs.cend()) {
		const auto &cfgs = it_v->second;
		if (!cfgs.empty()) {
			DebugDraw3DScopeConfig *tmp = cfgs.back().second;
			cached_scoped_configs[thread] = tmp->data;
			return tmp->data;
		}
	}

	cached_scoped_configs[thread] = default_scoped_config.ptr()->data;
	return default_scoped_config.ptr()->data;
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
	auto res = std::find_if(cfgs.rbegin(), cfgs.rend(), [&guard_id](const ScopedPairIdConfig &i) { return i.first == guard_id; });

	if (res != cfgs.rend()) {
		cfgs.erase(--res.base());

		// Update cached value
		if (!cfgs.empty()) {
			cached_scoped_configs[thread_id] = cfgs.back().second->data;
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

std::shared_ptr<DebugGeometryContainer> DebugDraw3D::create_debug_container() {
	return std::make_shared<DebugGeometryContainer>(
			this,
			PS()->get_setting(root_settings_section + s_add_bevel_to_volumetric),
			PS()->get_setting(root_settings_section + s_use_icosphere),
			PS()->get_setting(root_settings_section + s_use_icosphere_hd));
}

std::shared_ptr<DebugGeometryContainer> DebugDraw3D::get_debug_container(Viewport *p_vp) {
	ZoneScoped;
	LOCK_GUARD(datalock);

	if (!p_vp) {
		return nullptr;
	}

	if (const auto &cached_world = viewport_to_world_cache.find(p_vp);
			cached_world != viewport_to_world_cache.end()) {
		return cached_world->second.dgc;
	}

	Ref<World3D> vp_world = p_vp->find_world_3d();
	uint64_t vp_world_id = vp_world->get_instance_id();

	if (const auto &dgc = debug_containers.find(vp_world_id);
			dgc != debug_containers.end()) {

		viewport_to_world_cache[p_vp] = { dgc->first, dgc->second };
		return dgc->second;
	}

	auto dgc = create_debug_container();
	dgc->set_world(vp_world);
	debug_containers[vp_world_id] = dgc;
	viewport_to_world_cache[p_vp] = { vp_world_id, dgc };

	call_deferred(NAMEOF(_register_viewport_world_deferred), _get_root_world_viewport(p_vp), vp_world_id);

	return dgc;
}

void DebugDraw3D::_register_viewport_world_deferred(Viewport *p_vp, const uint64_t p_world_id) {
	ZoneScoped;

	// something failed. need to remove container
	if (!UtilityFunctions::is_instance_valid(p_vp) || p_vp->is_queued_for_deletion()) {
		_remove_debug_container(p_world_id);
		return;
	}

	auto *watcher = memnew(_DD3D_WorldWatcher(this, p_world_id));
	p_vp->add_child(watcher);
	p_vp->move_child(watcher, 0);
}

Viewport *DebugDraw3D::_get_root_world_viewport(Viewport *p_vp) {
	Viewport *parent_vp = p_vp->get_viewport();
	if (!parent_vp || parent_vp == p_vp)
		return p_vp;

	if (p_vp->find_world_3d() == parent_vp->find_world_3d()) {
		return _get_root_world_viewport(parent_vp);
	}

	return p_vp;
}

void DebugDraw3D::_remove_debug_container(const uint64_t &p_world_id) {
	ZoneScoped;
	LOCK_GUARD(datalock);

	if (const auto &dgc = debug_containers.find(p_world_id);
			dgc != debug_containers.end()) {
		debug_containers.erase(dgc);
		DEV_PRINT_STD("World3D (%d) is no longer in use, its storage will be deleted.\n", p_world_id);

		std::vector<const Viewport *> viewport_to_remove;

		for (const auto &p : viewport_to_world_cache) {
			if (p.second.world_id == p_world_id) {
				viewport_to_remove.push_back(p.first);
			}
		}

		for (const auto &p : viewport_to_remove) {
			viewport_to_world_cache.erase(p);
		}
	}
}

#endif

Ref<DebugDraw3DScopeConfig> DebugDraw3D::new_scoped_config() {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	LOCK_GUARD(datalock);
	static std::atomic<uint64_t> create_counter = 0;
	create_counter++;

	uint64_t thread = OS::get_singleton()->get_thread_caller_id();
	Ref<DebugDraw3DScopeConfig> res(memnew(
			DebugDraw3DScopeConfig(
					thread,
					create_counter,
					scoped_config_for_current_thread(),
					[this](const uint64_t &p_thread_id, const uint64_t &p_guard_id) {
						_unregister_scoped_config(p_thread_id, p_guard_id);
					})));

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
#define LOAD_SHADER(code, mat, source) \
	code.instantiate();                \
	code->set_code(source);            \
	mat.instantiate();                 \
	mat->set_shader(code);

	LOAD_SHADER(shader_wireframe_code, shader_wireframe_mat, DD3DResources::src_resources_wireframe_unshaded_gdshader);
	LOAD_SHADER(shader_billboard_code, shader_billboard_mat, DD3DResources::src_resources_billboard_unshaded_gdshader);
	LOAD_SHADER(shader_plane_code, shader_plane_mat, DD3DResources::src_resources_plane_unshaded_gdshader);
	LOAD_SHADER(shader_extendable_code, shader_extendable_mat, DD3DResources::src_resources_extendable_meshes_gdshader);
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

Ref<ShaderMaterial> DebugDraw3D::get_wireframe_material() {
#ifndef DISABLE_DEBUG_RENDERING
	return shader_wireframe_mat;
#else
	return Ref<ShaderMaterial>();
#endif
}

Ref<ShaderMaterial> DebugDraw3D::get_billboard_material() {
#ifndef DISABLE_DEBUG_RENDERING
	return shader_billboard_mat;
#else
	return Ref<ShaderMaterial>();
#endif
}

Ref<ShaderMaterial> DebugDraw3D::get_plane_material() {
#ifndef DISABLE_DEBUG_RENDERING
	return shader_plane_mat;
#else
	return Ref<ShaderMaterial>();
#endif
}

Ref<ShaderMaterial> DebugDraw3D::get_extendable_material() {
#ifndef DISABLE_DEBUG_RENDERING
	return shader_extendable_mat;
#else
	return Ref<ShaderMaterial>();
#endif
}

Color DebugDraw3D::get_empty_color() const {
	return Colors::empty_color;
}

#pragma region Exposed Parameters

void DebugDraw3D::set_debug_enabled(const bool &state) {
	debug_enabled = state;

	if (!state) {
		clear_all();
	}
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
	Ref<DebugDraw3DStats> stats_3d;
	stats_3d.instantiate();

	for (const auto &p : debug_containers) {
		const auto &dgc = p.second;

		if (dgc) {
			dgc->get_render_stats(stats_3d);
			res->combine_with(stats_3d);
		}
	}
	res->set_scoped_config_stats(scoped_stats_3d.created, scoped_stats_3d.orphans);
#endif
	return res;
}

Ref<DebugDraw3DStats> DebugDraw3D::get_render_stats_for_world(Viewport *viewport) {
	Ref<DebugDraw3DStats> stats_3d;
	stats_3d.instantiate();
#ifndef DISABLE_DEBUG_RENDERING
	auto dgc = get_debug_container(viewport);

	if (dgc)
		dgc->get_render_stats(stats_3d);
#endif
	return stats_3d;
}

void DebugDraw3D::regenerate_geometry_meshes() {
#ifndef DISABLE_DEBUG_RENDERING
	LOCK_GUARD(datalock);
	for (auto &p : debug_containers) {
		Ref<World3D> old_world = p.second->get_world();

		p.second = create_debug_container();
		p.second->set_world(old_world);
	}
#endif
}

void DebugDraw3D::clear_all() {
	ZoneScoped;
#ifndef DISABLE_DEBUG_RENDERING
	for (auto &p : debug_containers) {
		p.second->clear_3d_objects();
	}
#else
	return;
#endif
}

#ifndef DISABLE_DEBUG_RENDERING
#define IS_DEFAULT_COLOR(name) (name == Colors::empty_color)
#define GET_PROC_TYPE() (Engine::get_singleton()->is_in_physics_frame() ? ProcessType::PHYSICS_PROCESS : ProcessType::PROCESS)
#define CHECK_BEFORE_CALL() \
	if (NEED_LEAVE || config->is_freeze_3d_render()) return;
#endif

#define GET_SCOPED_CFG_AND_DGC()                    \
	auto scfg = scoped_config_for_current_thread(); \
	auto dgc = get_debug_container(scfg->viewport); \
	if (!dgc) return

#ifndef DISABLE_DEBUG_RENDERING

Vector3 DebugDraw3D::get_up_vector(const Vector3 &p_dir) {
	bool ex = Math::is_equal_approx(p_dir.x, 0);
	bool ey = Math::is_equal_approx(p_dir.y, 0);
	bool ez = Math::is_equal_approx(p_dir.z, 0);

	if (ex) {
		if (ey) {
			return Vector3_UP;
		} else if (ez) {
			return Vector3_FORWARD;
		}
		return Vector3_UP;
	} else if (ey) {
		return p_dir.normalized().cross(Vector3_UP);
	} else if (ez) {
		return Vector3_UP;
	}

	return Vector3_UP;
}

void DebugDraw3D::add_or_update_line_with_thickness(real_t p_exp_time, std::unique_ptr<Vector3[]> p_lines, const size_t p_line_count, const Color &p_col, const std::function<void(DelayedRendererLine *)> p_custom_upd) {
	ZoneScoped;

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_DGC();

	if (!scfg->thickness) {
		dgc->geometry_pool.add_or_update_line(
				scfg,
				p_exp_time,
				GET_PROC_TYPE(),
				std::move(p_lines),
				p_line_count,
				p_col);
	} else {
		for (int i = 0; i < p_line_count; i += 2) {
			ZoneScopedN("Convert AB to xf");
			Vector3 a = p_lines.get()[i];
			Vector3 diff = p_lines.get()[i + 1] - a;
			real_t len = diff.length();
			Vector3 center = diff.normalized() * len * .5f;
			dgc->geometry_pool.add_or_update_instance(
					scfg,
					InstanceType::LINE_VOLUMETRIC,
					p_exp_time,
					GET_PROC_TYPE(),
					Transform3D(Basis().looking_at(center, get_up_vector(center)).scaled(VEC3_ONE(len)), a), // slow
					p_col,
					SphereBounds(a + center, len * .5f));
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
			GET_PROC_TYPE(),
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
			GET_PROC_TYPE(),
			transform,
			IS_DEFAULT_COLOR(color) ? Colors::forest_green : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::CylinderRadiusForSphere));
}

void DebugDraw3D::draw_cylinder_ab(const Vector3 &a, const Vector3 &b, const real_t &radius, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	// TODO maybe someone knows a better way to solve it?
	Vector3 diff = b - a;
	real_t len = diff.length();
	Vector3 half_center = diff.normalized() * len * .5f;
	Vector3 up = get_up_vector(half_center);
	Transform3D t = Transform3D(Basis().looking_at(half_center, up).scaled_local(Vector3(radius, radius, len)), a + half_center);

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_DGC();

	dgc->geometry_pool.add_or_update_instance(
			scfg,
			ConvertableInstanceType::CYLINDER_AB,
			duration,
			GET_PROC_TYPE(),
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

	// TODO maybe someone knows a better way to solve it?
	if (is_ab_diagonal) {
		ZoneScopedN("From diagonals");
		Vector3 up_n = up.normalized();
		Vector3 half_center_orig = diff.normalized() * diff.length() * .5f;
		Vector3 half_center = half_center_orig.rotated(up_n, Math::deg_to_rad(45.f));
		Vector3 half_center_side = half_center / MathUtils::Sqrt2;
		Vector3 front = half_center_side.cross(up_n);

		Transform3D t(half_center.project(up_n) * 2, half_center_side.project(front.cross(up_n)) * 2, front * 2, a);

		// copied from draw_box_xf
		SphereBounds sb(t.origin + half_center_orig, MathUtils::get_max_basis_length(t.basis) * MathUtils::CubeRadiusForSphere);

		LOCK_GUARD(datalock);
		GET_SCOPED_CFG_AND_DGC();

		dgc->geometry_pool.add_or_update_instance(
				scfg,
				ConvertableInstanceType::CUBE,
				duration,
				GET_PROC_TYPE(),
				t,
				IS_DEFAULT_COLOR(color) ? Colors::forest_green : color,
				sb);
	} else {
		ZoneScopedN("From edges");
		Vector3 half_center = diff.normalized() * diff.length() * .5f;
		Vector3 front = diff.cross(up.normalized());

		Transform3D t(half_center.project(up) * 2, half_center.project(front.cross(up)) * 2, front, a + half_center);
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
			GET_PROC_TYPE(),
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
		add_or_update_line_with_thickness(duration, std::unique_ptr<Vector3[]>(new Vector3[2]{ start, hit }), 2, IS_DEFAULT_COLOR(hit_color) ? config->get_line_hit_color() : hit_color);
		add_or_update_line_with_thickness(duration, std::unique_ptr<Vector3[]>(new Vector3[2]{ hit, end }), 2, IS_DEFAULT_COLOR(after_hit_color) ? config->get_line_after_hit_color() : after_hit_color);

		GET_SCOPED_CFG_AND_DGC();

		dgc->geometry_pool.add_or_update_instance(
				scfg,
				InstanceType::BILLBOARD_SQUARE,
				duration,
				GET_PROC_TYPE(),
				Transform3D(Basis().scaled(VEC3_ONE(hit_size)), hit),
				IS_DEFAULT_COLOR(hit_color) ? config->get_line_hit_color() : hit_color,
				SphereBounds(hit, MathUtils::CubeRadiusForSphere * hit_size),
				&Colors::empty_color);
	} else {
		add_or_update_line_with_thickness(duration, std::unique_ptr<Vector3[]>(new Vector3[2]{ start, end }), 2, IS_DEFAULT_COLOR(hit_color) ? config->get_line_hit_color() : hit_color);
	}
}

void DebugDraw3D::draw_line_hit_offset(const Vector3 &start, const Vector3 &end, const bool &is_hit, const real_t &unit_offset_of_hit, const real_t &hit_size, const Color &hit_color, const Color &after_hit_color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	if (is_hit && unit_offset_of_hit >= 0 && unit_offset_of_hit <= 1) {
		draw_line_hit(start, end, ((end - start).normalized() * start.distance_to(end) * unit_offset_of_hit + start), is_hit, hit_size, hit_color, after_hit_color, duration);
	} else {
		draw_line_hit(start, end, {}, is_hit, hit_size, hit_color, after_hit_color, duration);
	}
}

#pragma region Normal

void DebugDraw3D::draw_line(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	add_or_update_line_with_thickness(duration, std::unique_ptr<Vector3[]>(new Vector3[2]{ a, b }), 2, IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

void DebugDraw3D::draw_lines(const PackedVector3Array &lines, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	if (lines.size() % 2 != 0) {
		PRINT_ERROR("The size of the lines array must be even. " + String::num_int64(lines.size()) + " is not even.");
		return;
	}

	size_t s = lines.size();
	std::unique_ptr<Vector3[]> l(new Vector3[s]);
	memcpy(l.get(), lines.ptr(), s * sizeof(Vector3));

	add_or_update_line_with_thickness(duration, std::move(l), s, IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

void DebugDraw3D::draw_lines_c(const std::vector<Vector3> &lines, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	if (lines.size() % 2 != 0) {
		PRINT_ERROR("The size of the lines array must be even. " + String::num_int64(lines.size()) + " is not even.");
		return;
	}

	size_t s = lines.size();
	std::unique_ptr<Vector3[]> l(new Vector3[s]);
	memcpy(l.get(), lines.data(), s * sizeof(Vector3));

	add_or_update_line_with_thickness(duration, std::move(l), s, IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

void DebugDraw3D::draw_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	add_or_update_line_with_thickness(duration, std::unique_ptr<Vector3[]>(new Vector3[2]{ origin, origin + direction * length }), 2, IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

void DebugDraw3D::draw_line_path(const PackedVector3Array &path, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	if (path.size() < 2) {
		if (path.size() == 1) {
			PRINT_ERROR("Line path must contains at least 2 points.");
		}
		return;
	}

	size_t s = (path.size() - 1) * 2;
	std::unique_ptr<Vector3[]> l(new Vector3[s]);
	GeometryGenerator::CreateLinesFromPathWireframe(path, l.get());

	add_or_update_line_with_thickness(duration, std::move(l), s, IS_DEFAULT_COLOR(color) ? Colors::light_green : color);
}

#pragma endregion // Normal
#pragma region Arrows

void DebugDraw3D::create_arrow(const Vector3 &p_a, const Vector3 &p_b, const Color &p_color, const real_t &p_arrow_size, const bool &p_is_absolute_size, const real_t &p_duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	Vector3 dir = (p_b - p_a);
	real_t size = (p_is_absolute_size ? p_arrow_size : dir.length() * p_arrow_size) * 2;

	Vector3 up = get_up_vector(dir);
	Transform3D t = Transform3D(Basis().looking_at(dir, up).scaled(VEC3_ONE(size)), p_b);

	GET_SCOPED_CFG_AND_DGC();

	LOCK_GUARD(datalock);
	dgc->geometry_pool.add_or_update_instance(
			scfg,
			ConvertableInstanceType::ARROWHEAD,
			p_duration,
			GET_PROC_TYPE(),
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
			GET_PROC_TYPE(),
			transform,
			IS_DEFAULT_COLOR(color) ? Colors::light_green : color,
			SphereBounds(transform.origin + transform.basis.get_column(2) * 0.5f, MathUtils::ArrowRadiusForSphere * MathUtils::get_max_basis_length(transform.basis)));
}

void DebugDraw3D::draw_arrow(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	LOCK_GUARD(datalock);
	add_or_update_line_with_thickness(duration, std::unique_ptr<Vector3[]>(new Vector3[2]{ a, b }), 2, IS_DEFAULT_COLOR(color) ? Colors::light_green : color);
	create_arrow(a, b, color, arrow_size, is_absolute_size, duration);
}

void DebugDraw3D::draw_arrow_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();
	draw_arrow(origin, origin + direction * length, color, arrow_size, is_absolute_size, duration);
}

void DebugDraw3D::draw_arrow_path(const PackedVector3Array &path, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	size_t s = (path.size() - 1) * 2;
	std::unique_ptr<Vector3[]> l(new Vector3[s]);
	GeometryGenerator::CreateLinesFromPathWireframe(path, l.get());

	LOCK_GUARD(datalock);
	add_or_update_line_with_thickness(duration, std::move(l), s, IS_DEFAULT_COLOR(color) ? Colors::light_green : color);

	for (int i = 0; i < path.size() - 1; i++) {
		create_arrow(path[i], path[i + 1], color, arrow_size, is_absolute_size, duration);
	}
}

#pragma endregion // Arrows
#pragma region Points

void DebugDraw3D::draw_point_path(const PackedVector3Array &path, const PointType type, const real_t &size, const Color &points_color, const Color &lines_color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	LOCK_GUARD(datalock);
	draw_points(path, type, size, IS_DEFAULT_COLOR(points_color) ? Colors::red : points_color, duration);
	draw_line_path(path, IS_DEFAULT_COLOR(lines_color) ? Colors::green : lines_color, duration);
}

#pragma endregion // Points
#pragma endregion // Lines

#pragma region Misc

void DebugDraw3D::draw_square(const Vector3 &position, const real_t &size, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	Transform3D t(Basis().scaled(VEC3_ONE(size)), position);

	LOCK_GUARD(datalock);
	GET_SCOPED_CFG_AND_DGC();

	dgc->geometry_pool.add_or_update_instance(
			scfg,
			InstanceType::BILLBOARD_SQUARE,
			duration,
			GET_PROC_TYPE(),
			t,
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

	Camera3D *cam = scfg->viewport ? scfg->viewport->get_camera_3d() : nullptr;

	Vector3 center_pos = plane.project(anchor_point == Vector3_INF ? (cam ? cam->get_global_position() : Vector3()) : anchor_point);
	real_t plane_size = scfg->plane_size != INFINITY ? scfg->plane_size : (cam ? (real_t)cam->get_far() : 1000);
	Transform3D t(Basis(), center_pos);
	t = t.looking_at(center_pos + plane.normal, get_up_vector(plane.normal)).scaled_local(VEC3_ONE(plane_size));
	Color custom_col = Color::from_hsv(front_color.get_h(), Math::clamp(front_color.get_s() - 0.25f, 0.f, 1.f), Math::clamp(front_color.get_v() - 0.25f, 0.f, 1.f), front_color.a);

	dgc->geometry_pool.add_or_update_instance(
			scfg,
			InstanceType::PLANE,
			duration,
			GET_PROC_TYPE(),
			t,
			front_color,
			SphereBounds(center_pos, MathUtils::CubeRadiusForSphere * plane_size),
			&custom_col);
}

void DebugDraw3D::draw_points(const PackedVector3Array &points, const PointType type, const real_t &size, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	LOCK_GUARD(datalock);
	for (int i = 0; i < points.size(); i++) {
		switch (type) {
			case PointType::POINT_TYPE_SQUARE:
				draw_square(points[i], size, color, duration);
				break;
			case PointType::POINT_TYPE_SPHERE:
				draw_sphere(points[i], size, color, duration);
				break;
		}
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
			GET_PROC_TYPE(),
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

	Vector3 origin = is_centered ?
							 transform.origin - x_d * (real_t)subdivision.x * 0.5 - z_d * (real_t)subdivision.y * 0.5 :
							 transform.origin;

	std::vector<Vector3> lines;
	for (int x = 0; x < subdivision.x + 1; x++) {
		lines.push_back(origin + x_d * (real_t)x);
		lines.push_back(origin + x_d * (real_t)x + z_axis);
	}

	for (int y = 0; y < subdivision.y + 1; y++) {
		lines.push_back(origin + z_d * (real_t)y);
		lines.push_back(origin + z_d * (real_t)y + x_axis);
	}

	draw_lines_c(lines, IS_DEFAULT_COLOR(color) ? Colors::white : color, duration);
}

#pragma region Camera Frustum

void DebugDraw3D::draw_camera_frustum_planes_c(const std::array<Plane, 6> &planes, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();

	size_t s = GeometryGenerator::CubeIndexes.size();
	std::unique_ptr<Vector3[]> l(new Vector3[s]);
	GeometryGenerator::CreateCameraFrustumLinesWireframe(planes, l.get());

	LOCK_GUARD(datalock);
	add_or_update_line_with_thickness(duration, std::move(l), s, IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

void DebugDraw3D::draw_camera_frustum(const Camera3D *camera, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();
	ERR_FAIL_COND(!camera);
	draw_camera_frustum_planes(camera->get_frustum(), color, duration);
}

void DebugDraw3D::draw_camera_frustum_planes(const Array &camera_frustum, const Color &color, const real_t &duration) {
	ZoneScoped;
	CHECK_BEFORE_CALL();
	std::array<Plane, 6> planes = { Plane() };

	if (camera_frustum.size() == 6) {
		for (int i = 0; i < camera_frustum.size(); i++) {
			planes[i] = camera_frustum[i];
		}
	} else {
		PRINT_ERROR("Camera frustum requires an array of 6 planes. Recieved " + String::num_int64(camera_frustum.size()));
	}

	draw_camera_frustum_planes_c(planes, color, duration);
}

#pragma endregion // Camera Frustum

#pragma endregion // Misc
#endif

#undef IS_DEFAULT_COLOR
#undef GET_PROC_TYPE
#undef CHECK_BEFORE_CALL
#undef NEED_LEAVE
#undef GET_SCOPED_CFG_AND_DGC
