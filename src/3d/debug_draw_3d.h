#pragma once

#include "common/colors.h"
#include "common/i_scoped_storage.h"
#include "config_scoped_3d.h"
#include "utils/profiler.h"

#include <map>
#include <memory>
#include <mutex>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class DataGraphManager;
class DebugDrawManager;
class DebugDrawConfig3D;
class DebugDrawStats3D;

#ifndef DISABLE_DEBUG_RENDERING
class DebugGeometryContainer;
class DelayedRendererLine;

enum class InstanceType : char;
enum class ConvertableInstanceType : char;
#endif

class DebugDraw3D : public Object, public IScopedStorage<DDScopedConfig3D> {
	GDCLASS(DebugDraw3D, Object)

	friend DebugDrawManager;

#ifndef DISABLE_DEBUG_RENDERING
	friend DebugGeometryContainer;

	enum GeometryType{
		Wireframe,
		Volumetric,
		Solid,
	};
#endif

private:
	static DebugDraw3D *singleton;

	String root_settings_section;
	const static char *s_add_bevel_to_volumetric;
	const static char *s_default_thickness;

	std::vector<SubViewport *> custom_editor_viewports;
	DebugDrawManager *root_node = nullptr;

	Ref<DebugDrawStats3D> stats_3d;
	Ref<DDScopedConfig3D> default_scoped_config;

#ifndef DISABLE_DEBUG_RENDERING
	ProfiledMutex(std::recursive_mutex, datalock, "3D Geometry lock");
	ProfiledMutex(std::recursive_mutex, scoped_datalock, "3D Scoped config lock");

	typedef std::pair<uint64_t, DDScopedConfig3D *> ScopedPairIdConfig;
	// stores thread id and array of id's with ptrs
	std::map<uint64_t, std::vector<ScopedPairIdConfig> > scoped_configs;
	// stores thread id and most recent config
	std::map<uint64_t, DDScopedConfig3D *> cached_scoped_configs;
	uint64_t create_scoped_configs = 0;

	// Inherited via IScopedStorage
	Ref<DDScopedConfig3D> scoped_config_for_current_thread() override;
#endif

#ifndef DISABLE_DEBUG_RENDERING

	// Meshes
	std::unique_ptr<DebugGeometryContainer> dgc;

	// Default materials and shaders
	Ref<ShaderMaterial> shader_unshaded_mat;
	Ref<Shader> shader_unshaded_code;

	Ref<ShaderMaterial> shader_billboard_mat;
	Ref<Shader> shader_billboard_code;

	Ref<ShaderMaterial> shader_extendable_mat;
	Ref<Shader> shader_extendable_code;

	// Inherited via IScopedStorage
	void _register_scoped_config(uint64_t thread_id, uint64_t guard_id, DDScopedConfig3D *cfg) override;
	void _unregister_scoped_config(uint64_t thread_id, uint64_t guard_id) override;
	void _clear_scoped_configs() override;

	// Internal use of raw pointer to avoid ref/unref
	Color _scoped_config_to_custom(DDScopedConfig3D *cfg);
	InstanceType _scoped_config_type_convert(ConvertableInstanceType type, DDScopedConfig3D *cfg);
	GeometryType _scoped_config_get_geometry_type(DDScopedConfig3D *cfg);

	_FORCE_INLINE_ Vector3 get_up_vector(const Vector3 &dir);
	void add_or_update_line_with_thickness(real_t _exp_time, const std::vector<Vector3> &_lines, const Color &_col, const std::function<void(DelayedRendererLine *)> _custom_upd = nullptr);

#endif

	void _load_materials();
	void _set_base_world_node(Node *world_base);
	inline bool _is_enabled_override() const;

	void process(double delta);

#pragma region Exposed Parameter Values

	/// Enable or disable all debug draw
	bool debug_enabled = true;
	/// Custom 'Viewport' to use for frustum culling.
	Viewport *custom_viewport = nullptr;

	Ref<DebugDrawConfig3D> config;

#pragma endregion // Exposed Parameter Values

protected:
	static void _bind_methods();

public:
	DebugDraw3D();
	~DebugDraw3D();

	void init(DebugDrawManager *root);

	static DebugDraw3D *get_singleton() {
		return singleton;
	};

	Node *get_root_node();
	void set_custom_editor_viewport(std::vector<SubViewport *> _viewports);
	std::vector<SubViewport *> get_custom_editor_viewports();

	Ref<ShaderMaterial> get_basic_unshaded_material();
	Ref<ShaderMaterial> get_billboard_unshaded_material();
	Ref<ShaderMaterial> get_extendable_material();

#pragma region Exposed Parameters
	Ref<DDScopedConfig3D> new_scoped_config();
	Ref<DDScopedConfig3D> scoped_config() override;

	void set_empty_color(const Color &_col);
	Color get_empty_color() const;

	void set_debug_enabled(const bool &_state);
	bool is_debug_enabled() const;

	void set_config(Ref<DebugDrawConfig3D> _cfg);
	Ref<DebugDrawConfig3D> get_config() const;

	void set_custom_viewport(Viewport *_viewport);
	Viewport *get_custom_viewport() const;

#pragma endregion // Exposed Parametes

#pragma region Exposed Draw Functions

	/// Returns a dictionary with rendering statistics.
	/// Some data can be delayed by 1 frame.
	Ref<DebugDrawStats3D> get_render_stats();

#ifndef DISABLE_DEBUG_RENDERING
#define FAKE_FUNC_IMPL
#else
#define FAKE_FUNC_IMPL \
	{}
#endif

	void regenerate_geometry_meshes();

	/// Clear all 3D objects
	void clear_all();

#pragma region Spheres

	void draw_sphere_base(const Vector3 &position, const real_t &radius = 0.5f, const Color &color = Colors::empty_color, const real_t &duration = 0, const bool &hd = false) FAKE_FUNC_IMPL;
	void draw_sphere_xf_base(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0, const bool &hd = false) FAKE_FUNC_IMPL;
	/// Draw sphere
	/// position: Position of the sphere center
	/// radius: Sphere radius
	/// color: Sphere color
	/// duration: Duration of existence in seconds
	void draw_sphere(const Vector3 &position, const real_t &radius = 0.5f, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw sphere with a radius of 0.5
	/// transform: Transform3D of the Sphere
	/// color: Sphere color
	/// duration: Duration of existence in seconds
	void draw_sphere_xf(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw sphere with higher lines count
	/// position: Position of the sphere center
	/// radius: Sphere radius
	/// color: Sphere color
	/// duration: Duration of existence in seconds
	void draw_sphere_hd(const Vector3 &position, const real_t &radius = 0.5f, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw sphere with higher lines count
	/// transform: Transform3D of the sphere
	/// color: Sphere color
	/// duration: Duration of existence in seconds
	void draw_sphere_hd_xf(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Spheres

#pragma region Cylinders

	/// Draw vertical cylinder with a radius of 0.5 and a height of 1.0
	/// transform: Transform3D of the Cylinder
	/// color: Cylinder color
	/// duration: Duration of existence in seconds
	void draw_cylinder(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw a cylinder between points A and B with a certain radius
	/// a: Bottom point of the Cylinder
	/// b: Top point of the Cylinder
	/// radius: Cylinder radius
	/// color: Cylinder color
	/// duration: Duration of existence in seconds
	void draw_cylinder_ab(const Vector3 &a, const Vector3 &b, const real_t &radius = 0.5f, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Cylinders

#pragma region Boxes

	/// Draw box
	/// position: Position of the Box
	/// size: Size of the Box
	/// color: Box color
	/// is_box_centered: Use 'position' as the center of the box, not as the bottom corner
	/// duration: Duration of existence in seconds
	void draw_box(const Vector3 &position, const Vector3 &size, const Color &color = Colors::empty_color, const bool &is_box_centered = false, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw box
	/// transform: Transform3D of the Box
	/// color: Box color
	/// is_box_centered: Use 'transform' as the center of the box, not as the bottom corner
	/// duration: Duration of existence in seconds
	void draw_box_xf(const Transform3D &transform, const Color &color = Colors::empty_color, const bool &is_box_centered = true, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw AABB
	/// aabb: AABB
	/// color: Box color
	/// duration: Duration of existence in seconds
	void draw_aabb(const AABB &aabb, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw AABB from 'a' to 'b'
	/// a: First corner
	/// b: Second corner
	/// color: Box color
	/// duration: Duration of existence in seconds
	void draw_aabb_ab(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Boxes

#pragma region Lines

	/// Draw line separated by hit point (billboard square) or not separated if 'is_hit' = 'false'
	/// start: Start point
	/// end: End point
	/// hit: Hit point
	/// is_hit: Is hit
	/// hit_size: Size of the hit point
	/// hit_color: Color of the hit point and line before hit
	/// after_hit_color: Color of line after hit position
	/// duration: Duration of existence in seconds
	void draw_line_hit(const Vector3 &start, const Vector3 &end, const Vector3 &hit, const bool &is_hit, const real_t &hit_size = 0.25f, const Color &hit_color = Colors::empty_color, const Color &after_hit_color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw line separated by hit point (billboard square) or not separated if 'is_hit' = 'false'
	/// start: Start point
	/// end: End point
	/// is_hit: Is hit
	/// unit_offset_of_hit: Unit offset on the line where the hit occurs
	/// hit_size: Size of the hit point
	/// hit_color: Color of the hit point and line before hit
	/// after_hit_color: Color of line after hit position
	/// duration: Duration of existence in seconds
	void draw_line_hit_offset(const Vector3 &start, const Vector3 &end, const bool &is_hit, const real_t &unit_offset_of_hit = 0.5f, const real_t &hit_size = 0.25f, const Color &hit_color = Colors::empty_color, const Color &after_hit_color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma region Normal

	void draw_lines_c(const std::vector<Vector3> &lines, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;
	/// Draw line
	/// a: Start point
	/// b: End point
	/// color: Line color
	/// duration: Duration of existence in seconds
	void draw_line(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw many lines
	/// lines: Array of line points. 1 line = 2 Vector3. The size of the array must be even.
	/// color: Lines color
	/// duration: Duration of existence in seconds
	void draw_lines(const PackedVector3Array &lines, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw ray
	/// origin: Origin
	/// direction: Direction
	/// length: Length
	/// color: Ray color
	/// duration: Duration of existence in seconds
	void draw_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw a sequence of points connected by lines
	/// path: Sequence of points
	/// color: Lines Color
	/// duration: Duration of existence in seconds
	void draw_line_path(const PackedVector3Array &path, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Normal

#pragma region Arrows

	void create_arrow(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration = 0) FAKE_FUNC_IMPL;
	/// Draw arrow
	/// Transform: Transform3D of the Arrow
	/// color: Arrow color
	/// duration: Duration of existence in seconds
	void draw_arrow(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw line with arrow
	/// a: Start point
	/// b: End point
	/// color: Line color
	/// arrow_size: Size of the arrow
	/// absolute_size: Is the 'arrowSize' absolute or relative to the length of the line?
	/// duration: Duration of existence in seconds
	void draw_arrow_line(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &arrow_size = 0.5f, const bool &is_absolute_size = false, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw ray with arrow
	/// origin: Origin
	/// direction: Direction
	/// length: Length
	/// color: Ray color
	/// arrow_size: Size of the arrow
	/// absolute_size: Is the 'arrowSize' absolute or relative to the length of the line?
	/// duration: Duration of existence in seconds
	void draw_arrow_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color = Colors::empty_color, const real_t &arrow_size = 0.5f, const bool &is_absolute_size = false, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw a sequence of points connected by lines with arrows
	/// path: Sequence of points
	/// color: Color
	/// arrow_size: Size of the arrow
	/// absolute_size: Is the 'arrowSize' absolute or relative to the length of the line?
	/// duration: Duration of existence in seconds
	void draw_arrow_path(const PackedVector3Array &path, const Color &color = Colors::empty_color, const real_t &arrow_size = 0.75f, const bool &is_absolute_size = true, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Arrows
#pragma region Points

	/// Draw a sequence of points connected by lines with billboard squares
	/// path: Sequence of points
	/// points_color: Color of points
	/// lines_color: Color of lines
	/// size: Size of squares
	/// duration: Duration of existence in seconds
	void draw_point_path(const PackedVector3Array &path, const real_t &size = 0.25f, const Color &points_color = Colors::empty_color, const Color &lines_color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Points
#pragma endregion // Lines

#pragma region Misc

	/// Draw a square that will always be turned towards the camera
	/// position: Center position of square
	/// size: Square size
	/// color: Color
	/// duration: Duration of existence in seconds
	void draw_square(const Vector3 &position, const real_t &size = 0.2f, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw a sequence of points using billboard squares
	/// path: Sequence of points
	/// color: Color
	/// size: Size of squares
	/// duration: Duration of existence in seconds
	void draw_points(const PackedVector3Array &points, const real_t &size = 0.25f, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw 3 intersecting lines with the given transformations
	/// transform: Transform3D of lines
	/// color: Color
	/// duration: Duration of existence in seconds
	void draw_position(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw 3 lines with the given transformations and arrows at the ends
	/// transform: Transform3D of lines
	/// color: Color
	/// is_centered: If 'true', then the lines will intersect in the center of the transform
	/// duration: Duration of existence in seconds
	void draw_gizmo(const Transform3D &transform, const Color &color = Colors::empty_color, const bool &is_centered = false, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw simple grid with given size and subdivision
	/// origin: Grid origin
	/// x_size: Direction and size of the X side. As an axis in the Basis.
	/// y_size: Direction and size of the Y side. As an axis in the Basis.
	/// subdivision: Number of cells for the X and Y axes
	/// color: Lines color
	/// is_centered: Draw lines relative to origin
	/// duration: Duration of existence in seconds
	void draw_grid(const Vector3 &origin, const Vector3 &x_size, const Vector3 &y_size, const Vector2i &subdivision, const Color &color = Colors::empty_color, const bool &is_centered = true, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw simple grid with given size and subdivision
	/// transform: Transform3D of the Grid
	/// subdivision: Number of cells for the X and Y axes
	/// color: Lines color
	/// is_centered: Draw lines relative to origin
	/// duration: Duration of existence in seconds
	void draw_grid_xf(const Transform3D &transform, const Vector2i &_subdivision, const Color &color = Colors::empty_color, const bool &is_centered = true, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma region Camera Frustum

	void draw_camera_frustum_planes_c(const std::array<Plane, 6> &planes, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;
	/// Draw camera frustum area
	/// camera: Camera node
	/// color: Color
	/// duration: Duration of existence in seconds
	void draw_camera_frustum(const class godot::Camera3D *camera, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

	/// Draw camera frustum area
	/// cameraFrustum: Array of frustum planes
	/// color: Color
	/// duration: Duration of existence in seconds
	void draw_camera_frustum_planes(const Array &camera_frustum, const Color &color = Colors::empty_color, const real_t &duration = 0) FAKE_FUNC_IMPL;

#pragma endregion // Camera Frustum

#pragma endregion // Misc
#pragma endregion // Exposed Draw Functions

#undef FAKE_FUNC_IMPL
};
