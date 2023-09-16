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
@export var text_groups_show_stats := false
@export var text_groups_show_stats_2d := false
@export var text_groups_position := DebugDrawConfig2D.POSITION_LEFT_TOP
@export var text_groups_offset := Vector2i(8, 8)
@export var text_groups_padding := Vector2i(3, 1)
@export_range(1, 100) var text_groups_default_font_size := 12
@export_range(1, 100) var text_groups_title_font_size := 14
@export_range(1, 100) var text_groups_text_font_size := 12

@export_group("Graphs", "graph")
@export var graph_offset := Vector2i(8, 8)
@export var graph_size := Vector2i(200, 80)
@export_range(1, 100) var graph_title_font_size := 14
@export_range(1, 100) var graph_text_font_size := 12
@export_range(0, 64) var graph_text_precision := 2
@export_range(1, 32) var graph_line_width := 1.0
@export_range(1, 512) var graph_buffer_size := 128
@export var graph_frame_time_mode := true
@export var graph_is_enabled := true

var button_presses := {}
var time := 0.0
var time2 := 0.0
var time3 := 0.0
var time_text := 0.0

func _ready() -> void:
	_update_keys_just_press()
	
	await get_tree().process_frame
	
	# this check is required for inherited scenes, because an instance of this 
	# script is created first, and then overridden by another
	if !is_inside_tree():
		return


func _is_key_just_pressed(key):
	if (button_presses[key] == 1):
		button_presses[key] = 2
		return true
	return false


func _update_keys_just_press():
	var set_key = func (k: Key): return (1 if button_presses[k] == 0 else button_presses[k]) if Input.is_key_pressed(k) else 0
	button_presses[KEY_LEFT] = set_key.call(KEY_LEFT)
	button_presses[KEY_UP] = set_key.call(KEY_UP)
	button_presses[KEY_F1] = set_key.call(KEY_F1)
	button_presses[KEY_1] = set_key.call(KEY_1)
	button_presses[KEY_2] = set_key.call(KEY_2)
	button_presses[KEY_3] = set_key.call(KEY_3)


func _process(delta: float) -> void:
	_update_keys_just_press()
	
	if _is_key_just_pressed(KEY_F1):
		zylann_example = !zylann_example
	
	# Zylann's example :D
	if zylann_example:
		DebugDraw2D.clear_graphs()
		var _time = Time.get_ticks_msec() / 1000.0
		var box_pos = Vector3(0, sin(_time * 4), 0)
		var line_begin = Vector3(-1, sin(_time * 4), 0)
		var line_end = Vector3(1, cos(_time * 4), 0)
		
		DebugDraw3D.draw_box(box_pos, Vector3(1, 2, 1), Color(0, 1, 0))
		DebugDraw3D.draw_line(line_begin, line_end, Color(1, 1, 0))
		DebugDraw2D.set_text("Time", _time)
		DebugDraw2D.set_text("Frames drawn", Engine.get_frames_drawn())
		DebugDraw2D.set_text("FPS", Engine.get_frames_per_second())
		DebugDraw2D.set_text("delta", delta)
		$HitTest.visible = false
		$LagTest.visible = false
		return
	
	$HitTest.visible = true
	$LagTest.visible = true
	
	# Testing the rendering layers by showing the image from the second camera inside the 2D panel
	DebugDraw3D.config.geometry_render_layers = 1 if !Input.is_key_pressed(KEY_SHIFT) else 0b10010
	$Panel.visible = Input.is_key_pressed(KEY_SHIFT)
	DebugDraw2D.custom_canvas = $CustomCanvas if Input.is_key_pressed(KEY_SHIFT) else null
	
	# More property toggles
	DebugDraw3D.config.freeze_3d_render = Input.is_key_pressed(KEY_DOWN)
	DebugDraw3D.config.visible_instance_bounds = Input.is_key_pressed(KEY_RIGHT)
	
	# Some property toggles
	if _is_key_just_pressed(KEY_LEFT):
		DebugDraw3D.config.use_frustum_culling = !DebugDraw3D.config.use_frustum_culling
	if _is_key_just_pressed(KEY_UP):
		DebugDraw3D.config.force_use_camera_from_scene = !DebugDraw3D.config.force_use_camera_from_scene
	
	if _is_key_just_pressed(KEY_1):
		DebugDraw3D.debug_enabled = !DebugDraw3D.debug_enabled
	if _is_key_just_pressed(KEY_2):
		DebugDraw2D.debug_enabled = !DebugDraw2D.debug_enabled
	if _is_key_just_pressed(KEY_3):
		DebugDrawManager.debug_enabled = !DebugDrawManager.debug_enabled
	
	
	if Engine.is_editor_hint():
		DebugDraw3D.config.cull_by_distance = start_culling_distance if DebugDraw3D.config.force_use_camera_from_scene else 0.0
	else:
		DebugDraw3D.config.cull_by_distance = start_culling_distance
	
	# Zones with black borders
	for z in $Zones.get_children():
		DebugDraw3D.draw_box_xf(z.global_transform, Color.BLACK)
	
	# Spheres
	DebugDraw3D.draw_sphere_xf($SphereTransform.global_transform, Color.CRIMSON)
	DebugDraw3D.draw_sphere_hd_xf($SphereHDTransform.global_transform, Color.ORANGE_RED)
	
	# Delayed spheres
	if time <= 0:
		DebugDraw3D.draw_sphere($SpherePosition.global_transform.origin, 2.0, Color.BLUE_VIOLET, 2.0)
		DebugDraw3D.draw_sphere_hd($SpherePosition.global_transform.origin + Vector3.FORWARD * 4, 2.0, Color.CORNFLOWER_BLUE, 2.0)
		time = 2
	time -= delta
	
	# Cylinders
	DebugDraw3D.draw_cylinder($Cylinder1.global_transform, Color.CRIMSON)
	DebugDraw3D.draw_cylinder(Transform3D(Basis.IDENTITY, $Cylinder2.global_transform.origin).scaled(Vector3(1,2,1)), Color.RED)
	
	# Boxes
	DebugDraw3D.draw_box_xf($Box1.global_transform, Color.MEDIUM_PURPLE)
	DebugDraw3D.draw_box($Box2.global_transform.origin, Vector3.ONE, Color.REBECCA_PURPLE)
	DebugDraw3D.draw_box_xf(Transform3D(Basis(Vector3.UP, PI * 0.25).scaled(Vector3.ONE * 2), $Box3.global_transform.origin), Color.ROSY_BROWN)
	
	DebugDraw3D.draw_box_xf($BoxOutOfDistanceCulling.global_transform, Color.RED)
	
	DebugDraw3D.draw_aabb(AABB($AABB_fixed.global_transform.origin, Vector3(2, 1, 2)), Color.AQUA)
	DebugDraw3D.draw_aabb_ab($AABB.get_child(0).global_transform.origin, $AABB.get_child(1).global_transform.origin, Color.DEEP_PINK)
	
	# Lines
	var target = $Lines/Target
	DebugDraw3D.draw_square(target.global_transform.origin, 0.5, Color.RED)
	
	DebugDraw3D.draw_line($"Lines/1".global_transform.origin, target.global_transform.origin, Color.FUCHSIA)
	DebugDraw3D.draw_ray($"Lines/3".global_transform.origin, (target.global_transform.origin - $"Lines/3".global_transform.origin).normalized(), 3.0, Color.CRIMSON)
	
	if time3 <= 0:
		DebugDraw3D.draw_line($"Lines/6".global_transform.origin, target.global_transform.origin, Color.FUCHSIA, 2.0)
		time3 = 2
	time3 -= delta
	
	# Lines with Arrow
	DebugDraw3D.draw_arrow_line($"Lines/2".global_transform.origin, target.global_transform.origin, Color.BLUE, 0.5, true)
	DebugDraw3D.draw_arrow_ray($"Lines/4".global_transform.origin, (target.global_transform.origin - $"Lines/4".global_transform.origin).normalized(), 8.0, Color.LAVENDER, 0.5, true)
	
	DebugDraw3D.draw_line_hit_offset($"Lines/5".global_transform.origin, target.global_transform.origin, true, abs(sin(Time.get_ticks_msec() / 1000.0)), 0.25, Color.AQUA)
	
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
	DebugDraw3D.draw_lines(lines_above)
	DebugDraw3D.draw_line_path(points, Color.BEIGE)
	DebugDraw3D.draw_arrow_path(points_below, Color.GOLD, 0.5)
	DebugDraw3D.draw_points(points_below2, 0.2, Color.DARK_GREEN)
	DebugDraw3D.draw_point_path(points_below3, 0.25, Color.BLUE, Color.TOMATO)
	
	# Misc
	#for i in 1000:
	DebugDraw3D.draw_camera_frustum($Camera, Color.DARK_ORANGE)
	
	DebugDraw3D.draw_arrow($Misc/Arrow.global_transform, Color.YELLOW_GREEN)
	
	DebugDraw3D.draw_square($Misc/Billboard.global_transform.origin, 0.5, Color.GREEN)
	
	DebugDraw3D.draw_position($Misc/Position.global_transform, Color.BROWN)
	
	DebugDraw3D.draw_gizmo($Misc/GizmoTransform.global_transform, DebugDraw3D.empty_color, true)
	DebugDraw3D.draw_gizmo($Misc/GizmoNormal.global_transform.orthonormalized(), DebugDraw3D.empty_color, false)
	DebugDraw3D.draw_gizmo($Misc/GizmoOneColor.global_transform, Color.BROWN, true)
	
	var tg : Transform3D = $Misc/Grids/Grid.global_transform
	var tn : Vector3 = $Misc/Grids/Grid/Subdivision.transform.origin
	DebugDraw3D.draw_grid(tg.origin, tg.basis.x, tg.basis.z, Vector2i(int(tn.x*10), int(tn.z*10)), Color.LIGHT_CORAL, false)
	
	var tn1 = $Misc/Grids/GridCentered/Subdivision.transform.origin
	DebugDraw3D.draw_grid_xf($Misc/Grids/GridCentered.global_transform, Vector2i(tn1.x*10, tn1.z*10))
	
	# 2D
	DebugDraw2D.config.text_default_size = text_groups_default_font_size
	DebugDraw2D.config.text_block_offset = text_groups_offset
	DebugDraw2D.config.text_block_position = text_groups_position
	DebugDraw2D.config.text_padding = text_groups_padding
	
	DebugDraw2D.config.text_custom_font = custom_font
	
	if test_text:
		time_text -= delta
		_text_tests()
	
	# Graphs
	# Enable FPSGraph if not exists
	_create_graph(&"FPS", true, false, DebugDrawGraph.TEXT_CURRENT | DebugDrawGraph.TEXT_AVG | DebugDrawGraph.TEXT_MAX | DebugDrawGraph.TEXT_MIN, &"", DebugDrawGraph.SIDE_BOTTOM, DebugDrawGraph.POSITION_LEFT_TOP if Engine.is_editor_hint() else DebugDrawGraph.POSITION_RIGHT_TOP, Vector2i(200, 80), custom_font)
	if Engine.is_editor_hint():
		if DebugDraw2D.get_graph(&"FPS"):
			DebugDraw2D.get_graph(&"FPS").offset = Vector2i(0, 64)
	
	# Adding more graphs
	if test_graphs and DebugDraw2D.debug_enabled:
		_graph_test()
	else:
		_remove_graphs()
	_upd_graph_params()
	
	# Lag Test
	$LagTest.position = $LagTest/RESET.get_animation("RESET").track_get_key_value(0,0) + Vector3(sin(Time.get_ticks_msec() / 100.0) * 2.5, 0, 0)
	DebugDraw3D.draw_box($LagTest.global_transform.origin, Vector3.ONE * 2.01, DebugDraw3D.empty_color, true)
	
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
	if time_text < 0:
		DebugDraw2D.set_text("Some delayed text", "for 2.5s", -1, Color.BLACK, 2.5) # it's supposed to show text for 2.5 seconds
		time_text += 5
	
	DebugDraw2D.set_text("FPS", "%.2f" % Engine.get_frames_per_second(), 0, Color.GOLD)
	
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
			DebugDraw2D.set_text("Instances", render_stats.instances, 1)
			DebugDraw2D.set_text("Lines", render_stats.lines, 2)
			DebugDraw2D.set_text("Total Visible", render_stats.total_visible, 3)
			DebugDraw2D.set_text("Visible Instances", render_stats.visible_instances, 4)
			DebugDraw2D.set_text("Visible Lines", render_stats.visible_lines, 5)
			
			DebugDraw2D.set_text("---", null, 6)
			
			DebugDraw2D.set_text("Culling time", "%.2f ms" % (render_stats.total_time_culling_usec / 1000.0), 7)
			DebugDraw2D.set_text("Filling instances buffer", "%.2f ms" % (render_stats.time_filling_buffers_instances_usec / 1000.0), 8)
			DebugDraw2D.set_text("Filling lines buffer", "%.2f ms" % (render_stats.time_filling_buffers_lines_usec / 1000.0), 9)
			DebugDraw2D.set_text("Filling time", "%.2f ms" % (render_stats.total_time_filling_buffers_usec / 1000.0), 10)
			DebugDraw2D.set_text("Total time", "%.2f ms" % (render_stats.total_time_spent_usec / 1000.0), 11)
		
		if text_groups_show_stats && text_groups_show_stats_2d:
			DebugDraw2D.set_text("----", null, 19)
		
		var render_stats_2d := DebugDraw2D.get_render_stats()
		if render_stats_2d && text_groups_show_stats_2d:
			DebugDraw2D.set_text("Text groups", render_stats_2d.overlay_text_groups, 20)
			DebugDraw2D.set_text("Text lines", render_stats_2d.overlay_text_lines, 21)
			DebugDraw2D.set_text("Graphs total", render_stats_2d.overlay_graphs_total, 22)
			DebugDraw2D.set_text("Graphs enabled", render_stats_2d.overlay_graphs_enabled, 23)
			
		DebugDraw2D.end_text_group()
	
	if text_groups_show_hints:
		DebugDraw2D.begin_text_group("controls", 1024, Color.WHITE, false)
		DebugDraw2D.set_text("Shift: change render layers", DebugDraw3D.config.geometry_render_layers, 1)
		DebugDraw2D.set_text("Down: freeze render", DebugDraw3D.config.freeze_3d_render, 2)
		DebugDraw2D.set_text("Up: use scene camera", DebugDraw3D.config.force_use_camera_from_scene, 3)
		DebugDraw2D.set_text("1,2,3: toggle debug", "%s, %s 😐, %s 😏" % [DebugDraw3D.debug_enabled, DebugDraw2D.debug_enabled, DebugDrawManager.debug_enabled], 4)
		DebugDraw2D.set_text("Left: toggle frustum culling", DebugDraw3D.config.use_frustum_culling, 5)
		DebugDraw2D.set_text("Right: draw bounds for culling", DebugDraw3D.config.visible_instance_bounds, 6)


func _more_tests():
	# Line hits render
	for ray in $HitTest/RayEmitter.get_children():
		if ray is RayCast3D:
			DebugDraw3D.draw_line_hit(ray.global_transform.origin, ray.to_global(ray.target_position), ray.get_collision_point(), ray.is_colliding(), 0.15)
	
		# Delayed line render
	DebugDraw3D.draw_line($LagTest.global_transform.origin + Vector3.UP, $LagTest.global_transform.origin + Vector3(0,3,sin(Time.get_ticks_msec() / 50.0)), DebugDraw3D.empty_color, 0.5)


func _draw_array_of_boxes():
	# Lots of boxes to check performance.. I guess..
	if time2 <= 0:
		for x in 50:
			for y in 50:
				for z in 3:
					DebugDraw3D.draw_box(Vector3(x, -4-z, y), Vector3.ONE, DebugDraw3D.empty_color, false, 1.25)
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
	# warning-ignore:return_value_discarded
	_create_graph(&"fps13", true, true, DebugDrawGraph.TEXT_ALL, &"", DebugDrawGraph.SIDE_RIGHT)
	if not DebugDraw2D.get_graph(&"fps13"):
		return
	
	DebugDraw2D.get_graph(&"fps13").enabled = false
	
	# If graphs exists, then more tests are done
	DebugDraw2D.get_graph(&"Sin Wave!").data_getter = Callable(self, &"_get_sin_wave_for_graph")
	DebugDraw2D.get_graph(&"Sin Wave!").upside_down =false
	
	DebugDraw2D.get_graph(&"randf").text_suffix = "utf8 ноль zéro"
	#DebugDraw2D.get_graph(&"fps9").line_position = DebugDrawGraph.LINE_TOP
	DebugDraw2D.get_graph(&"fps9").offset = Vector2i(0, 0)
	#DebugDraw2D.get_graph(&"fps11").line_position = DebugDrawGraph.LINE_BOTTOM
	DebugDraw2D.get_graph(&"fps11").offset = Vector2i(16, 0)
	DebugDraw2D.get_graph(&"fps6").offset = Vector2i(0, 32)
	DebugDraw2D.get_graph(&"fps").offset = Vector2i(16, 72)
	
	DebugDraw2D.get_graph(&"fps9").enabled = graph_is_enabled
	if !Engine.is_editor_hint():
		DebugDraw2D.get_graph(&"fps").corner = DebugDrawGraph.POSITION_LEFT_TOP
	
	# Just sending random data to the graph
	DebugDraw2D.graph_update_data(&"randf", randf())


func _upd_graph_params():
	DebugDraw2D.config.graphs_base_offset = graph_offset
	for g in [&"FPS", &"fps5", &"fps8"]:
		var graph := DebugDraw2D.get_graph(g) as DebugDrawFPSGraph
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
	DebugDraw2D.remove_graph(&"randf")
	DebugDraw2D.remove_graph(&"fps")
	DebugDraw2D.remove_graph(&"fps2")
	DebugDraw2D.remove_graph(&"Sin Wave!")
	DebugDraw2D.remove_graph(&"fps5")
	DebugDraw2D.remove_graph(&"fps6")
	DebugDraw2D.remove_graph(&"fps7")
	DebugDraw2D.remove_graph(&"fps8")
	DebugDraw2D.remove_graph(&"fps9")
	DebugDraw2D.remove_graph(&"fps10")
	DebugDraw2D.remove_graph(&"fps11")
	DebugDraw2D.remove_graph(&"fps12")
	DebugDraw2D.remove_graph(&"fps13")


func _create_graph(title, is_fps, show_title, flags, parent := &"", parent_side := DebugDrawGraph.SIDE_BOTTOM, pos = DebugDrawGraph.POSITION_LEFT_BOTTOM, size := Vector2i(256, 60), font = null) -> DebugDrawGraph:
	var graph := DebugDraw2D.get_graph(title)
	if !graph:
		if is_fps:
			graph = DebugDraw2D.create_fps_graph(title)
		else:
			graph = DebugDraw2D.create_graph(title)
		
		if graph:
			graph.size = size
			graph.buffer_size = 50
			graph.corner = pos
			graph.show_title = show_title
			graph.show_text_flags = flags
			graph.custom_font = font
			graph.set_parent(parent, parent_side)
	
	return graph
