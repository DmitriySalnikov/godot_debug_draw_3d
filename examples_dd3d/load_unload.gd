extends Node


func _input(event: InputEvent) -> void:
	if event is InputEventKey:
		if event.pressed:
			if event.keycode == KEY_F3:
				var n = Control.new()
				get_tree().root.add_child(n)
				DebugDraw3D.draw_aabb_ab(Vector3.ZERO, Vector3.ONE, Color.RED, 5.0)
				get_tree().create_timer(2).timeout.connect(func(): n.queue_free())
				
			if event.keycode == KEY_F4:
				get_tree().unload_current_scene()
				
			if event.keycode == KEY_F5:
				get_tree().change_scene_to_file("res://examples_dd3d/DebugDrawDemoScene.tscn")
