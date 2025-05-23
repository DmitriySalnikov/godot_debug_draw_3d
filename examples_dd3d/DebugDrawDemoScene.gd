@tool
extends Node3D

@export var custom_font : Font
@export var custom_3d_font : Font
@export var zylann_example := false
@export var update_in_physics := false
@export var test_text := true
@export var more_test_cases := true
@export var draw_3d_text := true
@export var draw_array_of_boxes := false
@export var draw_text_with_boxes := false
@export var draw_1m_boxes := false
@export_range(0, 5, 0.001) var debug_thickness := 0.1
@export_range(0, 1, 0.001) var debug_center_brightness := 0.8
@export_range(0, 1) var camera_frustum_scale := 0.9

@export_group("Text groups", "text_groups")
@export var text_groups_show_examples := true
@export var text_groups_show_hints := true
@export var text_groups_show_stats := false
@export var text_groups_show_stats_2d := false
@export var text_groups_position := DebugDraw2DConfig.POSITION_LEFT_TOP
@export var text_groups_offset := Vector2i(8, 8)
@export var text_groups_padding := Vector2i(3, 1)
@export_range(1, 100) var text_groups_default_font_size := 15
@export_range(1, 100) var text_groups_title_font_size := 20
@export_range(1, 100) var text_groups_text_font_size := 17

@export_group("Tests", "tests")
@export var tests_use_threads := false
var test_thread : Thread = null
var test_thread_closing := false

var button_presses := {}
var frame_rendered := false
var physics_tick_processed := false

var timer_1 := 0.0
var timer_cubes := 0.0
var timer_3 := 0.0
var timer_text := 0.0


func _process(delta) -> void:
	#print("Label3Ds count: %d" % get_child(0).get_child_count() if Engine.is_editor_hint() else get_tree().root.get_child(0).get_child_count())

	$OtherWorld.mesh.material.set_shader_parameter("albedo_texture", $OtherWorld/SubViewport.get_texture())
	
	physics_tick_processed = false
	if not update_in_physics:
			main_update(delta)
			_update_timers(delta)
	
	_call_from_thread()


## Since physics frames may not be called every frame or may be called multiple times in one frame,
## there is an additional check to ensure that a new frame has been drawn before updating the data.
func _physics_process(delta: float) -> void:
	if not physics_tick_processed:
		physics_tick_processed = true
		if update_in_physics:
			main_update(delta)
			_update_timers(delta)
		
		# Physics specific:
		if not zylann_example:
			DebugDraw3D.draw_line($"Lines/8".global_position, $Lines/Target.global_position, Color.YELLOW)
			
			if more_test_cases:
				_draw_rays_casts()

			## Additional drawing in the Viewport
			if true:
				var _w1 = DebugDraw3D.new_scoped_config().set_viewport(%OtherWorldBox.get_viewport()).set_thickness(0.01).set_center_brightness(1).set_no_depth_test(true)
				DebugDraw3D.draw_box_xf(Transform3D(Basis()
				.scaled(Vector3.ONE*0.3)
				.rotated(Vector3(0,0,1), PI/4)
				.rotated(Vector3(0,1,0), wrapf(Time.get_ticks_msec() / -1500.0, 0, TAU) - PI/4), %OtherWorldBox.global_transform.origin),
				Color.BROWN, true, 0.4)


func main_update(delta: float) -> void:
	DebugDraw3D.scoped_config().set_thickness(debug_thickness).set_center_brightness(debug_center_brightness)
	
	_update_keys_just_press()
	
	if _is_key_just_pressed(KEY_F1):
		zylann_example = !zylann_example
	
	# Zylann's example :D
	if zylann_example:
		var _time = Time.get_ticks_msec() / 1000.0
		var box_pos = Vector3(0, sin(_time * 4), 0)
		var line_begin = Vector3(-1, sin(_time * 4), 0)
		var line_end = Vector3(1, cos(_time * 4), 0)
		
		DebugDraw3D.draw_box(box_pos, Quaternion.IDENTITY, Vector3(1, 2, 1), Color(0, 1, 0))
		DebugDraw3D.draw_line(line_begin, line_end, Color(1, 1, 0))
		DebugDraw2D.set_text("Time", _time)
		DebugDraw2D.set_text("Frames drawn", Engine.get_frames_drawn())
		DebugDraw2D.set_text("FPS", Engine.get_frames_per_second())
		DebugDraw2D.set_text("delta", delta)
		
		$HitTest.visible = false
		$LagTest.visible = false
		$PlaneOrigin.visible = false
		$OtherWorld.visible = false
		%ZDepthTestCube.visible = false
		return
	
	$HitTest.visible = true
	$LagTest.visible = true
	$PlaneOrigin.visible = true
	$OtherWorld.visible = true
	%ZDepthTestCube.visible = true
	
	# Testing the rendering layers by showing the image from the second camera inside the 2D panel
	DebugDraw3D.config.geometry_render_layers = 1 if not Input.is_key_pressed(KEY_ALT) else 0b10010
	$Panel.visible = Input.is_key_pressed(KEY_ALT)
	DebugDraw2D.custom_canvas = %CustomCanvas if Input.is_key_pressed(KEY_ALT) else null
	
	# More property toggles
	DebugDraw3D.config.freeze_3d_render = Input.is_key_pressed(KEY_DOWN)
	DebugDraw3D.config.visible_instance_bounds = Input.is_key_pressed(KEY_RIGHT)
	
	# Regenerate meshes
	if Input.is_action_just_pressed("ui_end"):
		DebugDraw3D.regenerate_geometry_meshes()
	
	# Some property toggles
	if _is_key_just_pressed(KEY_LEFT):
		DebugDraw3D.config.use_frustum_culling = !DebugDraw3D.config.use_frustum_culling
	if _is_key_just_pressed(KEY_UP):
		DebugDraw3D.config.force_use_camera_from_scene = !DebugDraw3D.config.force_use_camera_from_scene
	if _is_key_just_pressed(KEY_CTRL):
		if not Engine.is_editor_hint():
			get_viewport().msaa_3d = Viewport.MSAA_DISABLED if get_viewport().msaa_3d == Viewport.MSAA_4X else Viewport.MSAA_4X
	
	if not Engine.is_editor_hint():
		if _is_key_just_pressed(KEY_1):
			DebugDraw3D.debug_enabled = !DebugDraw3D.debug_enabled
		if _is_key_just_pressed(KEY_2):
			DebugDraw2D.debug_enabled = !DebugDraw2D.debug_enabled
		if _is_key_just_pressed(KEY_3):
			DebugDrawManager.debug_enabled = !DebugDrawManager.debug_enabled
	
	
	DebugDraw3D.config.frustum_length_scale = camera_frustum_scale
	
	# Zones with black borders
	for z in $Zones.get_children():
		DebugDraw3D.draw_box_xf(z.global_transform, Color.BLACK)
	
	# Spheres
	_draw_zone_title(%SpheresBox, "Spheres")
	
	DebugDraw3D.draw_sphere_xf($Spheres/SphereTransform.global_transform, Color.CRIMSON)
	if true:
		var _shd = DebugDraw3D.new_scoped_config().set_hd_sphere(true)
		DebugDraw3D.draw_sphere_xf($Spheres/SphereHDTransform.global_transform, Color.ORANGE_RED)
	
	## Delayed spheres
	if timer_1 < 0:
		DebugDraw3D.draw_sphere($Spheres/SpherePosition.global_position, 2.0, Color.BLUE_VIOLET, 2.0)
		var _shd = DebugDraw3D.new_scoped_config().set_hd_sphere(true)
		DebugDraw3D.draw_sphere($Spheres/SpherePosition.global_position + Vector3.FORWARD * 4, 2.0, Color.CORNFLOWER_BLUE, 2.0)
		timer_1 = 2
	
	# Cylinders
	_draw_zone_title(%CylindersBox, "Cylinders")
	
	DebugDraw3D.draw_cylinder($Cylinders/Cylinder1.global_transform, Color.CRIMSON)
	DebugDraw3D.draw_cylinder(Transform3D(Basis.IDENTITY.scaled(Vector3(1,2,1)), $Cylinders/Cylinder2.global_position), Color.RED)
	DebugDraw3D.draw_cylinder_ab($"Cylinders/Cylinder3/1".global_position, $"Cylinders/Cylinder3/2".global_position, 0.7)
	
	# Boxes
	_draw_zone_title(%BoxesBox, "Boxes")
	
	DebugDraw3D.draw_box_xf($Boxes/Box1.global_transform, Color.MEDIUM_PURPLE)
	DebugDraw3D.draw_box($Boxes/Box2.global_position, Quaternion.from_euler(Vector3(0, deg_to_rad(45), deg_to_rad(45))), Vector3.ONE, Color.REBECCA_PURPLE)
	DebugDraw3D.draw_box_xf(Transform3D(Basis(Vector3.UP, PI * 0.25).scaled(Vector3.ONE * 2), $Boxes/Box3.global_position), Color.ROSY_BROWN)
	
	DebugDraw3D.draw_aabb(AABB($Boxes/AABB_fixed.global_position, Vector3(2, 1, 2)), Color.AQUA)
	DebugDraw3D.draw_aabb_ab($Boxes/AABB/a.global_position, $Boxes/AABB/b.global_position, Color.DEEP_PINK)
	
	# Boxes AB
	DebugDraw3D.draw_arrow($Boxes/BoxAB.global_position, $Boxes/BoxAB/o/up.global_position, Color.GOLD, 0.1, true)
	DebugDraw3D.draw_box_ab($Boxes/BoxAB/a.global_position, $Boxes/BoxAB/b.global_position, $Boxes/BoxAB/o/up.global_position - $Boxes/BoxAB.global_position, Color.PERU)
	
	DebugDraw3D.draw_arrow($Boxes/BoxABEdge.global_position, $Boxes/BoxABEdge/o/up.global_position, Color.DARK_RED, 0.1, true)
	DebugDraw3D.draw_box_ab($Boxes/BoxABEdge/a.global_position, $Boxes/BoxABEdge/b.global_position, $Boxes/BoxABEdge/o/up.global_position - $Boxes/BoxABEdge.global_position, Color.DARK_OLIVE_GREEN, false)
	
	# Lines
	_draw_zone_title(%LinesBox, "Lines")
	
	var target = $Lines/Target
	DebugDraw3D.draw_square(target.global_position, 0.5, Color.RED)
	
	DebugDraw3D.draw_line($"Lines/1".global_position, target.global_position, Color.FUCHSIA)
	DebugDraw3D.draw_ray($"Lines/3".global_position, (target.global_position - $"Lines/3".global_position).normalized(), 3.0, Color.CRIMSON)
	
	if timer_3 < 0:
		DebugDraw3D.draw_line($"Lines/6".global_position, target.global_position, Color.FUCHSIA, 2.0)
		timer_3 = 2
	
	# Test UP vector
	DebugDraw3D.draw_line($"Lines/7".global_position, target.global_position, Color.RED)
	
	# Lines with Arrow
	DebugDraw3D.draw_arrow($"Lines/2".global_position, target.global_position, Color.BLUE, 0.5, true)
	DebugDraw3D.draw_arrow_ray($"Lines/4".global_position, (target.global_position - $"Lines/4".global_position).normalized(), 8.0, Color.LAVENDER, 0.5, true)
	
	DebugDraw3D.draw_line_hit_offset($"Lines/5".global_position, target.global_position, true, abs(sin(Time.get_ticks_msec() / 1000.0)), 0.25, Color.AQUA)
	
	# Paths
	_draw_zone_title(%PathsBox, "Paths")
	
	## preparing data
	var points: PackedVector3Array = []
	var points_below: PackedVector3Array = []
	var points_below2: PackedVector3Array = []
	var points_below3: PackedVector3Array = []
	var points_below4: PackedVector3Array = []
	var lines_above: PackedVector3Array = []
	
	for c in $LinePath.get_children():
		if not c is Node3D:
			break
		points.append(c.global_position)
		points_below.append(c.global_position + Vector3.DOWN)
		points_below2.append(c.global_position + Vector3.DOWN * 2)
		points_below3.append(c.global_position + Vector3.DOWN * 3)
		points_below4.append(c.global_position + Vector3.DOWN * 4)
	
	for x in points.size()-1:
		lines_above.append(points[x] + Vector3.UP)
		lines_above.append(points[x+1] + Vector3.UP)
	
	## drawing lines
	DebugDraw3D.draw_lines(lines_above)
	DebugDraw3D.draw_line_path(points, Color.BEIGE)
	DebugDraw3D.draw_points(points_below, DebugDraw3D.POINT_TYPE_SQUARE, 0.2, Color.DARK_GREEN)
	DebugDraw3D.draw_point_path(points_below2, DebugDraw3D.POINT_TYPE_SQUARE, 0.25, Color.BLUE, Color.TOMATO)
	DebugDraw3D.draw_arrow_path(points_below3, Color.GOLD, 0.5)
	if true:
		var _sl = DebugDraw3D.new_scoped_config().set_thickness(0.05)
		DebugDraw3D.draw_point_path(points_below4, DebugDraw3D.POINT_TYPE_SPHERE, 0.25, Color.MEDIUM_SEA_GREEN, Color.MEDIUM_VIOLET_RED)
	
	# Misc
	_draw_zone_title(%MiscBox, "Misc")
	
	if Engine.is_editor_hint():
		#for i in 1000:
		var _a11 = DebugDraw3D.new_scoped_config().set_thickness(0)
		DebugDraw3D.draw_camera_frustum($Camera, Color.DARK_ORANGE)
	
	if true:
		var _s123 = DebugDraw3D.new_scoped_config().set_center_brightness(0.1)
		DebugDraw3D.draw_arrowhead($Misc/Arrow.global_transform, Color.YELLOW_GREEN)
	
	DebugDraw3D.draw_square($Misc/Billboard.global_position, 0.5, Color.GREEN)
	
	DebugDraw3D.draw_position($Misc/Position.global_transform, Color.BROWN)
	
	DebugDraw3D.draw_gizmo($Misc/GizmoTransform.global_transform, DebugDraw3D.empty_color, true)
	DebugDraw3D.draw_gizmo($Misc/GizmoOneColor.global_transform, Color.BROWN, true)
	if true:
		var _s123 = DebugDraw3D.new_scoped_config().set_center_brightness(0.5).set_no_depth_test(true)
		DebugDraw3D.draw_gizmo($Misc/GizmoNormal.global_transform.orthonormalized(), DebugDraw3D.empty_color, false)
	
	# Grids
	_draw_zone_title_pos($Grids/GridCentered.global_position + Vector3(0, 1.5, 0), "Grids", 96, 36)
	
	var tg : Transform3D = $Grids/Grid.global_transform
	var tn : Vector3 = $Grids/Grid/Subdivision.transform.origin
	DebugDraw3D.draw_grid(tg.origin, tg.basis.x, tg.basis.z, Vector2i(int(tn.x*10), int(tn.z*10)), Color.LIGHT_CORAL, false)
	
	var tn1 = $Grids/GridCentered/Subdivision.transform.origin
	DebugDraw3D.draw_grid_xf($Grids/GridCentered.global_transform, Vector2i(tn1.x*10, tn1.z*10))
	
	if true:
		var _s32 = DebugDraw3D.new_scoped_config().set_thickness(0.05)
		DebugDraw3D.draw_box_xf($PostProcess.global_transform, Color.SEA_GREEN)
	
	# Local transform
	_draw_local_xf_box(%LocalTransformRecursiveOrigin.global_transform, 0.05, 10)
	
	# 2D
	DebugDraw2D.config.text_default_size = text_groups_default_font_size
	DebugDraw2D.config.text_block_offset = text_groups_offset
	DebugDraw2D.config.text_block_position = text_groups_position
	DebugDraw2D.config.text_padding = text_groups_padding
	
	DebugDraw2D.config.text_custom_font = custom_font
	
	if test_text:
		_text_tests()
	
	# Lag Test
	var lag_test_pos = $LagTest/RESET.get_animation("RESET").track_get_key_value(0,0)
	_draw_zone_title_pos(lag_test_pos, "Lag test")
	
	$LagTest.position = lag_test_pos + Vector3(sin(Time.get_ticks_msec() / 100.0) * 2.5, 0, 0)
	DebugDraw3D.draw_box($LagTest.global_position, Quaternion.IDENTITY, Vector3.ONE * 2.01, Color.CHOCOLATE, true)
	
	if more_test_cases:
		for ray in $HitTest/RayEmitter.get_children():
			ray.set_physics_process_internal(true)
		
		_more_tests()
	else:
		for ray in $HitTest/RayEmitter.get_children():
			ray.set_physics_process_internal(false)
	
	_draw_other_world()
	
	if draw_array_of_boxes:
		_draw_array_of_boxes()


func _text_tests():
	DebugDraw2D.set_text("FPS", "%.2f" % Engine.get_frames_per_second(), 0, Color.GOLD)
	
	if text_groups_show_examples:
		if timer_text < 0:
			DebugDraw2D.set_text("Some delayed text", "for 2.5s", -1, Color.BLACK, 2.5) # it's supposed to show text for 2.5 seconds
			timer_text = 5
		
		DebugDraw2D.begin_text_group("-- First Group --", 2, Color.LIME_GREEN, true, text_groups_title_font_size, text_groups_text_font_size)
		DebugDraw2D.set_text("Simple text")
		DebugDraw2D.set_text("Text", "Value", 0, Color.AQUAMARINE)
		DebugDraw2D.set_text("Text out of order", null, -1, Color.SILVER)
		DebugDraw2D.begin_text_group("-- Second Group --", 1, Color.BEIGE)
		DebugDraw2D.set_text("Rendered frames", Engine.get_frames_drawn())
		DebugDraw2D.end_text_group()
	
	if text_groups_show_stats or text_groups_show_stats_2d:
		DebugDraw2D.begin_text_group("-- Stats --", 3, Color.WHEAT)
		
		var render_stats := DebugDraw3D.get_render_stats()
		if render_stats && text_groups_show_stats:
			DebugDraw2D.set_text("Total", render_stats.total_geometry)
			DebugDraw2D.set_text("Instances", render_stats.instances + render_stats.instances_physics, 1)
			DebugDraw2D.set_text("Lines", render_stats.lines + render_stats.lines_physics, 2)
			DebugDraw2D.set_text("Total Visible", render_stats.total_visible, 3)
			DebugDraw2D.set_text("Visible Instances", render_stats.visible_instances, 4)
			DebugDraw2D.set_text("Visible Lines", render_stats.visible_lines, 5)
			
			DebugDraw2D.set_text("---", null, 12)
			
			DebugDraw2D.set_text("Culling time", "%.2f ms" % (render_stats.total_time_culling_usec / 1000.0), 13)
			DebugDraw2D.set_text("Filling instances buffer", "%.2f ms" % (render_stats.time_filling_buffers_instances_usec / 1000.0), 14)
			DebugDraw2D.set_text("Filling lines buffer", "%.2f ms" % (render_stats.time_filling_buffers_lines_usec / 1000.0), 15)
			DebugDraw2D.set_text("Filling time", "%.2f ms" % (render_stats.total_time_filling_buffers_usec / 1000.0), 16)
			DebugDraw2D.set_text("Total time", "%.2f ms" % (render_stats.total_time_spent_usec / 1000.0), 17)
			
			DebugDraw2D.set_text("----", null, 32)
			
			DebugDraw2D.set_text("Total Label3D", render_stats.nodes_label3d_exists_total, 33)
			DebugDraw2D.set_text("Visible Label3D", render_stats.nodes_label3d_visible + render_stats.nodes_label3d_visible_physics, 34)
			
			DebugDraw2D.set_text("-----", null, 48)
			
			DebugDraw2D.set_text("Created scoped configs", "%d" % render_stats.created_scoped_configs, 49)
		
		if text_groups_show_stats && text_groups_show_stats_2d:
			DebugDraw2D.set_text("------", null, 64)
		
		var render_stats_2d := DebugDraw2D.get_render_stats()
		if render_stats_2d && text_groups_show_stats_2d:
			DebugDraw2D.set_text("Text groups", render_stats_2d.overlay_text_groups, 96)
			DebugDraw2D.set_text("Text lines", render_stats_2d.overlay_text_lines, 97)
			
		DebugDraw2D.end_text_group()
	
	if text_groups_show_hints:
		DebugDraw2D.begin_text_group("controls", 1024, Color.WHITE, false)
		if not Engine.is_editor_hint():
			DebugDraw2D.set_text("WASD QE, LMB", "To move", 0)
		DebugDraw2D.set_text("Alt: change render layers", DebugDraw3D.config.geometry_render_layers, 1)
		if not OS.has_feature("web"):
			DebugDraw2D.set_text("Ctrl: toggle anti-aliasing", "MSAA 4x" if get_viewport().msaa_3d == Viewport.MSAA_4X else "Disabled", 2)
		DebugDraw2D.set_text("Down: freeze render", DebugDraw3D.config.freeze_3d_render, 3)
		if Engine.is_editor_hint():
			DebugDraw2D.set_text("Up: use scene camera", DebugDraw3D.config.force_use_camera_from_scene, 4)
		DebugDraw2D.set_text("1,2,3: toggle debug", "%s, %s 😐, %s 😏" % [DebugDraw3D.debug_enabled, DebugDraw2D.debug_enabled, DebugDrawManager.debug_enabled], 5)
		DebugDraw2D.set_text("Left: toggle frustum culling", DebugDraw3D.config.use_frustum_culling, 6)
		DebugDraw2D.set_text("Right: draw bounds for culling", DebugDraw3D.config.visible_instance_bounds, 7)
		DebugDraw2D.end_text_group()


func _draw_zone_title(node: Node3D, title: String):
	if draw_3d_text:
		var _s1 = DebugDraw3D.new_scoped_config().set_text_outline_size(72)
		DebugDraw3D.draw_text(node.global_position + node.global_basis.y * 0.85, title, 128)


func _draw_zone_title_pos(pos: Vector3, title: String, font_size: int = 128, outline: int = 72):
	if draw_3d_text:
		var _s1 = DebugDraw3D.new_scoped_config().set_text_outline_size(outline)
		DebugDraw3D.draw_text(pos, title, font_size)


const _local_mul := 0.45
const _local_mul_vec := Vector3(_local_mul, _local_mul, _local_mul)
var __local_lines_cross_recursive = PackedVector3Array([Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, 0.5), Vector3(-0.5, -0.5, 0.5), Vector3(0.5, -0.5, -0.5)])
var __local_box_recursive = Transform3D.IDENTITY.rotated_local(Vector3.UP, deg_to_rad(30)).translated(Vector3(-0.25, -0.55, 0.25)).scaled(_local_mul_vec)
var __local_sphere_recursive = Transform3D.IDENTITY.translated(Vector3(0.5, 0.55, -0.5)).scaled(_local_mul_vec)

func _draw_local_xf_box(xf: Transform3D, thickness: float, max_depth: int, depth: int = 0):
	if depth >= max_depth:
		return
	
	var _s1 = DebugDraw3D.new_scoped_config().set_thickness(thickness).set_transform(xf)
	
	# a box with a small offset
	DebugDraw3D.draw_box_xf(Transform3D(Basis(), Vector3(0, 0.001, 0)), Color.BROWN)
	# a box and a stand for the next depth
	DebugDraw3D.draw_box_xf(__local_box_recursive, Color.CHARTREUSE)
	# just a sphere and lines
	DebugDraw3D.draw_sphere_xf(__local_sphere_recursive, Color.DARK_ORANGE)
	_s1.set_thickness(0)
	DebugDraw3D.draw_lines(__local_lines_cross_recursive, Color.CRIMSON)
	
	# A simple animation generator with descent into the depth of the scene
	if false:
		var anim: Animation = %RecursiveTransformTest.get_animation("recursive")
		# clear keys
		if depth == 0: for i in anim.track_get_key_count(0): anim.track_remove_key(0, 0); anim.track_remove_key(1, 0)
		
		var time = depth * 2
		var s_xf = xf * __local_sphere_recursive
		var next_s_xf = (xf * __local_box_recursive.translated(__local_box_recursive.basis.y)) * __local_sphere_recursive
		var get_sphere_pos = func(l_xf): return l_xf.origin + (l_xf).basis.y
		anim.position_track_insert_key(0, time, get_sphere_pos.call(s_xf))
		anim.rotation_track_insert_key(1, time, Transform3D(Basis(), get_sphere_pos.call(s_xf)).looking_at(get_sphere_pos.call(next_s_xf), xf.basis.y).basis.get_rotation_quaternion())
	
	_draw_local_xf_box(xf * __local_box_recursive.translated(__local_box_recursive.basis.y), thickness * _local_mul, max_depth, depth + 1)


func _draw_other_world():
	var _w1 = DebugDraw3D.new_scoped_config().set_viewport(%OtherWorldBox.get_viewport())
	DebugDraw3D.draw_box_xf(%OtherWorldBox.global_transform.rotated_local(Vector3(1,1,-1).normalized(), wrapf(Time.get_ticks_msec() / 1000.0, 0, TAU)), Color.SANDY_BROWN)
	DebugDraw3D.draw_box_xf(%OtherWorldBox.global_transform.rotated_local(Vector3(-1,1,-1).normalized(), wrapf(Time.get_ticks_msec() / -1000.0, 0, TAU) - PI/4), Color.SANDY_BROWN)
	
	if draw_3d_text:
		var angle = wrapf(Time.get_ticks_msec() / 1000.0, 0, TAU)
		if true:
			var _w2 = DebugDraw3D.new_scoped_config().set_text_font(custom_3d_font)
			DebugDraw3D.draw_text(%OtherWorldBox.global_position + Vector3(cos(angle), -0.25, sin(angle)), "Hello world!", 32, Color.CRIMSON, 0)
		
		if true:
			var _w3 = DebugDraw3D.new_scoped_config().set_no_depth_test(true).set_text_outline_color(Color.INDIAN_RED).set_text_outline_size(6)
			DebugDraw3D.draw_text(%OtherWorldBox.global_position + Vector3(cos(angle), +0.25, sin(-angle)), "World without depth", 20, Color.PINK, 0)


func _draw_rays_casts():
	# Line hits render
	_draw_zone_title_pos(%HitTestSphere.global_position, "Line hits", 96, 36)
	
	for ray in $HitTest/RayEmitter.get_children():
		if ray is RayCast3D:
			ray.force_raycast_update()
			DebugDraw3D.draw_line_hit(ray.global_position, ray.to_global(ray.target_position), ray.get_collision_point(), ray.is_colliding(), 0.3)


func _more_tests():
	# Delayed line render
	if true:
		var _a12 = DebugDraw3D.new_scoped_config().set_thickness(0.035)
		DebugDraw3D.draw_line($LagTest.global_position + Vector3.UP, $LagTest.global_position + Vector3(0,3,sin(Time.get_ticks_msec() / 50.0)), DebugDraw3D.empty_color, 0.35)
		
		if draw_3d_text:
			DebugDraw3D.draw_text($LagTest.global_position + Vector3(0,3,sin(Time.get_ticks_msec() / 50.0)), "%.1f" % sin(Time.get_ticks_msec() / 50.0), 16, DebugDraw3D.empty_color, 0.35)
	
	# Draw plane
	if true:
		var _s11 = DebugDraw3D.new_scoped_config().set_thickness(0.02).set_plane_size(10)
		
		var pl_node: Node3D = $PlaneOrigin
		var xf: Transform3D = pl_node.global_transform
		var normal: = xf.basis.y.normalized()
		var plane = Plane(normal, xf.origin.dot(normal))
		
		var vp: Viewport = get_viewport()
		if Engine.is_editor_hint() and Engine.get_singleton(&"EditorInterface").get_editor_viewport_3d(0):
			vp = Engine.get_singleton(&"EditorInterface").get_editor_viewport_3d(0)
		
		var cam = vp.get_camera_3d()
		if cam:
			var dir = vp.get_camera_3d().project_ray_normal(vp.get_mouse_position())
			var intersect = plane.intersects_ray(cam.global_position, dir)
			
			DebugDraw3D.draw_plane(plane, Color.CORAL * Color(1,1,1, 0.4), pl_node.global_position)
			if intersect and intersect.distance_to(pl_node.global_position) < _s11.get_plane_size() * 0.5:
				# Need to test different colors on both sides of the plane
				var col = Color.FIREBRICK if plane.is_point_over(cam.global_position) else Color.AQUAMARINE
				DebugDraw3D.draw_sphere(intersect, 0.3, col)


func _draw_array_of_boxes():
	# Lots of boxes to check performance..
	var x_size := 50
	var y_size := 50
	var z_size := 3
	var mul := 1
	var cubes_max_time := 1.25
	var show_text := draw_text_with_boxes
	var cfg = DebugDraw3D.new_scoped_config()
	
	if draw_1m_boxes:
		x_size = 100
		y_size = 100
		z_size = 100
		mul = 4
		cubes_max_time = 60
		show_text = false
	
	var size := Vector3.ONE
	var half_size := size * 0.5
	
	if timer_cubes < 0:
		var _start_time = Time.get_ticks_usec()
		for x in x_size:
			for y in y_size:
				for z in z_size:
					cfg.set_thickness(randf_range(0, 0.1))
					var pos := Vector3(x * mul, (-4-z) * mul, y * mul) + global_position
					DebugDraw3D.draw_box(pos, Quaternion.IDENTITY, size, DebugDraw3D.empty_color, false, cubes_max_time)
					
					if show_text and z == 0:
						DebugDraw3D.draw_text(pos + half_size, str(pos), 32, DebugDraw3D.empty_color, cubes_max_time)
		#print("Draw Cubes: %.3fms" % ((Time.get_ticks_usec() - _start_time) / 1000.0))
		timer_cubes = cubes_max_time


func _ready() -> void:
	_update_keys_just_press()
	
	await get_tree().process_frame
	
	# this check is required for inherited scenes, because an instance of this 
	# script is created first, and then overridden by another
	if not is_inside_tree():
		return
	
	DebugDraw2D.config.text_background_color = Color(0.3, 0.3, 0.3, 0.8)


func _is_key_just_pressed(key):
	if (button_presses[key] == 1):
		button_presses[key] = 2
		return true
	return false


func _update_keys_just_press():
	var set_key = func (k: Key): 
		if Input.is_key_pressed(k) and button_presses.has(k):
			if button_presses[k] == 0:
				return 1
			else:
				return button_presses[k]
		else:
			return 0
	button_presses[KEY_LEFT] = set_key.call(KEY_LEFT)
	button_presses[KEY_UP] = set_key.call(KEY_UP)
	button_presses[KEY_CTRL] = set_key.call(KEY_CTRL)
	button_presses[KEY_F1] = set_key.call(KEY_F1)
	button_presses[KEY_1] = set_key.call(KEY_1)
	button_presses[KEY_2] = set_key.call(KEY_2)
	button_presses[KEY_3] = set_key.call(KEY_3)


func _update_timers(delta : float):
	timer_1 -= delta
	timer_cubes -= delta
	timer_3 -= delta
	timer_text -= delta


func _notification(what: int) -> void:
	if what == NOTIFICATION_EDITOR_PRE_SAVE or what == NOTIFICATION_EXIT_TREE:
		_thread_stop()


func _call_from_thread():
	if tests_use_threads and (not test_thread or not test_thread.is_alive()):
		test_thread_closing = false
		test_thread = Thread.new()
		test_thread.start(_thread_body)
	elif not tests_use_threads and (test_thread and test_thread.is_alive()):
		_thread_stop()


func _thread_stop():
	if test_thread and test_thread.is_alive():
		tests_use_threads = false
		test_thread_closing = true
		test_thread.wait_to_finish()


func _thread_body():
	print("Thread started!")
	while not test_thread_closing:
		DebugDraw3D.draw_box(Vector3(0,-1,0), Quaternion.IDENTITY, Vector3.ONE, Color.BROWN, true, 0.016)
		
		var boxes = 10
		for y in boxes:
			var offset := sin(TAU/boxes * y + wrapf(Time.get_ticks_msec() / 100.0, 0, TAU))
			var pos := Vector3(offset, y, 0)
			DebugDraw3D.draw_box(pos, Quaternion.IDENTITY, Vector3.ONE, Color.GREEN_YELLOW, true, 0.016)
			DebugDraw3D.draw_text(pos, str(y), 64, Color.WHITE , 0.016)
			
			if y == 0:
				DebugDraw2D.set_text("thread. sin", offset)
		
		OS.delay_msec(16)
	print("Thread finished!")
