tool
extends Node

# Indicates the need for library initialization.
# Can be overridden in the project settings.
var is_need_to_initialize_lib_setting : String = "debug/settings/debug_draw_3d/is_need_to_initialize_lib" 

####################################################
### THIS FILE HAS BEEN GENERATED.
### CHANGES IN THIS FILE WILL BE OVERWRITTEN AFTER THE UPDATE!
### SEE debug_draw_3d/generate_debug_draw_3d_api.gd
####################################################

const empty_color = Color(0,0,0,0)
var _debug_draw_3d: Node = null


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


### Init

func _init() -> void:
	_register_setting(is_need_to_initialize_lib_setting, false)
	_register_setting(is_need_to_initialize_lib_setting + ".debug", true)
	
	var f = File.new()
	if (Engine.editor_hint\
			or bool(ProjectSettings.get_setting(is_need_to_initialize_lib_setting)))\
			and f.file_exists("res://addons/debug_draw_3d/libs/debug_draw_3d.gdns"):
		_debug_draw_3d = load("res://addons/debug_draw_3d/libs/debug_draw_3d.gdns").new()
		
		BlockPosition_LeftTop = _debug_draw_3d.BlockPosition_LeftTop
		BlockPosition_RightTop = _debug_draw_3d.BlockPosition_RightTop
		BlockPosition_LeftBottom = _debug_draw_3d.BlockPosition_LeftBottom
		BlockPosition_RightBottom = _debug_draw_3d.BlockPosition_RightBottom
		GraphTextFlags_None = _debug_draw_3d.GraphTextFlags_None
		GraphTextFlags_Current = _debug_draw_3d.GraphTextFlags_Current
		GraphTextFlags_Avarage = _debug_draw_3d.GraphTextFlags_Avarage
		GraphTextFlags_Max = _debug_draw_3d.GraphTextFlags_Max
		GraphTextFlags_Min = _debug_draw_3d.GraphTextFlags_Min
		GraphTextFlags_All = _debug_draw_3d.GraphTextFlags_All

func _register_setting(name, default_value):
	if !ProjectSettings.has_setting(name):
		ProjectSettings.set_setting(name, default_value)
	
	ProjectSettings.set_initial_value(name, default_value)
	ProjectSettings.add_property_info({"name": name, "type": TYPE_BOOL})

func _enter_tree() -> void:
	if !Engine.editor_hint and _debug_draw_3d:
		if !_debug_draw_3d.get_singleton():
			add_child(_debug_draw_3d)

func _exit_tree() -> void:
	if _debug_draw_3d:
		_debug_draw_3d.queue_free()

func get_singleton() -> Node:
	if _debug_draw_3d:
		return _debug_draw_3d.get_singleton()
	else:
		return null


### Parameters

## Recall all calls from DebugDraw3D instance to its singleton if needed
var recall_to_singleton: bool setget set_recall_to_singleton, is_recall_to_singleton

## Enable or disable all debug draw
var debug_enabled: bool setget set_debug_enabled, is_debug_enabled

## Freezing 3d debugging instances
var freeze_3d_render: bool setget set_freeze_3d_render, is_freeze_3d_render

## Debug for debug...
var visible_instance_bounds: bool setget set_visible_instance_bounds, is_visible_instance_bounds

## Geometry culling based on camera frustum.
## It is not recommended to use with the current implementation.
var use_frustum_culling: bool setget set_use_frustum_culling, is_use_frustum_culling

## Force use camera placed on edited scene.
## Usable for editor.
var force_use_camera_from_scene: bool setget set_force_use_camera_from_scene, is_force_use_camera_from_scene

## Base offset for all graphs
var graphs_base_offset: Vector2 setget set_graphs_base_offset, get_graphs_base_offset

## Layers on which the geometry will be displayed
var geometry_render_layers: int setget set_geometry_render_layers, get_geometry_render_layers

## Position of text block
var text_block_position: int setget set_text_block_position, get_text_block_position

## Offset from the corner selected in 'text_block_position'
var text_block_offset: Vector2 setget set_text_block_offset, get_text_block_offset

## Text padding for each line
var text_padding: Vector2 setget set_text_padding, get_text_padding

## How long text remain shown after being invoked.
var text_default_duration: float setget set_text_default_duration, get_text_default_duration

## Default color of the text
var text_foreground_color: Color setget set_text_foreground_color, get_text_foreground_color

## Background color of the text
var text_background_color: Color setget set_text_background_color, get_text_background_color

## Custom text Font
var text_custom_font: Font setget set_text_custom_font, get_text_custom_font

## Default color of line with hit
var line_hit_color: Color setget set_line_hit_color, get_line_hit_color

## Default color of line after hit
var line_after_hit_color: Color setget set_line_after_hit_color, get_line_after_hit_color

## Custom 'Viewport' to use for frustum culling.
var custom_viewport: Viewport setget set_custom_viewport, get_custom_viewport

## Custom 'CanvasItem' to draw on it. Set to 'null' to disable.
var custom_canvas: CanvasItem setget set_custom_canvas, get_custom_canvas


### Draw Functions

## Returns a dictionary with rendering statistics.
## Some data can be delayed by 1 frame.
func get_render_stats() -> Dictionary:
	if _debug_draw_3d: return _debug_draw_3d.get_render_stats()
	else: return Dictionary()

## Clear all 3D objects
func clear_3d_objects() -> void:
	if _debug_draw_3d: _debug_draw_3d.clear_3d_objects()

## Clear all 2D objects
func clear_2d_objects() -> void:
	if _debug_draw_3d: _debug_draw_3d.clear_2d_objects()

## Clear all debug objects
func clear_all() -> void:
	if _debug_draw_3d: _debug_draw_3d.clear_all()

## Draw sphere
## position: Position of the sphere center
## radius: Sphere radius
## color: Sphere color
## duration: Duration of existence in seconds
func draw_sphere(position: Vector3, radius: float = 0.5, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_sphere(position, radius, color, duration)

## Draw sphere
## transform: Transform of the Sphere
## color: Sphere color
## duration: Duration of existence in seconds
func draw_sphere_xf(transform: Transform, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_sphere_xf(transform, color, duration)

## Draw sphere with higher lines count
## position: Position of the sphere center
## radius: Sphere radius
## color: Sphere color
## duration: Duration of existence in seconds
func draw_sphere_hd(position: Vector3, radius: float = 0.5, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_sphere_hd(position, radius, color, duration)

## Draw sphere with higher lines count
## transform: Transform of the sphere
## color: Sphere color
## duration: Duration of existence in seconds
func draw_sphere_hd_xf(transform: Transform, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_sphere_hd_xf(transform, color, duration)

## Draw vertical cylinder
## transform: Transform of the Cylinder
## color: Cylinder color
## duration: Duration of existence in seconds
func draw_cylinder(transform: Transform, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_cylinder(transform, color, duration)

## Draw box
## position: Position of the Box
## size: Size of the Box
## color: Box color
## is_box_centered: Use 'position' as the center of the box, not as the bottom corner
## duration: Duration of existence in seconds
func draw_box(position: Vector3, size: Vector3, color: Color = empty_color, is_box_centered: bool = false, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_box(position, size, color, is_box_centered, duration)

## Draw box
## transform: Transform of the Box
## color: Box color
## is_box_centered: Use 'transform' as the center of the box, not as the bottom corner
## duration: Duration of existence in seconds
func draw_box_xf(transform: Transform, color: Color = empty_color, is_box_centered: bool = true, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_box_xf(transform, color, is_box_centered, duration)

## Draw AABB
## aabb: AABB
## color: Box color
## duration: Duration of existence in seconds
func draw_aabb(aabb: AABB, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_aabb(aabb, color, duration)

## Draw AABB from 'a' to 'b'
## a: Firts corner
## b: Second corner
## color: Box color
## duration: Duration of existence in seconds
func draw_aabb_ab(a: Vector3, b: Vector3, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_aabb_ab(a, b, color, duration)

## Draw line separated by hit point (billboard square) or not separated if 'is_hit' = 'false'
## start: Start point
## end: End point
## hit: Hit point
## is_hit: Is hit
## hit_size: Size of the hit point
## hit_color: Color of the hit point and line before hit
## after_hit_color: Color of line after hit position
## duration: Duration of existence in seconds
func draw_line_hit(start: Vector3, end: Vector3, hit: Vector3, is_hit: bool, hit_size: float = 0.25, hit_color: Color = empty_color, after_hit_color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_line_hit(start, end, hit, is_hit, hit_size, hit_color, after_hit_color, duration)

## Draw line separated by hit point (billboard square) or not separated if 'is_hit' = 'false'
## start: Start point
## end: End point
## is_hit: Is hit
## unit_offset_of_hit: Unit offset on the line where the hit occurs
## hit_size: Size of the hit point
## hit_color: Color of the hit point and line before hit
## after_hit_color: Color of line after hit position
## duration: Duration of existence in seconds
func draw_line_hit_offset(start: Vector3, end: Vector3, is_hit: bool, unit_offset_of_hit: float = 0.5, hit_size: float = 0.25, hit_color: Color = empty_color, after_hit_color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_line_hit_offset(start, end, is_hit, unit_offset_of_hit, hit_size, hit_color, after_hit_color, duration)

## Draw line
## a: Start point
## b: End point
## color: Line color
## duration: Duration of existence in seconds
func draw_line(a: Vector3, b: Vector3, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_line(a, b, color, duration)

## Draw many lines
## lines: Array of line points. 1 line = 2 Vector3. The size of the array must be even.
## color: Lines color
## duration: Duration of existence in seconds
func draw_lines(lines: PoolVector3Array, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_lines(lines, color, duration)

## Draw ray
## origin: Origin
## direction: Direction
## length: Length
## color: Ray color
## duration: Duration of existence in seconds
func draw_ray(origin: Vector3, direction: Vector3, length: float, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_ray(origin, direction, length, color, duration)

## Draw a sequence of points connected by lines
## path: Sequence of points
## color: Lines Color
## duration: Duration of existence in seconds
func draw_line_path(path: PoolVector3Array, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_line_path(path, color, duration)

## Draw arrow
## Transform: Transform of the Arrow
## color: Arrow color
## duration: Duration of existence in seconds
func draw_arrow(transform: Transform, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_arrow(transform, color, duration)

## Draw line with arrow
## a: Start point
## b: End point
## color: Line color
## arrow_size: Size of the arrow
## absolute_size: Is the 'arrowSize' absolute or relative to the length of the line?
## duration: Duration of existence in seconds
func draw_arrow_line(a: Vector3, b: Vector3, color: Color = empty_color, arrow_size: float = 0.5, absolute_size: bool = false, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_arrow_line(a, b, color, arrow_size, absolute_size, duration)

## Draw ray with arrow
## origin: Origin
## direction: Direction
## length: Length
## color: Ray color
## arrow_size: Size of the arrow
## absolute_size: Is the 'arrowSize' absolute or relative to the length of the line?
## duration: Duration of existence in seconds
func draw_arrow_ray(origin: Vector3, direction: Vector3, length: float, color: Color = empty_color, arrow_size: float = 0.5, absolute_size: bool = false, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_arrow_ray(origin, direction, length, color, arrow_size, absolute_size, duration)

## Draw a sequence of points connected by lines with arrows
## path: Sequence of points
## color: Color
## arrow_size: Size of the arrow
## absolute_size: Is the 'arrowSize' absolute or relative to the length of the line?
## duration: Duration of existence in seconds
func draw_arrow_path(path: PoolVector3Array, color: Color = empty_color, arrow_size: float = 0.75, absolute_size: bool = true, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_arrow_path(path, color, arrow_size, absolute_size, duration)

## Draw a sequence of points connected by lines with billboard squares
## path: Sequence of points
## points_color: Color of points
## lines_color: Color of lines
## size: Size of squares
## duration: Duration of existence in seconds
func draw_point_path(path: PoolVector3Array, size: float = 0.25, points_color: Color = empty_color, lines_color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_point_path(path, size, points_color, lines_color, duration)

## Draw a square that will always be turned towards the camera
## position: Center position of square
## size: Square size
## color: Color
## duration: Duration of existence in seconds
func draw_square(position: Vector3, size: float = 0.2, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_square(position, size, color, duration)

## Draw a sequence of points using billboard squares
## path: Sequence of points
## color: Color
## size: Size of squares
## duration: Duration of existence in seconds
func draw_points(points: PoolVector3Array, size: float = 0.25, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_points(points, size, color, duration)

## Draw 3 intersecting lines with the given transformations
## transform: Transform of lines
## color: Color
## duration: Duration of existence in seconds
func draw_position(transform: Transform, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_position(transform, color, duration)

## Draw 3 lines with the given transformations and arrows at the ends
## transform: Transform of lines
## color: Color
## is_centered: If 'true', then the lines will intersect in the center of the transform
## duration: Duration of existence in seconds
func draw_gizmo(transform: Transform, color: Color = empty_color, is_centered: bool = false, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_gizmo(transform, color, is_centered, duration)

## Draw simple grid with given size and subdivision
## origin: Grid origin
## x_size: Direction and size of the X side. As an axis in the Basis.
## y_size: Direction and size of the Y side. As an axis in the Basis.
## subdivision: Number of cells for the X and Y axes
## color: Lines color
## is_centered: Draw lines relative to origin
## duration: Duration of existence in seconds
func draw_grid(origin: Vector3, x_size: Vector3, y_size: Vector3, subdivision: Vector2, color: Color = empty_color, is_centered: bool = true, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_grid(origin, x_size, y_size, subdivision, color, is_centered, duration)

## Draw simple grid with given size and subdivision
## transform: Transform of the Grid
## subdivision: Number of cells for the X and Y axes
## color: Lines color
## is_centered: Draw lines relative to origin
## duration: Duration of existence in seconds
func draw_grid_xf(transform: Transform, subdivision: Vector2, color: Color = empty_color, is_centered: bool = true, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_grid_xf(transform, subdivision, color, is_centered, duration)

## Draw camera frustum area
## camera: Camera node
## color: Color
## duration: Duration of existence in seconds
func draw_camera_frustum(camera: Camera, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_camera_frustum(camera, color, duration)

## Draw camera frustum area
## cameraFrustum: Array of frustum planes
## color: Color
## duration: Duration of existence in seconds
func draw_camera_frustum_planes(camera_frustum: Array, color: Color = empty_color, duration: float = 0) -> void:
	if _debug_draw_3d: _debug_draw_3d.draw_camera_frustum_planes(camera_frustum, color, duration)

## Begin text group
## group_title: Group title and ID
## group_priority: Group priority
## group_color: Group color
## show_title: Whether to show the title
func begin_text_group(group_title: String, group_priority: int = 0, group_color: Color = empty_color, show_title: bool = true) -> void:
	if _debug_draw_3d: _debug_draw_3d.begin_text_group(group_title, group_priority, group_color, show_title)

## End text group. Should be called after 'begin_text_group' if you don't need more than one group.
## If you need to create 2+ groups just call again 'begin_text_group' and this function in the end.
func end_text_group() -> void:
	if _debug_draw_3d: _debug_draw_3d.end_text_group()

## Add or update text in overlay
## key: The name of the field, if there is a 'value', otherwise the whole string will be equal to the 'key'
## value: Value of field
## priority: Priority of this line. Lower value is higher position.
## color_of_value: Value color
## duration: Expiration time
func set_text(key: String, value = "", priority: int = 0, color_of_value: Color = empty_color, duration: float = -1) -> void:
	if _debug_draw_3d: _debug_draw_3d.set_text(key, value, priority, color_of_value, duration)

## Create new graph with custom data.
## To get more information about the returned class, please see the documentation.
## title: Title of the graph
func create_graph(title: String) -> GraphParameters:
	if _debug_draw_3d: return _debug_draw_3d.create_graph(title)
	else: return null

## Create new graph with custom data.
## To get more information about the returned class, please see the documentation.
## title: Title of the graph
func create_fps_graph(title: String) -> GraphParameters:
	if _debug_draw_3d: return _debug_draw_3d.create_fps_graph(title)
	else: return null

## Update custom graph data
## title: Title of the graph
## data: New data
func graph_update_data(title: String, data: float) -> void:
	if _debug_draw_3d: _debug_draw_3d.graph_update_data(title, data)

## Remove graph
## title: Title of the graph
func remove_graph(title: String) -> void:
	if _debug_draw_3d: _debug_draw_3d.remove_graph(title)

## Remove all graphs
func clear_graphs() -> void:
	if _debug_draw_3d: _debug_draw_3d.clear_graphs()

## Get config for graph.
## To get more information about the returned class, please see the documentation.
## title: Title of the graph
func get_graph_config(title: String) -> GraphParameters:
	if _debug_draw_3d: return _debug_draw_3d.get_graph_config(title)
	else: return null

## Get all graph names
func get_graph_names() -> PoolStringArray:
	if _debug_draw_3d: return _debug_draw_3d.get_graph_names()
	else: return PoolStringArray()


### Parameters Setget's

func set_recall_to_singleton(val):
	if _debug_draw_3d: _debug_draw_3d.recall_to_singleton = val

func is_recall_to_singleton() -> bool:
	if _debug_draw_3d: return _debug_draw_3d.recall_to_singleton
	else: return bool()

func set_debug_enabled(val):
	if _debug_draw_3d: _debug_draw_3d.debug_enabled = val

func is_debug_enabled() -> bool:
	if _debug_draw_3d: return _debug_draw_3d.debug_enabled
	else: return bool()

func set_freeze_3d_render(val):
	if _debug_draw_3d: _debug_draw_3d.freeze_3d_render = val

func is_freeze_3d_render() -> bool:
	if _debug_draw_3d: return _debug_draw_3d.freeze_3d_render
	else: return bool()

func set_visible_instance_bounds(val):
	if _debug_draw_3d: _debug_draw_3d.visible_instance_bounds = val

func is_visible_instance_bounds() -> bool:
	if _debug_draw_3d: return _debug_draw_3d.visible_instance_bounds
	else: return bool()

func set_use_frustum_culling(val):
	if _debug_draw_3d: _debug_draw_3d.use_frustum_culling = val

func is_use_frustum_culling() -> bool:
	if _debug_draw_3d: return _debug_draw_3d.use_frustum_culling
	else: return bool()

func set_force_use_camera_from_scene(val):
	if _debug_draw_3d: _debug_draw_3d.force_use_camera_from_scene = val

func is_force_use_camera_from_scene() -> bool:
	if _debug_draw_3d: return _debug_draw_3d.force_use_camera_from_scene
	else: return bool()

func set_graphs_base_offset(val):
	if _debug_draw_3d: _debug_draw_3d.graphs_base_offset = val

func get_graphs_base_offset() -> Vector2:
	if _debug_draw_3d: return _debug_draw_3d.graphs_base_offset
	else: return Vector2()

func set_geometry_render_layers(val):
	if _debug_draw_3d: _debug_draw_3d.geometry_render_layers = val

func get_geometry_render_layers() -> int:
	if _debug_draw_3d: return _debug_draw_3d.geometry_render_layers
	else: return int()

func set_text_block_position(val):
	if _debug_draw_3d: _debug_draw_3d.text_block_position = val

func get_text_block_position() -> int:
	if _debug_draw_3d: return _debug_draw_3d.text_block_position
	else: return int()

func set_text_block_offset(val):
	if _debug_draw_3d: _debug_draw_3d.text_block_offset = val

func get_text_block_offset() -> Vector2:
	if _debug_draw_3d: return _debug_draw_3d.text_block_offset
	else: return Vector2()

func set_text_padding(val):
	if _debug_draw_3d: _debug_draw_3d.text_padding = val

func get_text_padding() -> Vector2:
	if _debug_draw_3d: return _debug_draw_3d.text_padding
	else: return Vector2()

func set_text_default_duration(val):
	if _debug_draw_3d: _debug_draw_3d.text_default_duration = val

func get_text_default_duration() -> float:
	if _debug_draw_3d: return _debug_draw_3d.text_default_duration
	else: return float()

func set_text_foreground_color(val):
	if _debug_draw_3d: _debug_draw_3d.text_foreground_color = val

func get_text_foreground_color() -> Color:
	if _debug_draw_3d: return _debug_draw_3d.text_foreground_color
	else: return Color()

func set_text_background_color(val):
	if _debug_draw_3d: _debug_draw_3d.text_background_color = val

func get_text_background_color() -> Color:
	if _debug_draw_3d: return _debug_draw_3d.text_background_color
	else: return Color()

func set_text_custom_font(val):
	if _debug_draw_3d: _debug_draw_3d.text_custom_font = val

func get_text_custom_font() -> Font:
	if _debug_draw_3d: return _debug_draw_3d.text_custom_font
	else: return null

func set_line_hit_color(val):
	if _debug_draw_3d: _debug_draw_3d.line_hit_color = val

func get_line_hit_color() -> Color:
	if _debug_draw_3d: return _debug_draw_3d.line_hit_color
	else: return Color()

func set_line_after_hit_color(val):
	if _debug_draw_3d: _debug_draw_3d.line_after_hit_color = val

func get_line_after_hit_color() -> Color:
	if _debug_draw_3d: return _debug_draw_3d.line_after_hit_color
	else: return Color()

func set_custom_viewport(val):
	if _debug_draw_3d: _debug_draw_3d.custom_viewport = val

func get_custom_viewport() -> Viewport:
	if _debug_draw_3d: return _debug_draw_3d.custom_viewport
	else: return null

func set_custom_canvas(val):
	if _debug_draw_3d: _debug_draw_3d.custom_canvas = val

func get_custom_canvas() -> CanvasItem:
	if _debug_draw_3d: return _debug_draw_3d.custom_canvas
	else: return null



#######################################################################
#######################################################################


class GraphParameters:
	var orig_ref : Reference
	func _init(ref : Reference) -> void:
		orig_ref = ref
	
	## Is Graph enabled
	var enabled: bool setget set_enabled, is_enabled
	
	## Draw Graph title
	var show_title: bool setget set_show_title, is_show_title
	
	## Switch between frame time and FPS modes
	## Only for FPS Graphs
	var frametime_mode: bool setget set_frametime_mode, is_frametime_mode
	
	## Draw a graph line aligned vertically in the center
	var centered_graph_line: bool setget set_centered_graph_line, is_centered_graph_line
	
	## Sets the text visibility *GraphTextFlags*
	var show_text_flags: int setget set_show_text_flags, get_show_text_flags
	
	## The size of the graph.
	var size: Vector2 setget set_size, get_size
	
	## The size of the buffer where the values are stored.
	var buffer_size: int setget set_buffer_size, get_buffer_size
	
	## Offset from the corner selected in position
	var offset: Vector2 setget set_offset, get_offset
	
	## FPS Graph position *BlockPosition*
	var position: int setget set_position, get_position
	
	## Graph line color
	var line_color: Color setget set_line_color, get_line_color
	
	## Color of the info text
	var text_color: Color setget set_text_color, get_text_color
	
	## Background color
	var background_color: Color setget set_background_color, get_background_color
	
	## Border color
	var border_color: Color setget set_border_color, get_border_color
	
	## Border color
	var text_suffix: String setget set_text_suffix, get_text_suffix
	
	## Custom Font
	var custom_font: Font setget set_custom_font, get_custom_font
	
	
	func set_enabled(val):
		if orig_ref: orig_ref.enabled = val
	
	func is_enabled() -> bool:
		if orig_ref: return orig_ref.enabled
		else: return bool()
	
	func set_show_title(val):
		if orig_ref: orig_ref.show_title = val
	
	func is_show_title() -> bool:
		if orig_ref: return orig_ref.show_title
		else: return bool()
	
	func set_frametime_mode(val):
		if orig_ref: orig_ref.frametime_mode = val
	
	func is_frametime_mode() -> bool:
		if orig_ref: return orig_ref.frametime_mode
		else: return bool()
	
	func set_centered_graph_line(val):
		if orig_ref: orig_ref.centered_graph_line = val
	
	func is_centered_graph_line() -> bool:
		if orig_ref: return orig_ref.centered_graph_line
		else: return bool()
	
	func set_show_text_flags(val):
		if orig_ref: orig_ref.show_text_flags = val
	
	func get_show_text_flags() -> int:
		if orig_ref: return orig_ref.show_text_flags
		else: return int()
	
	func set_size(val):
		if orig_ref: orig_ref.size = val
	
	func get_size() -> Vector2:
		if orig_ref: return orig_ref.size
		else: return Vector2()
	
	func set_buffer_size(val):
		if orig_ref: orig_ref.buffer_size = val
	
	func get_buffer_size() -> int:
		if orig_ref: return orig_ref.buffer_size
		else: return int()
	
	func set_offset(val):
		if orig_ref: orig_ref.offset = val
	
	func get_offset() -> Vector2:
		if orig_ref: return orig_ref.offset
		else: return Vector2()
	
	func set_position(val):
		if orig_ref: orig_ref.position = val
	
	func get_position() -> int:
		if orig_ref: return orig_ref.position
		else: return int()
	
	func set_line_color(val):
		if orig_ref: orig_ref.line_color = val
	
	func get_line_color() -> Color:
		if orig_ref: return orig_ref.line_color
		else: return Color()
	
	func set_text_color(val):
		if orig_ref: orig_ref.text_color = val
	
	func get_text_color() -> Color:
		if orig_ref: return orig_ref.text_color
		else: return Color()
	
	func set_background_color(val):
		if orig_ref: orig_ref.background_color = val
	
	func get_background_color() -> Color:
		if orig_ref: return orig_ref.background_color
		else: return Color()
	
	func set_border_color(val):
		if orig_ref: orig_ref.border_color = val
	
	func get_border_color() -> Color:
		if orig_ref: return orig_ref.border_color
		else: return Color()
	
	func set_text_suffix(val):
		if orig_ref: orig_ref.text_suffix = val
	
	func get_text_suffix() -> String:
		if orig_ref: return orig_ref.text_suffix
		else: return ""
	
	func set_custom_font(val):
		if orig_ref: orig_ref.custom_font = val
	
	func get_custom_font() -> Font:
		if orig_ref: return orig_ref.custom_font
		else: return null
	