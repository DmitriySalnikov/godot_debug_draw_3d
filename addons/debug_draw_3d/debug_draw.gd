tool
extends Node

const empty_color = Color(0,0,0,0)
var debug_draw_3d: Node = null


### Constants

var BlockPosition_LeftTop
var BlockPosition_RightTop
var BlockPosition_LeftBottom
var BlockPosition_RightBottom
var FPSGraphTextFlags_None
var FPSGraphTextFlags_Current
var FPSGraphTextFlags_Avarage
var FPSGraphTextFlags_Max
var FPSGraphTextFlags_Min
var FPSGraphTextFlags_All


### Init

func _init() -> void:
	var f = File.new()
	if f.file_exists("res://addons/debug_draw_3d/debug_draw_3d.gdns"):
		debug_draw_3d = load("res://addons/debug_draw_3d/debug_draw_3d.gdns").new()
		
		BlockPosition_LeftTop = debug_draw_3d.BlockPosition_LeftTop
		BlockPosition_RightTop = debug_draw_3d.BlockPosition_RightTop
		BlockPosition_LeftBottom = debug_draw_3d.BlockPosition_LeftBottom
		BlockPosition_RightBottom = debug_draw_3d.BlockPosition_RightBottom
		FPSGraphTextFlags_None = debug_draw_3d.FPSGraphTextFlags_None
		FPSGraphTextFlags_Current = debug_draw_3d.FPSGraphTextFlags_Current
		FPSGraphTextFlags_Avarage = debug_draw_3d.FPSGraphTextFlags_Avarage
		FPSGraphTextFlags_Max = debug_draw_3d.FPSGraphTextFlags_Max
		FPSGraphTextFlags_Min = debug_draw_3d.FPSGraphTextFlags_Min
		FPSGraphTextFlags_All = debug_draw_3d.FPSGraphTextFlags_All

func _enter_tree() -> void:
	if !Engine.editor_hint and debug_draw_3d:
		if !debug_draw_3d.get_singleton():
			add_child(debug_draw_3d)

func _exit_tree() -> void:
	if debug_draw_3d:
		debug_draw_3d.queue_free()

func get_singleton() -> Node:
	if debug_draw_3d:
		return debug_draw_3d.get_singleton()
	else:
		return null


### Parameters

## Recall all calls from DebugDraw3D instance to its singleton if needed
var recall_to_singleton: bool setget set_recall_to_singleton, is_recall_to_singleton

## Enable or disable all debug draw.
var debug_enabled: bool setget set_debug_enabled, is_debug_enabled

## Debug for debug...
var freeze_3d_render: bool setget set_freeze_3d_render, is_freeze_3d_render

## Debug for debug...
var draw_instance_bounds: bool setget set_draw_instance_bounds, is_draw_instance_bounds

## Geometry culling based on camera frustum. Change to false to disable it
var use_frustum_culling: bool setget set_use_frustum_culling, is_use_frustum_culling

## Force use camera placed on edited scene. Usable for editor.
var force_use_camera_from_scene: bool setget set_force_use_camera_from_scene, is_force_use_camera_from_scene

## Position of text block
var text_block_position: int setget set_text_block_position, get_text_block_position

## Offset from the corner selected in 'text_block_position'
var text_block_offset: Vector2 setget set_text_block_offset, get_text_block_offset

## Text padding for each line
var text_padding: Vector2 setget set_text_padding, get_text_padding

## How long HUD text lines remain shown after being invoked.
var text_default_duration: float setget set_text_default_duration, get_text_default_duration

## Color of the text drawn as HUD
var text_foreground_color: Color setget set_text_foreground_color, get_text_foreground_color

## Background color of the text drawn as HUD
var text_background_color: Color setget set_text_background_color, get_text_background_color

## Custom Font
var text_custom_font: Font setget set_text_custom_font, get_text_custom_font

## Color of line with hit
var line_hit_color: Color setget set_line_hit_color, get_line_hit_color

## Color of line after hit
var line_after_hit_color: Color setget set_line_after_hit_color, get_line_after_hit_color

## Custom 'Viewport' to use for frustum culling. Usually used in editor.
var custom_viewport: Viewport setget set_custom_viewport, get_custom_viewport

## Custom 'CanvasItem' to draw on it. Set to 'null' to disable.
var custom_canvas: CanvasItem setget set_custom_canvas, get_custom_canvas


### Draw Functions

func get_rendered_primitives_count() -> Dictionary:
	if debug_draw_3d: return debug_draw_3d.get_rendered_primitives_count()
	else: return Dictionary()

## Clear all 3D objects
func clear_3d_objects() -> void:
	if debug_draw_3d: debug_draw_3d.clear_3d_objects()

## Clear all 2D objects
func clear_2d_objects() -> void:
	if debug_draw_3d: debug_draw_3d.clear_2d_objects()

## Clear all debug objects
func clear_all() -> void:
	if debug_draw_3d: debug_draw_3d.clear_all()

## Draw sphere
## position: Position of the sphere center
## radius: Sphere radius
## color: Sphere color
## duration: Duration of existence in seconds
func draw_sphere(position: Vector3, radius: float = 0.5, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_sphere(position, radius, color, duration)

## Draw sphere
## transform: Transform of the sphere
## color: Sphere color
## duration: Duration of existence in seconds
func draw_sphere_xf(transform: Transform, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_sphere_xf(transform, color, duration)

## Draw vertical cylinder
## position: Center position
## radius: Cylinder radius
## height: Cylinder height
## color: Cylinder color
## duration: Duration of existence in seconds
func draw_cylinder(position: Vector3, rotation: Quat, radius: float = 1, height: float = 1, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_cylinder(position, rotation, radius, height, color, duration)

## Draw vertical cylinder
## transform: Cylinder transform
## color: Cylinder color
## duration: Duration of existence in seconds
func draw_cylinder_xf(transform: Transform, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_cylinder_xf(transform, color, duration)

## Draw rotated box
## position: Position of the box
## rotation: Box rotation
## size: Box size
## color: Box color
## duration: Duration of existence in seconds
## isBoxCentered: Use 'position' as center of the box
func draw_box(position: Vector3, rotation: Quat, size: Vector3, color: Color = empty_color, is_box_centered: bool = true, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_box(position, rotation, size, color, is_box_centered, duration)

## Draw rotated box
## transform: Box transform
## color: Box color
## duration: Duration of existence in seconds
## isBoxCentered: Use 'position' as center of the box
func draw_box_xf(transform: Transform, color: Color = empty_color, is_box_centered: bool = true, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_box_xf(transform, color, is_box_centered, duration)

## Draw AABB
## aabb: AABB
## color: Box color
## duration: Duration of existence in seconds
func draw_aabb(aabb: AABB, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_aabb(aabb, color, duration)

## Draw AABB from 'a' to 'b'
## a: Firts corner
## b: Second corner
## color: Box color
## duration: Duration of existence in seconds
func draw_aabb_ab(a: Vector3, b: Vector3, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_aabb_ab(a, b, color, duration)

## Draw line separated by hit point (billboard square) or not separated if 'is_hit' = 'false'
## a: Start point
## b: End point
## is_hit: Is hit
## unitOffsetOfHit: Unit offset on the line where the hit occurs
## duration: Duration of existence in seconds
## hitColor: Color of the hit point and line before hit
## afterHitColor: Color of line after hit position
func draw_line_3d_hit(start: Vector3, end: Vector3, hit: Vector3, is_hit: bool, hit_size: float = 0.25, hit_color: Color = empty_color, after_hit_color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_line_3d_hit(start, end, hit, is_hit, hit_size, hit_color, after_hit_color, duration)

## Draw line separated by hit point (billboard square) or not separated if 'is_hit' = 'false'
## a: Start point
## b: End point
## is_hit: Is hit
## unitOffsetOfHit: Unit offset on the line where the hit occurs
## duration: Duration of existence in seconds
## hitColor: Color of the hit point and line before hit
## afterHitColor: Color of line after hit position
func draw_line_3d_hit_offset(start: Vector3, end: Vector3, is_hit: bool, unit_offset_of_hit: float = 0.5, hit_size: float = 0.25, hit_color: Color = empty_color, after_hit_color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_line_3d_hit_offset(start, end, is_hit, unit_offset_of_hit, hit_size, hit_color, after_hit_color, duration)

## Draw line
## a: Start point
## b: End point
## color: Line color
## duration: Duration of existence in seconds
func draw_line_3d(a: Vector3, b: Vector3, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_line_3d(a, b, color, duration)

## Draw ray
## origin: Origin
## direction: Direction
## length: Length
## color: Ray color
## duration: Duration of existence in seconds
func draw_ray_3d(origin: Vector3, direction: Vector3, length: float, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_ray_3d(origin, direction, length, color, duration)

## Draw a sequence of points connected by lines
## path: Sequence of points
## color: Color
## duration: Duration of existence in seconds
func draw_line_path_3d(path: PoolVector3Array, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_line_path_3d(path, color, duration)

## Draw line with arrow
## a: Start point
## b: End point
## color: Line color
## duration: Duration of existence in seconds
## arrowSize: Size of the arrow
## absoluteSize: Is the 'arrowSize' absolute or relative to the length of the line?
func draw_arrow_line_3d(a: Vector3, b: Vector3, color: Color = empty_color, arrow_size: float = 0.5, absolute_size: bool = true, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_arrow_line_3d(a, b, color, arrow_size, absolute_size, duration)

## Draw ray with arrow
## origin: Origin
## direction: Direction
## length: Length
## color: Ray color
## duration: Duration of existence in seconds
## arrowSize: Size of the arrow
## absoluteSize: Is the 'arrowSize' absolute or relative to the length of the line?
func draw_arrow_ray_3d(origin: Vector3, direction: Vector3, length: float, color: Color = empty_color, arrow_size: float = 0.5, absolute_size: bool = true, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_arrow_ray_3d(origin, direction, length, color, arrow_size, absolute_size, duration)

## Draw a sequence of points connected by lines with arrows
## path: Sequence of points
## color: Color
## duration: Duration of existence in seconds
## arrowSize: Size of the arrow
## absoluteSize: Is the 'arrowSize' absolute or relative to the length of the line?
func draw_arrow_path_3d(path: PoolVector3Array, color: Color = empty_color, arrow_size: float = 0.75, absolute_size: bool = true, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_arrow_path_3d(path, color, arrow_size, absolute_size, duration)

## Draw a square that will always be turned towards the camera
## position: Center position of square
## color: Color
## size: Unit size
## duration: Duration of existence in seconds
func draw_billboard_square(position: Vector3, size: float = 0.2, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_billboard_square(position, size, color, duration)

## Draw camera frustum area
## camera: Camera node
## color: Color
## duration: Duration of existence in seconds
func draw_camera_frustum(camera: Camera, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_camera_frustum(camera, color, duration)

## Draw camera frustum area
## cameraFrustum: Array of frustum planes
## color: Color
## duration: Duration of existence in seconds
func draw_camera_frustum_planes(camera_frustum: Array, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_camera_frustum_planes(camera_frustum, color, duration)

## Draw 3 intersecting lines with the given transformations
## position: Center position
## rotation: Rotation
## scale: Scale
## color: Color
## duration: Duration of existence in seconds
func draw_position_3d(position: Vector3, rotation: Quat, scale: Vector3, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_position_3d(position, rotation, scale, color, duration)

## Draw 3 intersecting lines with the given transformations
## transform: Transform
## color: Color
## duration: Duration of existence in seconds
func draw_position_3d_xf(transform: Transform, color: Color = empty_color, duration: float = 0) -> void:
	if debug_draw_3d: debug_draw_3d.draw_position_3d_xf(transform, color, duration)

## Begin text group
## groupTitle: Group title and ID
## groupPriority: Group priority
## showTitle: Whether to show the title
func begin_text_group(group_title: String, group_priority: int = 0, group_color: Color = empty_color, show_title: bool = true) -> void:
	if debug_draw_3d: debug_draw_3d.begin_text_group(group_title, group_priority, group_color, show_title)

## End text group. Should be called after 'begin_text_group' if you don't need more than one group.
## If you need to create 2+ groups just call again 'begin_text_group' and this function in the end.
## groupTitle: Group title and ID
## groupPriority: Group priority
## showTitle: Whether to show the title
func end_text_group() -> void:
	if debug_draw_3d: debug_draw_3d.end_text_group()

## Add or update text in overlay
## key: Name of field if 'value' exists, otherwise whole line will equal 'key'.
## value: Value of field
## priority: Priority of this line. Lower value is higher position.
## duration: Expiration time
func set_text(key: String, value = "", priority: int = 0, color_of_value: Color = empty_color, duration: float = -1) -> void:
	if debug_draw_3d: debug_draw_3d.set_text(key, value, priority, color_of_value, duration)

## Create new graph with custom data
## title: Title of the graph
func create_graph(title: String) -> Reference:
	if debug_draw_3d: return debug_draw_3d.create_graph(title)
	else: return null

## Create new graph with custom data
## title: Title of the graph
func create_fps_graph(title: String) -> Reference:
	if debug_draw_3d: return debug_draw_3d.create_fps_graph(title)
	else: return null

## Update custom graph data
## title: Title of the graph
## data: New data
func graph_update_data(title: String, data: float) -> void:
	if debug_draw_3d: debug_draw_3d.graph_update_data(title, data)

## Remove graph
## title: Title of the graph
func remove_graph(title: String) -> void:
	if debug_draw_3d: debug_draw_3d.remove_graph(title)

## Remove all graphs
func clear_graphs() -> void:
	if debug_draw_3d: debug_draw_3d.clear_graphs()

## Get config for graph
## title: Title of the graph
func get_graph_config(title: String) -> Reference:
	if debug_draw_3d: return debug_draw_3d.get_graph_config(title)
	else: return null

## Get all graph names
func get_graph_names() -> PoolStringArray:
	if debug_draw_3d: return debug_draw_3d.get_graph_names()
	else: return PoolStringArray()


### Parameters Setget's

func set_recall_to_singleton(val):
	debug_draw_3d.recall_to_singleton = val

func is_recall_to_singleton() -> bool:
	if debug_draw_3d: return debug_draw_3d.recall_to_singleton
	else: return bool()

func set_debug_enabled(val):
	debug_draw_3d.debug_enabled = val

func is_debug_enabled() -> bool:
	if debug_draw_3d: return debug_draw_3d.debug_enabled
	else: return bool()

func set_freeze_3d_render(val):
	debug_draw_3d.freeze_3d_render = val

func is_freeze_3d_render() -> bool:
	if debug_draw_3d: return debug_draw_3d.freeze_3d_render
	else: return bool()

func set_draw_instance_bounds(val):
	debug_draw_3d.draw_instance_bounds = val

func is_draw_instance_bounds() -> bool:
	if debug_draw_3d: return debug_draw_3d.draw_instance_bounds
	else: return bool()

func set_use_frustum_culling(val):
	debug_draw_3d.use_frustum_culling = val

func is_use_frustum_culling() -> bool:
	if debug_draw_3d: return debug_draw_3d.use_frustum_culling
	else: return bool()

func set_force_use_camera_from_scene(val):
	debug_draw_3d.force_use_camera_from_scene = val

func is_force_use_camera_from_scene() -> bool:
	if debug_draw_3d: return debug_draw_3d.force_use_camera_from_scene
	else: return bool()

func set_text_block_position(val):
	debug_draw_3d.text_block_position = val

func get_text_block_position() -> int:
	if debug_draw_3d: return debug_draw_3d.text_block_position
	else: return int()

func set_text_block_offset(val):
	debug_draw_3d.text_block_offset = val

func get_text_block_offset() -> Vector2:
	if debug_draw_3d: return debug_draw_3d.text_block_offset
	else: return Vector2()

func set_text_padding(val):
	debug_draw_3d.text_padding = val

func get_text_padding() -> Vector2:
	if debug_draw_3d: return debug_draw_3d.text_padding
	else: return Vector2()

func set_text_default_duration(val):
	debug_draw_3d.text_default_duration = val

func get_text_default_duration() -> float:
	if debug_draw_3d: return debug_draw_3d.text_default_duration
	else: return float()

func set_text_foreground_color(val):
	debug_draw_3d.text_foreground_color = val

func get_text_foreground_color() -> Color:
	if debug_draw_3d: return debug_draw_3d.text_foreground_color
	else: return Color()

func set_text_background_color(val):
	debug_draw_3d.text_background_color = val

func get_text_background_color() -> Color:
	if debug_draw_3d: return debug_draw_3d.text_background_color
	else: return Color()

func set_text_custom_font(val):
	debug_draw_3d.text_custom_font = val

func get_text_custom_font() -> Font:
	if debug_draw_3d: return debug_draw_3d.text_custom_font
	else: return null

func set_line_hit_color(val):
	debug_draw_3d.line_hit_color = val

func get_line_hit_color() -> Color:
	if debug_draw_3d: return debug_draw_3d.line_hit_color
	else: return Color()

func set_line_after_hit_color(val):
	debug_draw_3d.line_after_hit_color = val

func get_line_after_hit_color() -> Color:
	if debug_draw_3d: return debug_draw_3d.line_after_hit_color
	else: return Color()

func set_custom_viewport(val):
	debug_draw_3d.custom_viewport = val

func get_custom_viewport() -> Viewport:
	if debug_draw_3d: return debug_draw_3d.custom_viewport
	else: return null

func set_custom_canvas(val):
	debug_draw_3d.custom_canvas = val

func get_custom_canvas() -> CanvasItem:
	if debug_draw_3d: return debug_draw_3d.custom_canvas
	else: return null
