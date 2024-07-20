#pragma once

#include "common/colors.h"
#include "common/i_scope_storage.h"
#include "config_scope_3d.h"
#include "render_instances_enums.h"
#include "utils/profiler.h"

#include <map>
#include <memory>
#include <mutex>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class DebugDraw3D;
class DataGraphManager;
class DebugDrawManager;
class DebugDraw3DConfig;
class DebugDraw3DStats;

#ifndef DISABLE_DEBUG_RENDERING
class DebugGeometryContainer;
struct DelayedRendererLine;
#endif

/// @private
enum class MeshMaterialType : char {
	Wireframe,
	Billboard,
	Plane,
	Extendable,
	MAX,
};

/// @private
enum class MeshMaterialVariant : char {
	Normal,
	NoDepth,
	MAX,
};

#ifndef DISABLE_DEBUG_RENDERING
/// @private
class _DD3D_WorldWatcher : public Node3D {
	GDCLASS(_DD3D_WorldWatcher, Node3D)
protected:
	DebugDraw3D *m_owner = nullptr;
	uint64_t m_world_id;
	static void _bind_methods(){};

public:
	virtual void _process(double p_delta) override;
	virtual void _notification(int p_what);

	_DD3D_WorldWatcher() :
			m_world_id() {}
	_DD3D_WorldWatcher(DebugDraw3D *p_root, uint64_t p_world_id);
};

#endif

/**
 * @brief
 * Singleton class for calling debugging 3D methods.
 *
 * You can use the project settings `debug_draw_3d/settings/3d` for additional customization.
 *
 * For example, `add_bevel_to_volumetric_geometry` allows you to remove or add a bevel for volumetric lines.
 *
 * ![](docs/images/classes/LineBevel.webp)
 *
 * `use_icosphere` and `use_icosphere_for_hd` allow you to change the sphere mesh.
 *
 * ![](docs/images/classes/IcoSphere.webp)
 *
 * @note
 * Wireframe shapes and volumetric wireframes do not support translucency to avoid overlap issues and for better performance.
 * At this point, you can use translucency when drawing planes DebugDraw3D.draw_plane.
 *
 * ---
 * @note
 * Objects created in `_physics_process` are processed separately from those created in `_process`,
 * so they will be deleted only in the first physics tick after rendering.
 * This allows to display objects even if several frames passed between physics ticks.
 *
 * ---
 * @note
 * You can use this class anywhere, including in `_physics_process` and `_process` (and probably from other threads).
 * It is worth mentioning that physics ticks may not be called every frame or may be called several times in one frame.
 * So if you want to avoid multiple identical `draw_` calls, you can call `draw_` methods in `_process` or use such a check:
 * ```python
 * var physics_tick_processed := false
 * func _process(delta: float) -> void:
 * 	# Reset after rendering frame
 * 	physics_tick_processed = false
 * 	# some logic
 *
 * func _physics_process(delta: float) -> void:
 * 	if not physics_tick_processed:
 * 		physics_tick_processed = true
 * 		# some DD3D logic
 * ```
 *
 * ---
 * @note
 * Due to the way Godot registers this addon, it is not possible to use the `draw_` methods
 * in the first few frames immediately after the project is launched.
 */
class DebugDraw3D : public Object, public IScopeStorage<DebugDraw3DScopeConfig, DebugDraw3DScopeConfig::Data> {
	GDCLASS(DebugDraw3D, Object)

	friend DebugDrawManager;

#ifndef DISABLE_DEBUG_RENDERING
	friend DebugGeometryContainer;
	friend _DD3D_WorldWatcher;
#endif

public:
	/**
	 * Appearance of points on the path
	 */
	enum PointType : int {
		POINT_TYPE_SQUARE,
		POINT_TYPE_SPHERE,
	};

private:
	static DebugDraw3D *singleton;

	String root_settings_section;
	const static char *s_use_icosphere;
	const static char *s_use_icosphere_hd;
	const static char *s_add_bevel_to_volumetric;
	const static char *s_default_frustum_scale;

	const static char *s_default_thickness;
	const static char *s_default_center_brightness;
	const static char *s_default_hd_spheres;
	const static char *s_default_plane_size;

	const static char *s_render_priority;
	const static char *s_render_mode;
	const static char *s_render_fog_disabled;

	std::vector<SubViewport *> custom_editor_viewports;
	DebugDrawManager *root_node = nullptr;

	Ref<DebugDraw3DScopeConfig> default_scoped_config;

#ifndef DISABLE_DEBUG_RENDERING
	ProfiledMutex(std::recursive_mutex, datalock, "3D Geometry lock");

	typedef std::pair<uint64_t, DebugDraw3DScopeConfig *> ScopedPairIdConfig;
	// stores thread id and array of id's with ptrs
	std::unordered_map<uint64_t, std::vector<ScopedPairIdConfig> > scoped_configs;
	// stores thread id and most recent config
	std::unordered_map<uint64_t, std::shared_ptr<DebugDraw3DScopeConfig::Data> > cached_scoped_configs;
	uint64_t created_scoped_configs = 0;
	struct {
		uint64_t created;
		uint64_t orphans;
	} scoped_stats_3d = {};

	// Inherited via IScopeStorage
	const std::shared_ptr<DebugDraw3DScopeConfig::Data> scoped_config_for_current_thread() override;

	// Meshes
	/// Store meshes shared between many debug containers
	std::vector<std::array<Ref<ArrayMesh>, 2> > shared_generated_meshes;
	/// Store World3D id and debug container
	std::unordered_map<uint64_t, std::shared_ptr<DebugGeometryContainer>[2]> debug_containers;
	struct viewportToWorldCache {
		uint64_t world_id = 0;
		std::shared_ptr<DebugGeometryContainer> dgcs[2];
	};
	std::unordered_map<const Viewport *, viewportToWorldCache> viewport_to_world_cache;

	// Default materials and shaders
	Ref<ShaderMaterial> mesh_shaders[(int)MeshMaterialType::MAX][(int)MeshMaterialVariant::MAX];

	// Inherited via IScopeStorage
	void _register_scoped_config(uint64_t p_thread_id, uint64_t p_guard_id, DebugDraw3DScopeConfig *p_cfg) override;
	void _unregister_scoped_config(uint64_t p_thread_id, uint64_t p_guard_id) override;
	void _clear_scoped_configs() override;

	std::array<Ref<ArrayMesh>, 2> *get_shared_meshes();
	std::shared_ptr<DebugGeometryContainer> create_debug_container(bool p_no_depth_test);
	std::shared_ptr<DebugGeometryContainer> get_debug_container(const DebugDraw3DScopeConfig::DebugContainerDependent &p_dgcd, const bool p_generate_new_container);
	void _register_viewport_world_deferred(uint64_t /*Viewport * */ p_vp, const uint64_t p_world_id);
	Viewport *_get_root_world_viewport(Viewport *p_vp);
	void _remove_debug_container(const uint64_t &p_world_id);

	_FORCE_INLINE_ Vector3 get_up_vector(const Vector3 &p_dir);
	void add_or_update_line_with_thickness(real_t p_exp_time, std::unique_ptr<Vector3[]> p_lines, const size_t p_line_count, const Color &p_col, const std::function<void(DelayedRendererLine *)> p_custom_upd = nullptr);
	Node *get_root_node();

	void create_arrow(const Vector3 &p_a, const Vector3 &p_b, const Color &p_color, const real_t &p_arrow_size, const bool &p_is_absolute_size, const real_t &p_duration = 0);

#endif

	void init(DebugDrawManager *p_root);

	void set_custom_editor_viewport(std::vector<SubViewport *> p_viewports);
	std::vector<SubViewport *> get_custom_editor_viewports();

	Ref<ShaderMaterial> get_material_variant(MeshMaterialType p_type, MeshMaterialVariant p_var);

	void _load_materials();
	inline bool _is_enabled_override() const;

	void process(double p_delta);
	void physics_process_start(double p_delta);
	void physics_process_end(double p_delta);

#pragma region Exposed Parameter Values

	/// Enable or disable all debug draw
	bool debug_enabled = true;

	Ref<DebugDraw3DConfig> config;

#pragma endregion // Exposed Parameter Values

protected:
	/// @private
	static void _bind_methods();

public:
	DebugDraw3D();
	~DebugDraw3D();

	/**
	 * Get singleton. Not available in GDScript.
	 */
	static DebugDraw3D *get_singleton() {
		return singleton;
	};

#pragma region Configs
	/**
	 * Create a new DebugDraw3DScopeConfig instance and register it.
	 *
	 * This class allows you to override some parameters within scope for the following `draw_*` calls.
	 *
	 * Store this instance in a local variable inside the method.
	 */
	Ref<DebugDraw3DScopeConfig> new_scoped_config();
	/**
	 * Returns the default scope settings that will be applied at the start of each new frame.
	 *
	 * Default values can be overridden in the project settings `debug_draw_3d/settings/3d/volumetric_defaults`.
	 */
	Ref<DebugDraw3DScopeConfig> scoped_config() override;

	/**
	 * Set the configuration global for everything in DebugDraw3D.
	 */
	void set_config(Ref<DebugDraw3DConfig> cfg);
	/**
	 * Get the DebugDraw3DConfig.
	 */
	Ref<DebugDraw3DConfig> get_config() const;

#pragma endregion // Configs

#pragma region Exposed Parameters
	/// @private
	void set_empty_color(const Color &col){};
	/**
	 * Get the color that is used as the default parameter for `draw_*` calls.
	 */
	Color get_empty_color() const;

	/**
	 * Set whether debug drawing works or not.
	 */
	void set_debug_enabled(const bool &state);
	bool is_debug_enabled() const;

#pragma endregion // Exposed Parametes

#pragma region Exposed Draw Methods

	/**
	 * Returns an instance of DebugDraw3DStats with the current statistics.
	 *
	 * Some data can be delayed by 1 frame.
	 */
	Ref<DebugDraw3DStats> get_render_stats();

	/**
	 * Returns an instance of DebugDraw3DStats with the current statistics for the World3D of the Viewport.
	 *
	 * Some data can be delayed by 1 frame.
	 */
	Ref<DebugDraw3DStats> get_render_stats_for_world(Viewport *viewport);

#ifndef DISABLE_DEBUG_RENDERING
#define FAKE_FUNC_IMPL
#else
#define FAKE_FUNC_IMPL \
	{}
#endif

	/**
	 * Regenerate meshes.
	 *
	 * Can be useful if you want to change some project settings and not restart the project.
	 */
	void regenerate_geometry_meshes();

	/**
	 * Clear all 3D geometry
	 */
	void clear_all();

#pragma region Spheres

	/// @private
	void draw_sphere_base(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;
	/**
	 * Draw a sphere
	 *
	 * ![](docs/images/classes/DrawSphere.webp)
	 *
	 * @param position Center of the sphere
	 * @param radius Sphere radius
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_sphere(const Vector3 &position, const real_t &radius = 0.5f, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;
	/**
	 * Draw a sphere with a radius of 0.5
	 *
	 * ![](docs/images/classes/DrawSphereXf.webp)
	 *
	 * @param transform Sphere transform
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_sphere_xf(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Spheres

#pragma region Cylinders

	/**
	 * Draw a vertical cylinder with radius 0.5 and height 1.0
	 *
	 * ![](docs/images/classes/DrawCylinder.webp)
	 *
	 * @param transform Cylinder transform
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_cylinder(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw a cylinder between points A and B with a certain radius
	 *
	 * ![](docs/images/classes/DrawCylinderAb.webp)
	 *
	 * @param a Bottom point of the Cylinder
	 * @param b Top point of the Cylinder
	 * @param radius Cylinder radius
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_cylinder_ab(const Vector3 &a, const Vector3 &b, const real_t &radius = 0.5f, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Cylinders

#pragma region Boxes

	/**
	 * Draw a box
	 *
	 * ![is_box_centered = true](docs/images/classes/DrawBoxXf.webp)
	 *
	 * ![is_box_centered = false](docs/images/classes/DrawBoxXfCorner.webp)
	 *
	 * @param position Position of the Box
	 * @param rotation Rotation of the box
	 * @param size Size of the Box
	 * @param color Primary color
	 * @param is_box_centered Set where the center of the box will be. In the center or in the bottom corner
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_box(const Vector3 &position, const Quaternion &rotation, const Vector3 &size, const Color &color = Colors::empty_color, const bool &is_box_centered = false, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw a box between points A and B by rotating and scaling based on the up vector
	 *
	 * ![is_ab_diagonal = true](docs/images/classes/DrawBoxAb.webp)
	 *
	 * ![is_ab_diagonal = false](docs/images/classes/DrawBoxAbEdge.webp)
	 *
	 * @param a Start position
	 * @param b End position
	 * @param up Vertical vector by which the box will be aligned
	 * @param color Primary color
	 * @param is_ab_diagonal Set uses the diagonal between the corners or the diagonal between the centers of two edges
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_box_ab(const Vector3 &a, const Vector3 &b, const Vector3 &up, const Color &color = Colors::empty_color, const bool &is_ab_diagonal = true, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw a box as in DebugDraw3D.draw_box
	 *
	 * @param transform Box transform
	 * @param color Primary color
	 * @param is_box_centered Set where the center of the box will be. In the center or in the bottom corner
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_box_xf(const Transform3D &transform, const Color &color = Colors::empty_color, const bool &is_box_centered = true, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw a box as in DebugDraw3D.draw_box, but based on the AABB
	 *
	 * @param aabb AABB
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_aabb(const AABB &aabb, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw the box as in DebugDraw3D.draw_aabb, but AABB is defined by the diagonal AB
	 *
	 * @param a Start position
	 * @param b End position
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_aabb_ab(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Boxes

#pragma region Lines

	/**
	 * Draw line separated by hit point (billboard square) or not separated if `is_hit = false`.
	 *
	 * Some of the default settings can be overridden in DebugDraw3DConfig.
	 *
	 * ![](docs/images/classes/DrawLineHit.webp)
	 *
	 * @param start Start point
	 * @param end End point
	 * @param hit Hit point
	 * @param is_hit Whether to draw the collision point
	 * @param hit_size Size of the hit point
	 * @param hit_color Color of the hit point and line before hit
	 * @param after_hit_color Color of line after hit position
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_line_hit(const Vector3 &start, const Vector3 &end, const Vector3 &hit, const bool &is_hit, const real_t &hit_size = 0.25f, const Color &hit_color = Colors::empty_color, const Color &after_hit_color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw line separated by hit point.
	 *
	 * Similar to DebugDraw3D.draw_line_hit, but instead of a hit point, an offset from the start point is used.
	 *
	 * Some of the default settings can be overridden in DebugDraw3DConfig.
	 *
	 * @param start Start point
	 * @param end End point
	 * @param is_hit Whether to draw the collision point
	 * @param unit_offset_of_hit Unit offset on the line where the collision occurs
	 * @param hit_size Size of the hit point
	 * @param hit_color Color of the hit point and line before hit
	 * @param after_hit_color Color of line after hit position
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_line_hit_offset(const Vector3 &start, const Vector3 &end, const bool &is_hit, const real_t &unit_offset_of_hit = 0.5f, const real_t &hit_size = 0.25f, const Color &hit_color = Colors::empty_color, const Color &after_hit_color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma region Normal

	/// @private
	void draw_lines_c(const std::vector<Vector3> &lines, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;
	/**
	 * Draw a single line
	 *
	 * ![](docs/images/classes/Line.webp)
	 *
	 * @param a Start point
	 * @param b End point
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_line(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw a ray.
	 *
	 * Same as DebugDraw3D.draw_line, but uses origin, direction and length instead of A and B.
	 *
	 * @param origin Origin
	 * @param direction Direction
	 * @param length Length
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw an array of lines. Each line is two points, so the array must be of even size.
	 *
	 * ![](docs/images/classes/DrawLines.webp)
	 *
	 * @param lines An array of points of lines. 1 line = 2 vectors3. The array size must be even.
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_lines(const PackedVector3Array &lines, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw an array of lines.
	 *
	 * Unlike DebugDraw3D.draw_lines, here lines are drawn between each point in the array.
	 *
	 * The array can be of any size.
	 *
	 * @param path Sequence of points
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_line_path(const PackedVector3Array &path, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Normal

#pragma region Arrows

	/**
	 * Draw the arrowhead
	 *
	 * @param transform Transform3D of the Arrowhead
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_arrowhead(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw line with arrowhead
	 *
	 * ![](docs/images/classes/Arrow.webp)
	 *
	 * @param a Start point
	 * @param b End point
	 * @param color Primary color
	 * @param arrow_size Size of the arrow
	 * @param is_absolute_size Is `arrow_size` absolute or relative to the length of the string?
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_arrow(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &arrow_size = 0.5f, const bool &is_absolute_size = false, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Same as DebugDraw3D.draw_arrow, but uses origin, direction and length instead of A and B.
	 *
	 * @param origin Origin
	 * @param direction Direction
	 * @param length Length
	 * @param color Primary color
	 * @param arrow_size Size of the arrow
	 * @param is_absolute_size Is `arrow_size` absolute or relative to the line length?
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_arrow_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color = Colors::empty_color, const real_t &arrow_size = 0.5f, const bool &is_absolute_size = false, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw a sequence of points connected by lines with arrows like DebugDraw3D.draw_line_path.
	 *
	 * ![](docs/images/classes/DrawArrowPath.webp)
	 *
	 * @param path Sequence of points
	 * @param color Primary color
	 * @param arrow_size Size of the arrow
	 * @param is_absolute_size Is the `arrow_size` absolute or relative to the length of the line?
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_arrow_path(const PackedVector3Array &path, const Color &color = Colors::empty_color, const real_t &arrow_size = 0.75f, const bool &is_absolute_size = true, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Arrows
#pragma region Points

	/**
	 * Draw a sequence of points connected by lines using billboard squares or spheres like DebugDraw3D.draw_line_path.
	 *
	 * ![type = DebugDraw3D.POINT_TYPE_SQUARE](docs/images/classes/DrawPointsPath.webp)
	 *
	 * ![type = DebugDraw3D.POINT_TYPE_SPHERE](docs/images/classes/DrawPointsPathSpheres.webp)
	 *
	 * @param path Sequence of points
	 * @param type Type of points
	 * @param points_color Color of points
	 * @param lines_color Color of lines
	 * @param size Size of squares
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_point_path(const PackedVector3Array &path, const PointType type = PointType::POINT_TYPE_SQUARE, const real_t &size = 0.25f, const Color &points_color = Colors::empty_color, const Color &lines_color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Points
#pragma endregion // Lines

#pragma region Misc

	/**
	 * Draw a sequence of points using billboard squares or spheres.
	 *
	 * ![](docs/images/classes/DrawPoints.webp)
	 *
	 * @param points Sequence of points
	 * @param type Type of points
	 * @param size Size of squares
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_points(const PackedVector3Array &points, const PointType type = PointType::POINT_TYPE_SQUARE, const real_t &size = 0.25f, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw a square that will always be turned towards the camera
	 *
	 * @param position Center position of square
	 * @param size Square size
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_square(const Vector3 &position, const real_t &size = 0.2f, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draws a plane of non-infinite size relative to the position of the current camera.
	 *
	 * The plane size is set based on the `Far` parameter of the current camera or with DebugDraw3DScopeConfig.set_plane_size.
	 *
	 * ![](docs/images/classes/DrawPlane.webp)
	 *
	 * @param plane Plane data
	 * @param color Primary color
	 * @param anchor_point A point that is projected onto a Plane, and its projection is used as the center of the drawn plane
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_plane(const Plane &plane, const Color &color = Colors::empty_color, const Vector3 &anchor_point = Vector3(INFINITY, INFINITY, INFINITY), const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw 3 intersecting lines with the given transformations
	 *
	 * ![](docs/images/classes/DrawPosition.webp)
	 *
	 * @param transform Transform3D of lines
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_position(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw 3 lines with the given transformations and arrows at the ends
	 *
	 * ![is_centered = false](docs/images/classes/DrawGizmo.webp)
	 *
	 * ![is_centered = true](docs/images/classes/DrawGizmoCentered.webp)
	 *
	 * @param transform Transform3D of lines
	 * @param color Primary color
	 * @param is_centered If `true`, then the lines will intersect in the center of the transform
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_gizmo(const Transform3D &transform, const Color &color = Colors::empty_color, const bool &is_centered = false, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw simple grid with given size and subdivision
	 *
	 * ![](docs/images/classes/DrawGrid.webp)
	 *
	 * @param origin Grid origin
	 * @param x_size Direction and size of the X side. As an axis in the Basis.
	 * @param y_size Direction and size of the Y side. As an axis in the Basis.
	 * @param subdivision Number of cells for the X and Y axes
	 * @param color Primary color
	 * @param is_centered Draw lines relative to origin
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_grid(const Vector3 &origin, const Vector3 &x_size, const Vector3 &y_size, const Vector2i &subdivision, const Color &color = Colors::empty_color, const bool &is_centered = true, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw a simple grid with a given transform and subdivision.
	 *
	 * Like DebugDraw3D.draw_grid, but instead of origin, x_size and y_size, a single transform is used.
	 *
	 * @param transform Transform3D of the Grid
	 * @param p_subdivision Number of cells for the X and Y axes
	 * @param color Primary color
	 * @param is_centered Draw lines relative to origin
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_grid_xf(const Transform3D &transform, const Vector2i &p_subdivision, const Color &color = Colors::empty_color, const bool &is_centered = true, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma region Camera Frustum

	/// @private
	void draw_camera_frustum_planes_c(const std::array<Plane, 6> &planes, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;
	/**
	 * Draw camera frustum area.
	 *
	 * ![](docs/images/classes/DrawFrustum.webp)
	 *
	 * @param camera Camera node
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_camera_frustum(const class godot::Camera3D *camera, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/**
	 * Draw the frustum area of the camera based on an array of 6 planes.
	 *
	 * @param camera_frustum Array of frustum planes
	 * @param color Primary color
	 * @param duration The duration of how long the object will be visible
	 */
	void draw_camera_frustum_planes(const Array &camera_frustum, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Camera Frustum

#pragma endregion // Misc
#pragma endregion // Exposed Draw Methods

#undef FAKE_FUNC_IMPL
};

VARIANT_ENUM_CAST(DebugDraw3D::PointType);
