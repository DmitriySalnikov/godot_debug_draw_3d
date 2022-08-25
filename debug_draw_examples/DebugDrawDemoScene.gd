tool
extends Spatial

export var custom_font : Font
export var zylann_example := false
export var show_hints := true
export var test_graphs := false
export var more_test_cases := true
export var draw_array_of_boxes := false

var time := 0.0
var time2 := 0.0
var time3 := 0.0

func _ready() -> void:
	yield(get_tree(), "idle_frame")
	# this check is required for inherited scenes, because an instance of this 
	# script is created first, and then overridden by another
	if !is_inside_tree():
		return

func _input(event: InputEvent) -> void:
	if event is InputEventKey:
		if event.pressed:
			
			# Some property toggles
			if event.scancode == KEY_F1:
				zylann_example = !zylann_example
			if event.scancode == KEY_LEFT:
				DebugDraw.use_frustum_culling = !DebugDraw.use_frustum_culling

func _process(delta: float) -> void:
	# Zylann's example :D
	if zylann_example:
		DebugDraw.clear_graphs()
		var _time = OS.get_ticks_msec() / 1000.0
		var box_pos = Vector3(0, sin(_time * 4), 0)
		var line_begin = Vector3(-1, sin(_time * 4), 0)
		var line_end = Vector3(1, cos(_time * 4), 0)
		
		DebugDraw.draw_box(box_pos, Vector3(1, 2, 1), Color(0, 1, 0))
		DebugDraw.draw_line(line_begin, line_end, Color(1, 1, 0))
		DebugDraw.set_text("Time", _time)
		DebugDraw.set_text("Frames drawn", Engine.get_frames_drawn())
		DebugDraw.set_text("FPS", Engine.get_frames_per_second())
		DebugDraw.set_text("delta", delta)
		return
	
	# Testing the rendering layers by showing the image from the second camera inside the 2D panel
	DebugDraw.geometry_render_layers = 1 if !Input.is_key_pressed(KEY_SHIFT) else 0b10010
	$Panel.visible = Input.is_key_pressed(KEY_SHIFT)
	
	# More property toggles
	DebugDraw.freeze_3d_render = Input.is_key_pressed(KEY_ENTER)
	DebugDraw.force_use_camera_from_scene = Input.is_key_pressed(KEY_UP)
	DebugDraw.debug_enabled = !Input.is_key_pressed(KEY_DOWN)
	DebugDraw.visible_instance_bounds = Input.is_key_pressed(KEY_RIGHT)
	
	# Enable FPSGraph if not exists
	var g = _create_graph("FPS", true, false, DebugDraw.BlockPosition_LeftTop if Engine.editor_hint else DebugDraw.BlockPosition_RightTop, DebugDraw.GraphTextFlags_Current | DebugDraw.GraphTextFlags_Avarage | DebugDraw.GraphTextFlags_Max | DebugDraw.GraphTextFlags_Min, Vector2(200, 80), custom_font)
	if g:
		g.buffer_size = 300
	
	# Adding more graphs
	if test_graphs:
		_graph_test()
	
	# Zones with black borders
	for z in $Zones.get_children():
		DebugDraw.draw_box_xf(z.global_transform, Color.black)
	
	# Spheres
	DebugDraw.draw_sphere_xf($SphereTransform.global_transform, Color.crimson)
	DebugDraw.draw_sphere_hd_xf($SphereHDTransform.global_transform, Color.orangered)
	
	# Delayed spheres
	if time <= 0:
		DebugDraw.draw_sphere($SpherePosition.global_transform.origin, 2, Color.blueviolet, 2)
		DebugDraw.draw_sphere_hd($SpherePosition.global_transform.origin + Vector3.FORWARD * 4, 2, Color.cornflower, 2)
		time = 2
	time -= delta
	
	# Cylinders
	DebugDraw.draw_cylinder($Cylinder1.global_transform, Color.crimson)
	DebugDraw.draw_cylinder(Transform(Basis.IDENTITY, $Cylinder2.global_transform.origin).scaled(Vector3(1,2,1)), Color.red)
	
	# Boxes
	DebugDraw.draw_box_xf($Box1.global_transform, Color.purple)
	DebugDraw.draw_box($Box2.global_transform.origin, Vector3.ONE, Color.rebeccapurple)
	DebugDraw.draw_box_xf(Transform(Basis(Vector3.UP, PI * 0.25).scaled(Vector3.ONE * 2), $Box3.global_transform.origin), Color.rosybrown)
	
	DebugDraw.draw_aabb(AABB($AABB_fixed.global_transform.origin, Vector3(2, 1, 2)), Color.aqua)
	DebugDraw.draw_aabb_ab($AABB.get_child(0).global_transform.origin, $AABB.get_child(1).global_transform.origin, Color.deeppink)
	
	# Lines
	var target = $Lines/Target
	DebugDraw.draw_square(target.global_transform.origin, 0.5, Color.red)
	
	DebugDraw.draw_line($"Lines/1".global_transform.origin, target.global_transform.origin, Color.fuchsia)
	DebugDraw.draw_ray($"Lines/3".global_transform.origin, (target.global_transform.origin - $"Lines/3".global_transform.origin).normalized(), 3, Color.crimson)
	
	if time3 <= 0:
		DebugDraw.draw_line($"Lines/6".global_transform.origin, target.global_transform.origin, Color.fuchsia, 2)
		time3 = 2
	time3 -= delta
	
	# Lines with Arrow
	DebugDraw.draw_arrow_line($"Lines/2".global_transform.origin, target.global_transform.origin, Color.blue, 0.5, true)
	DebugDraw.draw_arrow_ray($"Lines/4".global_transform.origin, (target.global_transform.origin - $"Lines/4".global_transform.origin).normalized(), 8, Color.lavender, 0.5, true)
	
	DebugDraw.draw_line_hit_offset($"Lines/5".global_transform.origin, target.global_transform.origin, true, abs(sin(OS.get_ticks_msec() / 1000.0)), 0.25, Color.aqua)
	
	# Path
	
	## preparing data
	var points: PoolVector3Array = []
	var points_below: PoolVector3Array = []
	var points_below2: PoolVector3Array = []
	var points_below3: PoolVector3Array = []
	var lines_above: PoolVector3Array = []
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
	DebugDraw.draw_line_path(points, Color.beige)
	DebugDraw.draw_arrow_path(points_below, Color.gold, 0.5)
	DebugDraw.draw_points(points_below2, 0.2, Color.darkgreen)
	DebugDraw.draw_point_path(points_below3, 0.25, Color.blue, Color.tomato)
	
	# Misc
	DebugDraw.draw_camera_frustum($Camera, Color.darkorange)
	
	DebugDraw.draw_arrow($Misc/Arrow.global_transform, Color.yellowgreen)
	
	DebugDraw.draw_square($Misc/Billboard.global_transform.origin, 0.5, Color.green)
	
	DebugDraw.draw_position($Misc/Position.global_transform, Color.brown)
	
	DebugDraw.draw_gizmo($Misc/GizmoTransform.global_transform, DebugDraw.empty_color, true)
	DebugDraw.draw_gizmo($Misc/GizmoNormal.global_transform.orthonormalized(), DebugDraw.empty_color, false)
	DebugDraw.draw_gizmo($Misc/GizmoOneColor.global_transform, Color.brown, true)
	
	var tg = $Misc/Grids/Grid.global_transform
	var tn = $Misc/Grids/Grid/Subdivision.transform.origin
	DebugDraw.draw_grid(tg.origin, tg.basis.x, tg.basis.z, Vector2(tn.x*10, tn.z*10), Color.lightcoral, false)
	
	var tn1 = $Misc/Grids/GridCentered/Subdivision.transform.origin
	DebugDraw.draw_grid_xf($Misc/Grids/GridCentered.global_transform, Vector2(tn1.x*10, tn1.z*10))
	
	# Text
	DebugDraw.text_custom_font = custom_font
	DebugDraw.set_text("FPS", "%.2f" % Engine.get_frames_per_second(), 0, Color.gold)
	
	DebugDraw.begin_text_group("-- First Group --", 2, Color.forestgreen)
	DebugDraw.set_text("Simple text")
	DebugDraw.set_text("Text", "Value", 0, Color.aquamarine)
	DebugDraw.set_text("Text out of order", null, -1, Color.silver)
	DebugDraw.begin_text_group("-- Second Group --", 1, Color.beige)
	DebugDraw.set_text("Rendered frames", Engine.get_frames_drawn())
	DebugDraw.end_text_group()
	
	DebugDraw.begin_text_group("-- Stats --", 3, Color.wheat)
	
	var RenderCount = DebugDraw.get_render_stats()
	if RenderCount.size():
		DebugDraw.set_text("Total", RenderCount.total, 0)
		DebugDraw.set_text("Instances", RenderCount.instances, 1)
		DebugDraw.set_text("Wireframes", RenderCount.wireframes, 2)
		DebugDraw.set_text("Total Visible", RenderCount.total_visible, 3)
		DebugDraw.set_text("Visible Instances", RenderCount.visible_instances, 4)
		DebugDraw.set_text("Visible Wireframes", RenderCount.visible_wireframes, 5)
		DebugDraw.end_text_group()
	
	if show_hints:
		DebugDraw.begin_text_group("controls", 1024, Color.white, false)
		DebugDraw.set_text("Shift: change render layers", DebugDraw.geometry_render_layers, 1)
		DebugDraw.set_text("Enter: freeze render", DebugDraw.freeze_3d_render, 2)
		DebugDraw.set_text("Up: use scene camera", DebugDraw.force_use_camera_from_scene, 3)
		DebugDraw.set_text("Down: toggle debug", DebugDraw.debug_enabled, 4)
		DebugDraw.set_text("Left: toggle frustum culling", DebugDraw.use_frustum_culling, 5)
		DebugDraw.set_text("Right: draw bounds for culling", DebugDraw.visible_instance_bounds, 6)
	
	# Lag Test
	$LagTest.translation = $LagTest/RESET.get_animation("RESET").track_get_key_value(0,0).origin + Vector3(sin(OS.get_ticks_msec() / 100.0) * 2.5, 0, 0)
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

func _more_tests():
	# Line hits render
	for ray in $HitTest/RayEmitter.get_children():
		if ray is RayCast:
			DebugDraw.draw_line_hit(ray.global_transform.origin, ray.global_transform.translated(ray.cast_to).origin, ray.get_collision_point(), ray.is_colliding(), 0.15)
	
		# Delayed line render
	DebugDraw.draw_line($LagTest.global_transform.origin + Vector3.UP, $LagTest.global_transform.origin + Vector3(0,3,sin(OS.get_ticks_msec() / 50.0)), DebugDraw.empty_color, 0.5)

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
	_create_graph("fps", true, true, DebugDraw.BlockPosition_RightTop, DebugDraw.GraphTextFlags_Current)
# warning-ignore:return_value_discarded
	_create_graph("fps2", true, false, DebugDraw.BlockPosition_RightTop, DebugDraw.GraphTextFlags_Current)
# warning-ignore:return_value_discarded
	_create_graph("fps3", true, true, DebugDraw.BlockPosition_RightTop, DebugDraw.GraphTextFlags_Current)
	
# warning-ignore:return_value_discarded
	_create_graph("randf", false, true, DebugDraw.BlockPosition_RightBottom, DebugDraw.GraphTextFlags_Avarage, Vector2(256, 60), custom_font)
# warning-ignore:return_value_discarded
	_create_graph("fps5", true, false, DebugDraw.BlockPosition_RightBottom, DebugDraw.GraphTextFlags_All)
# warning-ignore:return_value_discarded
	_create_graph("fps6", true, true, DebugDraw.BlockPosition_RightBottom, DebugDraw.GraphTextFlags_All)
	
# warning-ignore:return_value_discarded
	_create_graph("fps7", true, false, DebugDraw.BlockPosition_LeftTop, DebugDraw.GraphTextFlags_All)
# warning-ignore:return_value_discarded
	_create_graph("fps8", true, true, DebugDraw.BlockPosition_LeftBottom, DebugDraw.GraphTextFlags_All)
# warning-ignore:return_value_discarded
	_create_graph("fps9", true, true, DebugDraw.BlockPosition_LeftBottom, DebugDraw.GraphTextFlags_All)
# warning-ignore:return_value_discarded
	_create_graph("fps10", true, false, DebugDraw.BlockPosition_LeftBottom, DebugDraw.GraphTextFlags_All)
	
	# If graphs exists, then more tests are done
	if DebugDraw.get_graph_config("randf"):
		DebugDraw.get_graph_config("randf").text_suffix = "utf8 ноль zéro"
		DebugDraw.get_graph_config("fps9").centered_graph_line = false
		
		if Engine.editor_hint:
			DebugDraw.get_graph_config("fps5").offset = Vector2(0, -30)
			DebugDraw.get_graph_config("fps8").offset = Vector2(280, -60)
			DebugDraw.get_graph_config("fps9").offset = Vector2(0, -75)
		else:
			DebugDraw.get_graph_config("fps5").offset = Vector2(0, 0)
			DebugDraw.get_graph_config("fps8").offset = Vector2(280, 0)
			DebugDraw.get_graph_config("fps9").offset = Vector2(0, -75)
	
	# Just sending random data to the graph
	DebugDraw.graph_update_data("randf", randf())

func _create_graph(title, is_fps, show_title, pos, flags, size = Vector2(256, 60), font = null) -> DebugDraw.GraphParameters:
	var graph = DebugDraw.get_graph_config(title)
	if !graph:
		if is_fps:
			graph = DebugDraw.create_fps_graph(title)
		else:
			graph = DebugDraw.create_graph(title)
		
		if graph:
			graph.size = size
			graph.buffer_size = 50
			graph.position = pos
			graph.show_title = show_title
			graph.show_text_flags = flags
			graph.custom_font = font
	
	return graph
