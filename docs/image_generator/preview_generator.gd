## Please define "editor/movie_writer/base_folder" with output path
## Also specify movie FPS "editor/movie_writer/fps" (default 30)
## It is advisable to override this setting for .movie: "rendering/viewport/transparent_background"
## Override "rendering/anti_aliasing/quality/msaa_2d" for better quality
## Override "display/window/size/transparent" to display transparency
## And you should have a folder with ffmpeg installed added to your environment variables (PATH)

@tool
extends Control

enum PreviewCase {
	LineThickness,
	LineCenterBrightness,
	LineBevel,
	SphereDensity,
	IcoSphere,
	Line,
	Arrow,
	
	DrawSphere,
	DrawSphereXf,
	DrawCylinder,
	DrawCylinderAb,
	DrawBoxXf,
	DrawBoxXfCorner,
	DrawBoxAb,
	DrawBoxAbEdge,
	
	DrawLineHit,
	DrawLines,
	DrawArrowPath,
	DrawPointsPath,
	DrawPointsPathSpheres,
	DrawPoints,
	
	DrawPosition,
	DrawGizmo,
	DrawGizmoCentered,
	DrawGrid,
	DrawFrustum,
}

class CaseData:
	var anim_name: String
	func _init(anim: String = ""):
		anim_name = anim

var case_maps = {
	PreviewCase.LineThickness : CaseData.new(),
	PreviewCase.LineCenterBrightness : CaseData.new(),
	PreviewCase.LineBevel : CaseData.new(),
	PreviewCase.SphereDensity : CaseData.new(),
	PreviewCase.IcoSphere : CaseData.new(),
	PreviewCase.Line : CaseData.new(),
	PreviewCase.Arrow : CaseData.new("DrawMethods360Lines"),
	
	PreviewCase.DrawSphere : CaseData.new("DrawMethods"),
	PreviewCase.DrawSphereXf : CaseData.new("DrawMethods"),
	PreviewCase.DrawCylinder : CaseData.new("DrawMethods"),
	PreviewCase.DrawCylinderAb : CaseData.new("DrawMethods360Wide"),
	PreviewCase.DrawBoxXf : CaseData.new("DrawMethods"),
	PreviewCase.DrawBoxXfCorner : CaseData.new("DrawMethods360"),
	PreviewCase.DrawBoxAb : CaseData.new("DrawMethods360Wide"),
	PreviewCase.DrawBoxAbEdge : CaseData.new("DrawMethods360Wide"),
	
	PreviewCase.DrawLineHit : CaseData.new("DrawMethods360Lines"),
	PreviewCase.DrawLines : CaseData.new(),
	PreviewCase.DrawArrowPath : CaseData.new("DrawLines"),
	PreviewCase.DrawPointsPath : CaseData.new("DrawLines"),
	PreviewCase.DrawPointsPathSpheres : CaseData.new("DrawLines"),
	PreviewCase.DrawPoints : CaseData.new("DrawLines"),
	
	PreviewCase.DrawPosition : CaseData.new("DrawMethodsWide"),
	PreviewCase.DrawGizmo : CaseData.new("DrawMethods360Lines"),
	PreviewCase.DrawGizmoCentered : CaseData.new("DrawMethods360Lines"),
	PreviewCase.DrawGrid : CaseData.new("DrawMethods180"),
	PreviewCase.DrawFrustum : CaseData.new("DrawMethods360Camera"),
}

var changed_by_code := false
@export var preview_case: PreviewCase = PreviewCase.LineThickness:
	set(val):
		preview_case = val
		DirAccess.make_dir_recursive_absolute(movie_temp_path)
		set_meta("movie_file", movie_temp_path.path_join("_.png"))
		
		if is_inside_tree():
			_set_anim()

@export var single_image: bool = false
@export var lossless: bool = false

@export var viewport_size: Vector2i = Vector2i(256, 256):
	set(val):
		viewport_size = val
		changed_by_code = true
		$Control.size = val
		$GridRender.size = val * 2
		changed_by_code = false

@export var anim_value_1: float = 0

var is_exporting := false
@export var export_all: bool = false:
	set(val):
		export_all = false
		if not Engine.is_editor_hint():
			return
		
		if not is_exporting:
			_export_all_images()
		else:
			_stop_export()

var movie_base_path: String = ProjectSettings.get_setting("editor/movie_writer/base_folder")
var movie_temp_path: String = movie_base_path.path_join("temp")
var movie_input_template: String = "_%08d.png"
var movie_output_ext: String = ".webp"
var movie_name: String = "output"
var movie_FPS: int = 30
var movie_compression: int = 4 # 0-6
var movie_quality: int = 90 # 0-100


func _enter_tree():
	var idx = OS.get_cmdline_user_args().find("--preview_case")
	if not Engine.is_editor_hint() and idx >= 0:
		preview_case = int(OS.get_cmdline_user_args()[idx + 1]) as PreviewCase
	
	var files = DirAccess.get_files_at(movie_temp_path)
	for f in files:
		DirAccess.remove_absolute(movie_temp_path.path_join(f))
	DirAccess.make_dir_recursive_absolute(movie_temp_path)
	
	_set_anim()
	movie_FPS = ProjectSettings.get_setting("editor/movie_writer/fps")
	
	if not Engine.is_editor_hint():
		$Control.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
		get_tree().root.size = viewport_size
	else:
		viewport_size = viewport_size


func _set_anim():
	movie_name = PreviewCase.keys()[preview_case]
	
	var override_anim_name = case_maps[preview_case].anim_name
	%AnimationPlayer.play(PreviewCase.keys()[preview_case] if override_anim_name == "" else override_anim_name)
	%AnimationPlayer.seek(0)
	
	if not Engine.is_editor_hint():
		%AnimationPlayer.callback_mode_process = AnimationMixer.ANIMATION_CALLBACK_MODE_PROCESS_MANUAL
	
	if not Engine.is_editor_hint() and OS.has_feature("movie"):
		var anim: Animation = %AnimationPlayer.get_animation(%AnimationPlayer.current_animation)
		anim.loop_mode = Animation.LOOP_NONE


func set_bevel(state: bool):
	if not Engine.is_editor_hint():
		ProjectSettings.set_setting("debug_draw_3d/settings/3d/add_bevel_to_volumetric_geometry", state)
		DebugDraw3D.regenerate_geometry_meshes()


func set_icosphere(state: bool):
	if not Engine.is_editor_hint():
		ProjectSettings.set_setting("debug_draw_3d/settings/3d/use_icosphere", state)
		ProjectSettings.set_setting("debug_draw_3d/settings/3d/use_icosphere_for_hd", state)
		DebugDraw3D.regenerate_geometry_meshes()


func _on_animation_player_animation_finished(_anim_name):
	if OS.has_feature("movie"):
		_finish.call_deferred()


func _end_of_frame(_delta):
	if not OS.has_feature("movie"):
		return
	
	if single_image:
		_finish.call_deferred()


func _finish():
	set_process(false)
	await get_tree().process_frame
	
	_generate_final_image(Engine.get_frames_drawn())
	get_tree().quit()


func _generate_final_image(generated_frames: int):
	if single_image:
		DirAccess.copy_absolute(movie_temp_path.path_join("_00000000.png"), movie_base_path.path_join(movie_name + ".png"))
	else:
		var cmd = [
			"-y", # skip overwrite confirmation
			"-framerate", str(movie_FPS),
			"-i", "\"%s\"" % movie_temp_path.path_join(movie_input_template), # input template
			"-loop", "0", # activate loop
			"-compression_level", str(movie_compression),
			"-qscale" if not lossless else "-lossless", str(movie_quality) if not lossless else str(1),
			"-frames:v", str(generated_frames), # max frames
			"\"%s\"" % (movie_base_path.path_join(movie_name) + movie_output_ext)
		]
		
		print("Executing: ffmpeg ", " ".join(cmd), "\n")
		var res = []
		OS.execute("ffmpeg", cmd, res, true, false)
		print("\n".join(res))


var _is_stoping_export = false
func _stop_export():
	_is_stoping_export = true


func _normalize_path(path: String) -> String:
	return path.replace(" ", "%20")


func _export_all_images():
	_is_stoping_export = false
	is_exporting = true
	
	for i in PreviewCase.values():
		if _is_stoping_export:
			is_exporting = false
			return
		print(PreviewCase.keys()[i])
		
		var args = [
			"--path", _normalize_path(ProjectSettings.globalize_path("res://").simplify_path()),
			"--write-movie", _normalize_path(get_meta("movie_file").simplify_path()),
			"--fixed-fps", str(ProjectSettings.get_setting("editor/movie_writer/fps")),
			_normalize_path(scene_file_path),
			"++",
			"--preview_case", str(i),
		]
		print(" ".join(args))
		OS.execute(OS.get_executable_path(), args)
	
	is_exporting = false
	_is_stoping_export = true


func _process(delta):
	if not Engine.is_editor_hint():
		%AnimationPlayer.advance(delta)
	
	# Default 3D
	var _s_def = DebugDraw3D.new_scope_config().set_thickness(0.05)
	
	# Default 2D
	DebugDraw2D.custom_canvas = $Control/PanelContainer
	DebugDraw2D.config.text_block_position = DebugDraw2DConfig.POSITION_RIGHT_TOP
	DebugDraw2D.config.text_block_offset = Vector2(4,4)
	DebugDraw2D.config.text_background_color = Color.TRANSPARENT
	
	# Lines
	var path := []
	var lines := []
	for c in $OriginLinesPath.get_children():
		path.append(c.global_position)
	for i in range(path.size() - 1):
		lines.append(path[i])
		lines.append(path[i + 1])
	
	# Grid
	if true:
		var _s = DebugDraw3D.new_scope_config().set_thickness(0.025).set_center_brightness(0.7)
		DebugDraw3D.draw_grid_xf(%Grid.global_transform, Vector2i(10,10), Color.LIGHT_GRAY)
	
	var up_vec = $OriginUpVector.global_position - $OriginUpVector/Up.global_position
	
	match preview_case:
		PreviewCase.LineThickness:
			var _s = DebugDraw3D.new_scope_config().set_thickness(anim_value_1)
			DebugDraw3D.draw_line(%OriginLine/A.global_position, %OriginLine/B.global_position, Color.DARK_SLATE_BLUE)
		PreviewCase.LineCenterBrightness:
			var _s = DebugDraw3D.new_scope_config().set_center_brightness(anim_value_1).set_thickness(0.2)
			DebugDraw3D.draw_line(%OriginLine/A.global_position, %OriginLine/B.global_position, Color.DARK_SLATE_BLUE)
		PreviewCase.LineBevel:
			var _s = DebugDraw3D.new_scope_config().set_center_brightness(0.7).set_thickness(0.2)
			DebugDraw3D.draw_line(%OriginLine/A.global_position, %OriginLine/B.global_position, Color.FIREBRICK)
		PreviewCase.SphereDensity:
			var _s = DebugDraw3D.new_scope_config().set_thickness(0.02).set_hd_sphere(anim_value_1 == 1)
			DebugDraw3D.draw_sphere_xf(%OriginInstances.global_transform, Color.INDIAN_RED)
		PreviewCase.IcoSphere:
			var _s = DebugDraw3D.new_scope_config().set_thickness(0.02)
			DebugDraw3D.draw_sphere_xf(%OriginInstances.global_transform, Color.INDIAN_RED)
		
		PreviewCase.Line:
			var _s = DebugDraw3D.new_scope_config().set_center_brightness(0.7).set_thickness(0.2)
			DebugDraw3D.draw_line(%OriginLine/A.global_position, %OriginLine/B.global_position, Color.FIREBRICK)
		PreviewCase.Arrow:
			var _s = DebugDraw3D.new_scope_config().set_center_brightness(0.7).set_thickness(0.05)
			DebugDraw3D.draw_arrow(%OriginInstances/A.global_position, %OriginInstances/B.global_position, Color.FIREBRICK, 0.2)
		
		PreviewCase.DrawSphere:
			var _s = DebugDraw3D.new_scope_config().set_thickness(0.03)
			DebugDraw3D.draw_sphere_xf(%OriginInstances.global_transform.scaled_local(Vector3.ONE * 1.1), Color.INDIAN_RED)
		PreviewCase.DrawSphereXf:
			var _s = DebugDraw3D.new_scope_config().set_thickness(0.03)
			DebugDraw3D.draw_sphere_xf(%OriginInstances.global_transform.scaled_local(Vector3(0.8, 1.1, 0.8)))
		PreviewCase.DrawCylinder:
			DebugDraw3D.draw_cylinder(%OriginInstances.global_transform.scaled_local(Vector3(0.5, 1, 0.5)), Color.LIGHT_SALMON)
		PreviewCase.DrawCylinderAb:
			DebugDraw3D.draw_square(%OriginInstances/A.global_position, 0.1, Color.RED)
			DebugDraw3D.draw_square(%OriginInstances/B.global_position, 0.1, Color.GREEN)
			DebugDraw3D.draw_cylinder_ab(%OriginInstances/A.global_position, %OriginInstances/B.global_position, 0.15)
		PreviewCase.DrawBoxXf:
			DebugDraw3D.draw_box_xf(%OriginInstances.global_transform, Color.GREEN)
		PreviewCase.DrawBoxXfCorner:
			DebugDraw3D.draw_box_xf(%OriginInstances.global_transform.scaled_local(Vector3.ONE * 0.7), Color.GREEN, false)
		PreviewCase.DrawBoxAb:
			DebugDraw3D.draw_square(%OriginInstances/A.global_position, 0.1, Color.RED)
			DebugDraw3D.draw_square(%OriginInstances/B.global_position, 0.1, Color.GREEN)
			DebugDraw3D.draw_box_ab(%OriginInstances/A.global_position, %OriginInstances/B.global_position, up_vec, Color.DODGER_BLUE)
		PreviewCase.DrawBoxAbEdge:
			DebugDraw3D.draw_square(%OriginInstances/A.global_position, 0.1, Color.RED)
			DebugDraw3D.draw_square(%OriginInstances/B.global_position, 0.1, Color.GREEN)
			DebugDraw3D.draw_box_ab(%OriginInstances/A.global_position, %OriginInstances/B.global_position, up_vec, Color.LIGHT_SEA_GREEN, false)
		
		PreviewCase.DrawLineHit:
			DebugDraw3D.draw_line_hit_offset(%OriginInstances/A.global_position, %OriginInstances/B.global_position, true, anim_value_1, 0.1)
		PreviewCase.DrawLines:
			DebugDraw3D.draw_lines(lines, Color.ROYAL_BLUE)
		PreviewCase.DrawArrowPath:
			DebugDraw3D.draw_arrow_path(path, Color.SLATE_BLUE, 0.15)
		PreviewCase.DrawPointsPath:
			DebugDraw3D.draw_point_path(path, DebugDraw3D.POINT_TYPE_SQUARE, 0.15, Color.SLATE_BLUE)
		PreviewCase.DrawPointsPathSpheres:
			var _s = DebugDraw3D.new_scope_config().set_thickness(0.015)
			DebugDraw3D.draw_point_path(path, DebugDraw3D.POINT_TYPE_SPHERE, 0.15, Color.SLATE_BLUE)
		PreviewCase.DrawPoints:
			DebugDraw3D.draw_points(path, DebugDraw3D.POINT_TYPE_SQUARE, 0.15, Color.SLATE_BLUE)
		
		PreviewCase.DrawPosition:
			DebugDraw3D.draw_position(%OriginInstances.global_transform, Color.FUCHSIA)
		PreviewCase.DrawGizmo:
			DebugDraw3D.draw_gizmo(%OriginInstances.global_transform.scaled_local(Vector3.ONE * 0.6), DebugDraw3D.empty_color)
		PreviewCase.DrawGizmoCentered:
			DebugDraw3D.draw_gizmo(%OriginInstances.global_transform.scaled_local(Vector3.ONE * 0.6), DebugDraw3D.empty_color, true)
		PreviewCase.DrawGrid:
			var xf: Transform3D = %OriginInstances.global_transform
			DebugDraw3D.draw_grid(xf.origin, xf.basis.x.rotated(xf.basis.z.normalized(), deg_to_rad(45)), xf.basis.y.rotated(xf.basis.z.normalized(), deg_to_rad(45)), Vector2i(2,2), Color.YELLOW_GREEN, true)
		PreviewCase.DrawFrustum:
			var _s = DebugDraw3D.new_scope_config().set_thickness(0.015)
			DebugDraw3D.draw_camera_frustum($OriginCameraFrustum/Camera3D, Color.CORAL)
	
	
	_end_of_frame(delta)

