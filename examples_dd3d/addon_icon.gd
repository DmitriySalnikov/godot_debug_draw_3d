@tool
extends Node3D

func _process(_delta: float) -> void:
	var _a = DebugDraw3D.new_scoped_config().set_thickness(0.015)
	DebugDraw3D.draw_box_xf($box.global_transform, Color.GREEN)
	DebugDraw3D.draw_gizmo($gizmo.global_transform)
	DebugDraw3D.draw_grid_xf($gizmo/grid.global_transform, Vector2i(2,2), DebugDraw3D.empty_color, false)
	DebugDraw3D.draw_sphere_xf($sphere.global_transform, Color.RED)
	DebugDraw3D.draw_cylinder($cylinder.global_transform, Color.BLUE)
	DebugDraw3D.draw_line_hit_offset($"line/1".global_transform.origin, $"line/2".global_transform.origin, true, 0.3, 0.1)
