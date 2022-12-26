@tool
extends Node3D

func _process(delta: float) -> void:
	DebugDraw.draw_box_xf($box.global_transform, Color.GREEN)
	DebugDraw.draw_gizmo($gizmo.global_transform)
	DebugDraw.draw_grid_xf($gizmo/grid.global_transform, Vector2i(2,2), DebugDraw.empty_color, false)
	DebugDraw.draw_sphere_xf($sphere.global_transform, Color.RED)
	DebugDraw.draw_cylinder($cylinder.global_transform, Color.BLUE)
	DebugDraw.draw_line_hit_offset($"line/1".global_transform.origin, $"line/2".global_transform.origin, true, 0.3, 0.1)
