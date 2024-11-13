extends SceneTree


func _init():
	if true:
		var _s = DebugDraw3D.new_scoped_config().set_thickness(0.1)
		DebugDraw3D.draw_box(Vector3.ZERO, Quaternion.IDENTITY, Vector3.ONE, DebugDraw3D.empty_color, true, 1.2)
	DebugDraw3D.draw_sphere(Vector3.ZERO, 0.5, DebugDraw3D.empty_color, 1.5)
	DebugDraw3D.draw_gizmo(Transform3D().scaled_local(Vector3.ONE*0.4), Color.BROWN, true, 2.5)
	DebugDraw2D.set_text("FPS", "0")
	DebugDraw2D.begin_text_group("Info")
	DebugDraw2D.set_text("INFO", "hello", 1, DebugDraw2D.empty_color, 1)
	DebugDraw2D.end_text_group()
	DebugDraw3D.draw_sphere(Vector3.ZERO)
	DebugDraw3D.config.frustum_length_scale = 0.07
	DebugDrawManager.clear_all()
	quit()
