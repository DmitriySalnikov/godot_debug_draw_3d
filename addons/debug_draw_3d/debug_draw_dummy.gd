tool
extends Node

####################################################
### THIS FILE IS GENERATED DO NOT EDIT
### This file can be used in the release build
### SEE debug_draw_3d/generate_debug_draw_3d_api.gd
####################################################

const empty_color = Color(0,0,0,0)


### Constants

var BlockPosition_LeftTop = 0
var BlockPosition_RightTop = 0
var BlockPosition_LeftBottom = 0
var BlockPosition_RightBottom = 0
var GraphTextFlags_None = 0
var GraphTextFlags_Current = 0
var GraphTextFlags_Avarage = 0
var GraphTextFlags_Max = 0
var GraphTextFlags_Min = 0
var GraphTextFlags_All = 0

func get_singleton() -> Node:
	return null


### Parameters

var recall_to_singleton: bool
var debug_enabled: bool
var freeze_3d_render: bool
var visible_instance_bounds: bool
var use_frustum_culling: bool
var force_use_camera_from_scene: bool
var graphs_base_offset: Vector2
var geometry_render_layers: int
var text_block_position: int
var text_block_offset: Vector2
var text_padding: Vector2
var text_default_duration: float
var text_foreground_color: Color
var text_background_color: Color
var text_custom_font: Font
var line_hit_color: Color
var line_after_hit_color: Color
var custom_viewport: Viewport
var custom_canvas: CanvasItem

### Draw Functions

func get_render_stats() -> Dictionary:
	return Dictionary()
func clear_3d_objects() -> void:
	pass
func clear_2d_objects() -> void:
	pass
func clear_all() -> void:
	pass
func draw_sphere(position: Vector3, radius: float = 0.5, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_sphere_xf(transform: Transform, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_sphere_hd(position: Vector3, radius: float = 0.5, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_sphere_hd_xf(transform: Transform, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_cylinder(transform: Transform, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_box(position: Vector3, size: Vector3, color: Color = empty_color, is_box_centered: bool = false, duration: float = 0) -> void:
	pass
func draw_box_xf(transform: Transform, color: Color = empty_color, is_box_centered: bool = true, duration: float = 0) -> void:
	pass
func draw_aabb(aabb: AABB, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_aabb_ab(a: Vector3, b: Vector3, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_line_hit(start: Vector3, end: Vector3, hit: Vector3, is_hit: bool, hit_size: float = 0.25, hit_color: Color = empty_color, after_hit_color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_line_hit_offset(start: Vector3, end: Vector3, is_hit: bool, unit_offset_of_hit: float = 0.5, hit_size: float = 0.25, hit_color: Color = empty_color, after_hit_color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_line(a: Vector3, b: Vector3, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_lines(lines: PoolVector3Array, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_ray(origin: Vector3, direction: Vector3, length: float, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_line_path(path: PoolVector3Array, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_arrow(transform: Transform, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_arrow_line(a: Vector3, b: Vector3, color: Color = empty_color, arrow_size: float = 0.5, absolute_size: bool = false, duration: float = 0) -> void:
	pass
func draw_arrow_ray(origin: Vector3, direction: Vector3, length: float, color: Color = empty_color, arrow_size: float = 0.5, absolute_size: bool = false, duration: float = 0) -> void:
	pass
func draw_arrow_path(path: PoolVector3Array, color: Color = empty_color, arrow_size: float = 0.75, absolute_size: bool = true, duration: float = 0) -> void:
	pass
func draw_point_path(path: PoolVector3Array, size: float = 0.25, points_color: Color = empty_color, lines_color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_square(position: Vector3, size: float = 0.2, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_points(points: PoolVector3Array, size: float = 0.25, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_position(transform: Transform, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_gizmo(transform: Transform, color: Color = empty_color, is_centered: bool = false, duration: float = 0) -> void:
	pass
func draw_grid(origin: Vector3, x_size: Vector3, y_size: Vector3, subdivision: Vector2, color: Color = empty_color, is_centered: bool = true, duration: float = 0) -> void:
	pass
func draw_grid_xf(transform: Transform, subdivision: Vector2, color: Color = empty_color, is_centered: bool = true, duration: float = 0) -> void:
	pass
func draw_camera_frustum(camera: Camera, color: Color = empty_color, duration: float = 0) -> void:
	pass
func draw_camera_frustum_planes(camera_frustum: Array, color: Color = empty_color, duration: float = 0) -> void:
	pass
func begin_text_group(group_title: String, group_priority: int = 0, group_color: Color = empty_color, show_title: bool = true) -> void:
	pass
func end_text_group() -> void:
	pass
func set_text(key: String, value = "", priority: int = 0, color_of_value: Color = empty_color, duration: float = -1) -> void:
	pass
func create_graph(title: String) -> GraphParameters:
	return null
func create_fps_graph(title: String) -> GraphParameters:
	return null
func graph_update_data(title: String, data: float) -> void:
	pass
func remove_graph(title: String) -> void:
	pass
func clear_graphs() -> void:
	pass
func get_graph_config(title: String) -> GraphParameters:
	return null
func get_graph_names() -> PoolStringArray:
	return PoolStringArray()


#######################################################################
#######################################################################


class GraphParameters:
	var enabled: bool
	var show_title: bool
	var frametime_mode: bool
	var centered_graph_line: bool
	var show_text_flags: int
	var size: Vector2
	var buffer_size: int
	var offset: Vector2
	var position: int
	var line_color: Color
	var text_color: Color
	var background_color: Color
	var border_color: Color
	var text_suffix: String
	var custom_font: Font