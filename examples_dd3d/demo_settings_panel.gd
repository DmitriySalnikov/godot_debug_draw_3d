@tool
extends Control

@export var switch_to_scene = ""
var is_ready := false

func _ready():
	if Engine.is_editor_hint():
		return
	
	if ProjectSettings.has_setting("application/config/no_csharp_support"):
		%SwitchLang.visible = false
	
	%SwitchLang.disabled = true
	
	var test := DebugDraw2D.get_graph(&"FPS")
	if test:
		%FPSEnabled.button_pressed = test.enabled
		%FPSMS.button_pressed = test.frame_time_mode
		%WidthSlider.value = test.size.x
		%BufferSlider.value = test.buffer_size
	
	%ThicknessSlider.value = get_parent().debug_thickness
	%FrustumScaleSlider.value = get_parent().camera_frustum_scale
	%UpdateInPhysics.text = "Update in physics (%d Ticks) *" % ProjectSettings.get_setting("physics/common/physics_ticks_per_second")
	%UpdateInPhysics.button_pressed = get_parent().update_in_physics
	
	%ShowStats.button_pressed = get_parent().text_groups_show_stats
	%DrawBoxes.button_pressed = get_parent().draw_array_of_boxes
	%Draw1MBoxes.button_pressed = get_parent().draw_1m_boxes
	
	if get_tree():
		await get_tree().create_timer(0.2).timeout
	
	%SwitchLang.disabled = false
	is_ready = true


func _on_CheckBox_toggled(button_pressed: bool) -> void:
	if not is_ready: return
	
	var cfg = DebugDraw2D.get_graph(&"FPS")
	if cfg:
		cfg.enabled = button_pressed


func _on_FPSMS_toggled(button_pressed: bool) -> void:
	if not is_ready: return
	
	var cfg = DebugDraw2D.get_graph(&"FPS")
	if cfg:
		cfg.frame_time_mode = button_pressed


func _on_Button_pressed() -> void:
	get_tree().call_deferred("change_scene_to_file", switch_to_scene)


func _on_hide_show_panel_pressed():
	if %SettingsPanel.visible:
		%SettingsPanel.hide()
		%HideShowPanelButton.text = "Show panel"
	else:
		%SettingsPanel.show()
		%HideShowPanelButton.text = "Hide panel"


func _on_width_slider_value_changed(value):
	if not is_ready: return
	
	get_parent().graph_size = Vector2i(int(value), get_parent().graph_size.y)


func _on_buffer_slider_value_changed(value):
	if not is_ready: return
	
	get_parent().graph_buffer_size = int(value)


func _on_thickness_slider_value_changed(value):
	if not is_ready: return
	
	get_parent().debug_thickness = value


func _on_frustum_scale_slider_value_changed(value):
	if not is_ready: return
	
	get_parent().camera_frustum_scale = value


func _on_update_in_physics_toggled(toggled_on):
	get_parent().update_in_physics = toggled_on


func _on_show_stats_toggled(toggled_on):
	get_parent().text_groups_show_stats = toggled_on


func _on_draw_boxes_toggled(toggled_on):
	get_parent().draw_array_of_boxes = toggled_on
	
	DebugDraw3D.clear_all()
	get_parent().timer_cubes = 0


func _on_draw_1m_boxes_toggled(toggled_on):
	get_parent().draw_1m_boxes = toggled_on
	
	if get_parent().draw_array_of_boxes:
		DebugDraw3D.clear_all()
		get_parent().timer_cubes = 0
