@tool
extends Node3D

@export var custom_font : Font
@export var zylann_example := false
@export var test_text := true
@export var test_graphs := false
@export var more_test_cases := true
@export var draw_array_of_boxes := false
@export_range(0, 1024) var start_culling_distance := 0.0

@export_group("Text groups", "text_groups")
@export var text_groups_show_hints := true
@export var text_groups_show_stats := true
@export var text_groups_position := DebugDrawConfig2D.POSITION_LEFT_TOP
@export var text_groups_offset := Vector2i(8, 8)
@export var text_groups_padding := Vector2i(3, 1)
@export_range(1, 100) var text_groups_default_font_size := 12
@export_range(1, 100) var text_groups_title_font_size := 14
@export_range(1, 100) var text_groups_text_font_size := 12

@export_category("Graphs")
@export var graph_offset := Vector2i(8, 8)
@export var graph_size := Vector2i(200, 80)
@export_range(1, 100) var graph_title_font_size := 14
@export_range(1, 100) var graph_text_font_size := 12
@export_range(0, 64) var graph_text_precision := 2
@export_range(1, 32) var graph_line_width := 1.0
@export_range(1, 512) var graph_buffer_size := 128
@export var graph_frame_time_mode := true
@export var graph_is_enabled := true

var button_presses = {}
var time := 0.0
var time2 := 0.0
var time3 := 0.0

func _ready() -> void:
	_update_keys_just_press()
	
	await get_tree().process_frame
	
	# this check is required for inherited scenes, because an instance of this 
	# script is created first, and then overridden by another
	if !is_inside_tree():
		return


func _is_key_just_pressed(key):
	return button_presses[key] != Input.is_key_pressed(key) and Input.is_key_pressed(key)


func _update_keys_just_press():
	button_presses[KEY_LEFT] = Input.is_key_pressed(KEY_LEFT)
	button_presses[KEY_UP] = Input.is_key_pressed(KEY_UP)
	button_presses[KEY_F1] = Input.is_key_pressed(KEY_F1)


func _process(delta: float) -> void:
	# Zylann's example :D
	if zylann_example:
		DebugDraw.clear_graphs()
		var _time = Time.get_ticks_msec() / 1000.0
		var box_pos = Vector3(0, sin(_time * 4), 0)
		var line_begin = Vector3(-1, sin(_time * 4), 0)
		var line_end = Vector3(1, cos(_time * 4), 0)
		
		DebugDraw.draw_box(box_pos, Vector3(1, 2, 1), Color(0, 1, 0))
		DebugDraw.draw_line(line_begin, line_end, Color(1, 1, 0))
		DebugDraw.set_text("Time", _time)
		DebugDraw.set_text("Frames drawn", Engine.get_frames_drawn())
		DebugDraw.set_text("FPS", Engine.get_frames_per_second())
		DebugDraw.set_text("delta", delta)
		$HitTest.visible = false
		$LagTest.visible = false
		return
	
	$HitTest.visible = true
	$LagTest.visible = true
	
	# Testing the rendering layers by showing the image from the second camera inside the 2D panel
	DebugDraw.config_3d.geometry_render_layers = 1 if !Input.is_key_pressed(KEY_SHIFT) else 0b10010
	$Panel.visible = Input.is_key_pressed(KEY_SHIFT)
	DebugDraw.custom_canvas = $CustomCanvas if Input.is_key_pressed(KEY_SHIFT) else null
	
	# More property toggles
	DebugDraw.config_3d.freeze_3d_render = Input.is_key_pressed(KEY_ENTER)
	DebugDraw.debug_enabled = !Input.is_key_pressed(KEY_DOWN)
	DebugDraw.config_3d.visible_instance_bounds = Input.is_key_pressed(KEY_RIGHT)
	
	# Some property toggles
	if _is_key_just_pressed(KEY_F1):
		zylann_example = !zylann_example
	if _is_key_just_pressed(KEY_LEFT):
		DebugDraw.config_3d.use_frustum_culling = !DebugDraw.config_3d.use_frustum_culling
	if _is_key_just_pressed(KEY_UP):
		DebugDraw.config_3d.force_use_camera_from_scene = !DebugDraw.config_3d.force_use_camera_from_scene
	
	if Engine.is_editor_hint():
		DebugDraw.config_3d.cull_by_distance = start_culling_distance if DebugDraw.config_3d.force_use_camera_from_scene else 0.0
	else:
		DebugDraw.config_3d.cull_by_distance = start_culling_distance
	_update_keys_just_press()
	
	# Zones with black borders
	for z in $Zones.get_children():
		DebugDraw.draw_box_xf(z.global_transform, Color.BLACK)
	
	# Spheres
	DebugDraw.draw_sphere_xf($SphereTransform.global_transform, Color.CRIMSON)
	DebugDraw.draw_sphere_hd_xf($SphereHDTransform.global_transform, Color.ORANGE_RED)
	
	# Delayed spheres
	if time <= 0:
		DebugDraw.draw_sphere($SpherePosition.global_transform.origin, 2.0, Color.BLUE_VIOLET, 2.0)
		DebugDraw.draw_sphere_hd($SpherePosition.global_transform.origin + Vector3.FORWARD * 4, 2.0, Color.CORNFLOWER_BLUE, 2.0)
		time = 2
	time -= delta
	
	# Cylinders
	DebugDraw.draw_cylinder($Cylinder1.global_transform, Color.CRIMSON)
	DebugDraw.draw_cylinder(Transform3D(Basis.IDENTITY, $Cylinder2.global_transform.origin).scaled(Vector3(1,2,1)), Color.RED)
	
	# Boxes
	DebugDraw.draw_box_xf($Box1.global_transform, Color.MEDIUM_PURPLE)
	DebugDraw.draw_box($Box2.global_transform.origin, Vector3.ONE, Color.REBECCA_PURPLE)
	DebugDraw.draw_box_xf(Transform3D(Basis(Vector3.UP, PI * 0.25).scaled(Vector3.ONE * 2), $Box3.global_transform.origin), Color.ROSY_BROWN)
	
	DebugDraw.draw_box_xf($BoxOutOfDistabceCulling.global_transform, Color.RED)
	
	DebugDraw.draw_aabb(AABB($AABB_fixed.global_transform.origin, Vector3(2, 1, 2)), Color.AQUA)
	DebugDraw.draw_aabb_ab($AABB.get_child(0).global_transform.origin, $AABB.get_child(1).global_transform.origin, Color.DEEP_PINK)
	
	# Lines
	var target = $Lines/Target
	DebugDraw.draw_square(target.global_transform.origin, 0.5, Color.RED)
	
	DebugDraw.draw_line($"Lines/1".global_transform.origin, target.global_transform.origin, Color.FUCHSIA)
	DebugDraw.draw_ray($"Lines/3".global_transform.origin, (target.global_transform.origin - $"Lines/3".global_transform.origin).normalized(), 3.0, Color.CRIMSON)
	
	if time3 <= 0:
		DebugDraw.draw_line($"Lines/6".global_transform.origin, target.global_transform.origin, Color.FUCHSIA, 2.0)
		time3 = 2
	time3 -= delta
	
	# Lines with Arrow
	DebugDraw.draw_arrow_line($"Lines/2".global_transform.origin, target.global_transform.origin, Color.BLUE, 0.5, true)
	DebugDraw.draw_arrow_ray($"Lines/4".global_transform.origin, (target.global_transform.origin - $"Lines/4".global_transform.origin).normalized(), 8.0, Color.LAVENDER, 0.5, true)
	
	DebugDraw.draw_line_hit_offset($"Lines/5".global_transform.origin, target.global_transform.origin, true, abs(sin(Time.get_ticks_msec() / 1000.0)), 0.25, Color.AQUA)
	
	# Path
	
	## preparing data
	var points: PackedVector3Array = []
	var points_below: PackedVector3Array = []
	var points_below2: PackedVector3Array = []
	var points_below3: PackedVector3Array = []
	var lines_above: PackedVector3Array = []
	for c in $LinePath.get_children():
		points.append(c.global_transform.origin)
		points_below.append(c.global_transform.origin + Vector3.DOWN)
		points_below2.append(c.global_transform.origin + Vector3.DOWN * 2)
		points_below3.append(c.global_transform.origin + Vector3.DOWN * 3)
	for x in points.size()-1:
		lines_above.append(points[x] + Vector3.UP)
		lines_above.append(points[x+1] + Vector3.UP)
	
	## drawing lines
	DebugDraw.draw_lines(lines_above)
	DebugDraw.draw_line_path(points, Color.BEIGE)
	DebugDraw.draw_arrow_path(points_below, Color.GOLD, 0.5)
	DebugDraw.draw_points(points_below2, 0.2, Color.DARK_GREEN)
	DebugDraw.draw_point_path(points_below3, 0.25, Color.BLUE, Color.TOMATO)
	
	# Misc
	#for i in 1000:
	DebugDraw.draw_camera_frustum($Camera, Color.DARK_ORANGE)
	
	DebugDraw.draw_arrow($Misc/Arrow.global_transform, Color.YELLOW_GREEN)
	
	DebugDraw.draw_square($Misc/Billboard.global_transform.origin, 0.5, Color.GREEN)
	
	DebugDraw.draw_position($Misc/Position.global_transform, Color.BROWN)
	
	DebugDraw.draw_gizmo($Misc/GizmoTransform.global_transform, DebugDraw.empty_color, true)
	DebugDraw.draw_gizmo($Misc/GizmoNormal.global_transform.orthonormalized(), DebugDraw.empty_color, false)
	DebugDraw.draw_gizmo($Misc/GizmoOneColor.global_transform, Color.BROWN, true)
	
	var tg : Transform3D = $Misc/Grids/Grid.global_transform
	var tn : Vector3 = $Misc/Grids/Grid/Subdivision.transform.origin
	DebugDraw.draw_grid(tg.origin, tg.basis.x, tg.basis.z, Vector2i(int(tn.x*10), int(tn.z*10)), Color.LIGHT_CORAL, false)
	
	var tn1 = $Misc/Grids/GridCentered/Subdivision.transform.origin
	DebugDraw.draw_grid_xf($Misc/Grids/GridCentered.global_transform, Vector2i(tn1.x*10, tn1.z*10))
	
	# 2D
	DebugDraw.config_2d.text_default_size = text_groups_default_font_size
	DebugDraw.config_2d.text_block_offset = text_groups_offset
	DebugDraw.config_2d.text_block_position = text_groups_position
	DebugDraw.config_2d.text_padding = text_groups_padding
	
	DebugDraw.config_2d.text_custom_font = custom_font
	
	if test_text:
		_text_tests()
	
	# Graphs
	# Enable FPSGraph if not exists
	_create_graph(&"FPS", true, false, DebugDrawGraph.TEXT_CURRENT | DebugDrawGraph.TEXT_AVG | DebugDrawGraph.TEXT_MAX | DebugDrawGraph.TEXT_MIN, &"", DebugDrawGraph.SIDE_BOTTOM, DebugDrawGraph.POSITION_LEFT_TOP if Engine.is_editor_hint() else DebugDrawGraph.POSITION_RIGHT_TOP, Vector2i(200, 80), custom_font)
	if Engine.is_editor_hint():
		if DebugDraw.get_graph(&"FPS"):
			DebugDraw.get_graph(&"FPS").offset = Vector2i(0, 64)
	
	# Adding more graphs
	if test_graphs:
		_graph_test()
	else:
		_remove_graphs()
	_upd_graph_params()
	
	# Lag Test
	$LagTest.position = $LagTest/RESET.get_animation("RESET").track_get_key_value(0,0) + Vector3(sin(Time.get_ticks_msec() / 100.0) * 2.5, 0, 0)
	DebugDraw.draw_box($LagTest.global_transform.origin, Vector3.ONE * 2.01, DebugDraw.empty_color, true)
	
	if more_test_cases:
		for ray in $HitTest/RayEmitter.get_children():
			ray.set_physics_process_internal(true)
		
		_more_tests()
	else:
		for ray in $HitTest/RayEmitter.get_children():
			ray.set_physics_process_internal(false)
	
	if draw_array_of_boxes:
		_draw_array_of_boxes()


func _text_tests():
	DebugDraw.set_text("FPS", "%.2f" % Engine.get_frames_per_second(), 0, Color.GOLD)
	
	DebugDraw.begin_text_group("-- First Group --", 2, Color.LIME_GREEN, true, text_groups_title_font_size, text_groups_text_font_size)
	DebugDraw.set_text("Simple text")
	DebugDraw.set_text("Text", "Value", 0, Color.AQUAMARINE)
	DebugDraw.set_text("Text out of order", null, -1, Color.SILVER)
	DebugDraw.begin_text_group("-- Second Group --", 1, Color.BEIGE)
	DebugDraw.set_text("Rendered frames", Engine.get_frames_drawn())
	DebugDraw.end_text_group()
	
	if text_groups_show_stats:
		DebugDraw.begin_text_group("-- Stats --", 3, Color.WHEAT)
		
		var render_stats := DebugDraw.get_render_stats()
		if render_stats:
			DebugDraw.set_text("Total", render_stats.total_geometry)
			DebugDraw.set_text("Instances", render_stats.instances, 1)
			DebugDraw.set_text("Lines", render_stats.lines, 2)
			DebugDraw.set_text("Total Visible", render_stats.total_visible, 3)
			DebugDraw.set_text("Visible Instances", render_stats.visible_instances, 4)
			DebugDraw.set_text("Visible Lines", render_stats.visible_lines, 5)
			
			DebugDraw.set_text("---", null, 6)
			
			DebugDraw.set_text("Culling time", "%.2f ms" % (render_stats.total_time_culling_usec / 1000.0), 7)
			DebugDraw.set_text("Filling instances buffer", "%.2f ms" % (render_stats.time_filling_buffers_instances_usec / 1000.0), 8)
			DebugDraw.set_text("Filling lines buffer", "%.2f ms" % (render_stats.time_filling_buffers_lines_usec / 1000.0), 9)
			DebugDraw.set_text("Filling time", "%.2f ms" % (render_stats.total_time_filling_buffers_usec / 1000.0), 10)
			DebugDraw.set_text("Total time", "%.2f ms" % (render_stats.total_time_spent_usec / 1000.0), 11)
			
		DebugDraw.end_text_group()
	
	if text_groups_show_hints:
		DebugDraw.begin_text_group("controls", 1024, Color.WHITE, false)
		DebugDraw.set_text("Shift: change render layers", DebugDraw.config_3d.geometry_render_layers, 1)
		DebugDraw.set_text("Enter: freeze render", DebugDraw.config_3d.freeze_3d_render, 2)
		DebugDraw.set_text("Up: use scene camera", DebugDraw.config_3d.force_use_camera_from_scene, 3)
		DebugDraw.set_text("Down: toggle debug", DebugDraw.debug_enabled, 4)
		DebugDraw.set_text("Left: toggle frustum culling", DebugDraw.config_3d.use_frustum_culling, 5)
		DebugDraw.set_text("Right: draw bounds for culling", DebugDraw.config_3d.visible_instance_bounds, 6)


func _more_tests():
	# Line hits render
	for ray in $HitTest/RayEmitter.get_children():
		if ray is RayCast3D:
			DebugDraw.draw_line_hit(ray.global_transform.origin, ray.to_global(ray.target_position), ray.get_collision_point(), ray.is_colliding(), 0.15)
	
		# Delayed line render
	DebugDraw.draw_line($LagTest.global_transform.origin + Vector3.UP, $LagTest.global_transform.origin + Vector3(0,3,sin(Time.get_ticks_msec() / 50.0)), DebugDraw.empty_color, 0.5)


func _draw_array_of_boxes():
	# Lots of boxes to check performance.. I guess..
	if time2 <= 0:
		for x in 50:
			for y in 50:
				for z in 3:
					DebugDraw.draw_box(Vector3(x, -4-z, y), Vector3.ONE, DebugDraw.empty_color, false, 1.25)
		time2 = 1.25
	time2 -= get_process_delta_time()


func _graph_test():
# warning-ignore:return_value_discarded
	_create_graph(&"fps", true, true, DebugDrawGraph.TEXT_CURRENT, &"", DebugDrawGraph.SIDE_LEFT, DebugDrawGraph.POSITION_RIGHT_TOP)
# warning-ignore:return_value_discarded
	_create_graph(&"fps2", true, false, DebugDrawGraph.TEXT_CURRENT, &"fps", DebugDrawGraph.SIDE_BOTTOM, 0, Vector2i(200, 100))
# warning-ignore:return_value_discarded
	_create_graph(&"Sin Wave!", false, true, DebugDrawGraph.TEXT_CURRENT, &"fps2", DebugDrawGraph.SIDE_BOTTOM)
	
# warning-ignore:return_value_discarded
	_create_graph(&"randf", false, true, DebugDrawGraph.TEXT_AVG, &"", DebugDrawGraph.SIDE_LEFT, DebugDrawGraph.POSITION_RIGHT_BOTTOM, Vector2i(256, 60), custom_font)
# warning-ignore:return_value_discarded
	_create_graph(&"fps5", true, true, DebugDrawGraph.TEXT_ALL, &"randf", DebugDrawGraph.SIDE_TOP)
# warning-ignore:return_value_discarded
	_create_graph(&"fps6", true, true, DebugDrawGraph.TEXT_ALL, &"fps5", DebugDrawGraph.SIDE_TOP)
# warning-ignore:return_value_discarded
	_create_graph(&"fps12", true, true, DebugDrawGraph.TEXT_ALL, &"fps5", DebugDrawGraph.SIDE_LEFT)
	
# warning-ignore:return_value_discarded
	_create_graph(&"fps7", true, false, DebugDrawGraph.TEXT_ALL, &"FPS", DebugDrawGraph.SIDE_BOTTOM)
# warning-ignore:return_value_discarded
	_create_graph(&"fps8", true, true, DebugDrawGraph.TEXT_ALL, &"", DebugDrawGraph.SIDE_TOP, DebugDrawGraph.POSITION_LEFT_BOTTOM)
# warning-ignore:return_value_discarded
	_create_graph(&"fps9", true, false, DebugDrawGraph.TEXT_ALL, &"fps8", DebugDrawGraph.SIDE_RIGHT)
# warning-ignore:return_value_discarded
	_create_graph(&"fps10", true, false, DebugDrawGraph.TEXT_ALL, &"fps8", DebugDrawGraph.SIDE_TOP)
	# warning-ignore:return_value_discarded
	_create_graph(&"fps11", true, true, DebugDrawGraph.TEXT_ALL, &"fps9", DebugDrawGraph.SIDE_RIGHT)
	
	# If graphs exists, then more tests are done
	DebugDraw.get_graph(&"Sin Wave!").data_getter = Callable(self, &"_get_sin_wave_for_graph")
	DebugDraw.get_graph(&"Sin Wave!").upside_down =false
	
	DebugDraw.get_graph(&"randf").text_suffix = "utf8 ноль zéro"
	#DebugDraw.get_graph(&"fps9").line_position = DebugDrawGraph.LINE_TOP
	DebugDraw.get_graph(&"fps9").offset = Vector2i(0, 0)
	#DebugDraw.get_graph(&"fps11").line_position = DebugDrawGraph.LINE_BOTTOM
	DebugDraw.get_graph(&"fps11").offset = Vector2i(16, 0)
	DebugDraw.get_graph(&"fps6").offset = Vector2i(0, 32)
	DebugDraw.get_graph(&"fps").offset = Vector2i(16, 72)
	
	DebugDraw.get_graph(&"fps9").enabled = graph_is_enabled
	if !Engine.is_editor_hint():
		DebugDraw.get_graph(&"fps").corner = DebugDrawGraph.POSITION_LEFT_TOP
	
	# Just sending random data to the graph
	DebugDraw.graph_update_data(&"randf", randf())


func _upd_graph_params():
	DebugDraw.config_2d.graphs_base_offset = graph_offset
	for g in [&"FPS", &"fps5", &"fps8"]:
		var graph := DebugDraw.get_graph(g) as DebugDrawFPSGraph
		if graph:
			graph.size = graph_size
			graph.title_size = graph_title_font_size
			graph.text_size = graph_text_font_size
			graph.line_width = graph_line_width
			graph.text_precision = graph_text_precision
			graph.buffer_size = graph_buffer_size
			if Engine.is_editor_hint() or g != &"FPS":
				graph.frame_time_mode = graph_frame_time_mode


func _get_sin_wave_for_graph() -> float:
	var mul = 4 if Input.is_key_pressed(KEY_END) else 2
	return sin(Engine.get_frames_drawn() * 0.5) * mul


func _remove_graphs():
	DebugDraw.remove_graph(&"randf")
	DebugDraw.remove_graph(&"fps")
	DebugDraw.remove_graph(&"fps2")
	DebugDraw.remove_graph(&"Sin Wave!")
	DebugDraw.remove_graph(&"fps5")
	DebugDraw.remove_graph(&"fps6")
	DebugDraw.remove_graph(&"fps7")
	DebugDraw.remove_graph(&"fps8")
	DebugDraw.remove_graph(&"fps9")
	DebugDraw.remove_graph(&"fps10")
	DebugDraw.remove_graph(&"fps11")
	DebugDraw.remove_graph(&"fps12")


func _create_graph(title, is_fps, show_title, flags, parent := &"", parent_side := DebugDrawGraph.SIDE_BOTTOM, pos = DebugDrawGraph.POSITION_LEFT_BOTTOM, size := Vector2i(256, 60), font = null) -> DebugDrawGraph:
	var graph := DebugDraw.get_graph(title)
	if !graph:
		if is_fps:
			graph = DebugDraw.create_fps_graph(title)
		else:
			graph = DebugDraw.create_graph(title)
		
		if graph:
			graph.size = size
			graph.buffer_size = 50
			graph.corner = pos
			graph.show_title = show_title
			graph.show_text_flags = flags
			graph.custom_font = font
			graph.set_parent(parent, parent_side)
	
	return graph
