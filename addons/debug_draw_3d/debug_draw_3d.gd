tool
extends Node

const empty_color = Color(0,0,0,0)
var debug_draw_3d : Node = preload("res://addons/debug_draw_3d/debug_draw_3d.gdns").new()

# Constants

var FPSGraphTextFlags_All = debug_draw_3d.FPSGraphTextFlags_All
var FPSGraphTextFlags_Avarage = debug_draw_3d.FPSGraphTextFlags_Avarage
var FPSGraphTextFlags_Current = debug_draw_3d.FPSGraphTextFlags_Current
var FPSGraphTextFlags_Max = debug_draw_3d.FPSGraphTextFlags_Max
var FPSGraphTextFlags_Min = debug_draw_3d.FPSGraphTextFlags_Min
var FPSGraphTextFlags_None = debug_draw_3d.FPSGraphTextFlags_None

var BlockPosition_LeftBottom = debug_draw_3d.BlockPosition_LeftBottom
var BlockPosition_LeftTop = debug_draw_3d.BlockPosition_LeftTop
var BlockPosition_RightBottom = debug_draw_3d.BlockPosition_RightBottom
var BlockPosition_RightTop = debug_draw_3d.BlockPosition_RightTop

# General

## Enable or disable all debug draw.
var debug_enabled : bool setget _set_debug_enabled, _is_debug_enabled
## Debug for debug...
var freeze_3d_render : bool setget _set_freeze_3d_render, _is_freeze_3d_render
## Geometry culling based on camera frustum. Change to false to disable it
var use_frustum_culling : bool setget _set_use_frustum_culling, _is_use_frustum_culling
## Force use camera placed on edited scene. Usable for editor.
var force_use_camera_from_scene : bool setget _set_force_use_camera_from_scene, _is_force_use_camera_from_scene

# Text

## Position of text block. Uses BlockPosition_* constants.
var text_block_position : int setget _set_text_block_position, _get_text_block_position
## Offset from the corner selected in text_block_position
var text_block_offset : Vector2 setget _set_text_block_offset, _get_text_block_offset
## Text padding for each line
var text_padding : Vector2 setget _set_text_padding, _get_text_padding
## How long HUD text lines remain shown after being invoked.
var text_default_duration = 500 setget _set_text_default_duration, _get_text_default_duration
## Color of the text drawn as HUD
var text_foreground_color : Color setget _set_text_foreground_color, _get_text_foreground_color
## Background color of the text drawn as HUD
var text_background_color : Color setget _set_text_background_color, _get_text_background_color
## Custom font of the text drawn as HUD
var text_custom_font : Font setget _set_text_custom_font, _get_text_custom_font

# Geometry

## Color of line with hit
var line_hit_color : Color setget _set_line_hit_color, _get_line_hit_color
## Color of line after hit
var line_after_hit_color : Color setget _set_line_after_hit_color, _get_line_after_hit_color

# Misc

## Custom Viewport to use for frustum culling. Usually used in editor.
var custom_viewport : Viewport setget _set_custom_viewport, _get_custom_viewport
## Custom CanvasItem to draw on it. Set to null to disable.
var custom_canvas : CanvasItem setget _set_custom_canvas, _get_custom_canvas

func _enter_tree() -> void:
	if !Engine.editor_hint:
		add_child(debug_draw_3d)
		
		print(get_singleton() == debug_draw_3d)

func _exit_tree() -> void:
	debug_draw_3d.queue_free()

func get_singleton() -> Node:
	return debug_draw_3d.get_singleton()

func get_rendered_primitives_count() -> Dictionary:
	return debug_draw_3d.get_rendered_primitives_count()


###################################
## Draw Functions

## Begin text group
## groupTitle: Group title and ID
## groupPriority: Group priority
## showTitle: Whether to show the title
func begin_text_group(groupTitle : String, groupPriority : int = 0, groupColor : Color = empty_color, showTitle : bool = true):
	debug_draw_3d.begin_text_group(groupTitle, groupPriority, groupColor, showTitle)

## End text group. Should be called after "BeginTextGroup" if you don't need more than one group.
## If you need to create 2+ groups just call again "BeginTextGroup" and this function in the end.
## groupTitle: Group title and ID
## groupPriority: Group priority
## showTitle: Whether to show the title
func end_text_group():
	debug_draw_3d.end_text_group()

## Add or update text in overlay
## key: Name of field if "value" exists, otherwise whole line will equal "key".
## value: Value of field
## priority: Priority of this line. Lower value is higher position.
## duration: Expiration time
func set_text(key : String, value = "", priority : int = 0, colorOfValue : Color = empty_color, duration : float = -1):
	debug_draw_3d.set_text(key, value, priority, colorOfValue, duration)

func create_graph(title : String) -> Resource:
	return debug_draw_3d.create_graph(title)

func create_fps_graph(title : String) -> Resource:
	return debug_draw_3d.create_fps_graph(title)

func graph_update_data(title : String, data : float):
	debug_draw_3d.graph_update_data(title, data)

func remove_graph(title : String):
	debug_draw_3d.remove_graph(title)

func clear_graphs():
	debug_draw_3d.clear_graphs()

func get_graph_config(title : String) -> Resource:
	return debug_draw_3d.get_graph_config(title)

func get_graph_names() -> PoolStringArray:
	return debug_draw_3d.get_graph_names()

###################################
## Parameters

func _set_debug_enabled(val : bool):
	debug_draw_3d.get_singleton().debug_enabled = val

func _is_debug_enabled() -> bool:
	return debug_draw_3d.get_singleton().debug_enabled

func _set_freeze_3d_render(val):
	debug_draw_3d.get_singleton().freeze_3d_render = val

func _is_freeze_3d_render() -> bool:
	return debug_draw_3d.get_singleton().freeze_3d_render

func _set_use_frustum_culling(val):
	debug_draw_3d.get_singleton().use_frustum_culling = val

func _is_use_frustum_culling() -> bool:
	return debug_draw_3d.get_singleton().use_frustum_culling

func _set_force_use_camera_from_scene(val):
	debug_draw_3d.get_singleton().force_use_camera_from_scene = val

func _is_force_use_camera_from_scene() -> bool:
	return debug_draw_3d.get_singleton().force_use_camera_from_scene

func _set_text_block_position(val):
	debug_draw_3d.get_singleton().text_block_position = val

func _get_text_block_position() -> int:
	return debug_draw_3d.get_singleton().text_block_position

func _set_text_block_offset(val):
	debug_draw_3d.get_singleton().text_block_offset = val

func _get_text_block_offset() -> Vector2:
	return debug_draw_3d.get_singleton().text_block_offset

func _set_text_padding(val):
	debug_draw_3d.get_singleton().text_padding = val

func _get_text_padding() -> Vector2:
	return debug_draw_3d.get_singleton().text_padding

func _set_text_default_duration(val):
	debug_draw_3d.get_singleton().text_default_duration = val

func _get_text_default_duration() -> int:
	return debug_draw_3d.get_singleton().text_default_duration

func _set_text_foreground_color(val):
	debug_draw_3d.get_singleton().text_foreground_color = val

func _get_text_foreground_color() -> Color:
	return debug_draw_3d.get_singleton().text_foreground_color

func _set_text_background_color(val):
	debug_draw_3d.get_singleton().text_background_color = val

func _get_text_background_color() -> Color:
	return debug_draw_3d.get_singleton().text_background_color

func _set_text_custom_font(val):
	debug_draw_3d.get_singleton().text_custom_font = val

func _get_text_custom_font() -> Font:
	return debug_draw_3d.get_singleton().text_custom_font

func _set_line_hit_color(val):
	debug_draw_3d.get_singleton().line_hit_color = val

func _get_line_hit_color() -> Color:
	return debug_draw_3d.get_singleton().line_hit_color

func _set_line_after_hit_color(val):
	debug_draw_3d.get_singleton().line_after_hit_color = val

func _get_line_after_hit_color() -> Color:
	return debug_draw_3d.get_singleton().line_after_hit_color

func _set_custom_viewport(val):
	debug_draw_3d.get_singleton().custom_viewport = val

func _get_custom_viewport() -> Viewport:
	return debug_draw_3d.get_singleton().custom_viewport

func _set_custom_canvas(val):
	debug_draw_3d.get_singleton().custom_canvas = val

func _get_custom_canvas() -> CanvasItem:
	return debug_draw_3d.get_singleton().custom_canvas
