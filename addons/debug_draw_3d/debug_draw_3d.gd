extends Node

var debug_draw_3d = preload("res://addons/debug_draw_3d/debug_draw_3d.gdns").new()

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

# FPS Graph

## Is FPSGraph enabled
var fps_graph_enabled : bool setget _set_fps_graph_enabled, _is_fps_graph_enabled
## Switch between frame time and FPS modes
var fps_graph_frame_time_mode : bool setget _set_fps_graph_frame_time_mode, _is_fps_graph_frame_time_mode
## Draw a graph line aligned vertically in the center
var fps_graph_centered_graph_line : bool setget _set_fps_graph_centered_graph_line, _is_fps_graph_centered_graph_line
## Sets the text visibility. Uses FPSGraphTextFlags* constants.
var fps_graph_show_text_flags : int setget _set_fps_graph_show_text_flags, _get_fps_graph_show_text_flags
## Size of the FPS Graph. The width is equal to the number of stored frames.
var fps_graph_size : Vector2 setget _set_fps_graph_size, _get_fps_graph_size
## Offset from the corner selected in fps_graph_position
var fps_graph_offset : Vector2 setget _set_fps_graph_offset, _get_fps_graph_offset
## FPS Graph position. Uses BlockPosition_* constants.
var fps_graph_position : int setget _set_fps_graph_position, _get_fps_graph_position
## Graph line color
var fps_graph_line_color : Color setget _set_fps_graph_line_color, _get_fps_graph_line_color
## Color of the info text
var fps_graph_text_color : Color setget _set_fps_graph_text_color, _get_fps_graph_text_color
## Background color
var fps_graph_background_color : Color setget _set_fps_graph_background_color, _get_fps_graph_background_color
## Border color
var fps_graph_border_color : Color setget _set_fps_graph_border_color, _get_fps_graph_border_color

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

func get_singleton():
	return debug_draw_3d.get_singleton()


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

func _set_fps_graph_enabled(val):
	debug_draw_3d.get_singleton().fps_graph_enabled = val

func _is_fps_graph_enabled() -> bool:
	return debug_draw_3d.get_singleton().fps_graph_enabled

func _set_fps_graph_frame_time_mode(val):
	debug_draw_3d.get_singleton().fps_graph_frame_time_mode = val

func _is_fps_graph_frame_time_mode() -> bool:
	return debug_draw_3d.get_singleton().fps_graph_frame_time_mode

func _set_fps_graph_centered_graph_line(val):
	debug_draw_3d.get_singleton().fps_graph_centered_graph_line = val

func _is_fps_graph_centered_graph_line() -> bool:
	return debug_draw_3d.get_singleton().fps_graph_centered_graph_line

func _set_fps_graph_show_text_flags(val):
	debug_draw_3d.get_singleton().fps_graph_show_text_flags = val

func _get_fps_graph_show_text_flags() -> int:
	return debug_draw_3d.get_singleton().fps_graph_show_text_flags

func _set_fps_graph_size(val):
	debug_draw_3d.get_singleton().fps_graph_size = val

func _get_fps_graph_size() -> Vector2:
	return debug_draw_3d.get_singleton().fps_graph_size

func _set_fps_graph_offset(val):
	debug_draw_3d.get_singleton().fps_graph_offset = val

func _get_fps_graph_offset() -> Vector2:
	return debug_draw_3d.get_singleton().fps_graph_offset

func _set_fps_graph_position(val):
	debug_draw_3d.get_singleton().fps_graph_position = val

func _get_fps_graph_position() -> int:
	return debug_draw_3d.get_singleton().fps_graph_position

func _set_fps_graph_line_color(val):
	debug_draw_3d.get_singleton().fps_graph_line_color = val

func _get_fps_graph_line_color() -> Color:
	return debug_draw_3d.get_singleton().fps_graph_line_color

func _set_fps_graph_text_color(val):
	debug_draw_3d.get_singleton().fps_graph_text_color = val

func _get_fps_graph_text_color() -> Color:
	return debug_draw_3d.get_singleton().fps_graph_text_color

func _set_fps_graph_background_color(val):
	debug_draw_3d.get_singleton().fps_graph_background_color = val

func _get_fps_graph_background_color() -> Color:
	return debug_draw_3d.get_singleton().fps_graph_background_color

func _set_fps_graph_border_color(val):
	debug_draw_3d.get_singleton().fps_graph_border_color = val

func _get_fps_graph_border_color() -> Color:
	return debug_draw_3d.get_singleton().fps_graph_border_color

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
