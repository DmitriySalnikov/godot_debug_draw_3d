## Please define "editor/movie_writer/base_folder" with output path
## Also specify movie FPS "editor/movie_writer/fps" (default 48)
## It is advisable to override this setting for .movie: "rendering/viewport/transparent_background"
## Override "rendering/anti_aliasing/quality/msaa_2d" for better quality
## Override "display/window/size/transparent" to display transparency
## And you should have a folder with ffmpeg installed added to your environment variables (PATH)

@tool
extends Control

enum PreviewCase {
	Thickness,
	CenterBrightness,
}

class CaseData:
	var cycle_time: float
	func _init(cycle: float):
		cycle_time = cycle

var case_maps = {
	PreviewCase.Thickness : CaseData.new(2),
	PreviewCase.CenterBrightness : CaseData.new(2),
}

var changed_by_code := false
@export var preview_case: PreviewCase = PreviewCase.Thickness:
	set(val):
		preview_case = val
		DirAccess.make_dir_recursive_absolute(movie_temp_path)
		set_meta("movie_file", movie_temp_path.path_join("_.png"))
		
		if is_inside_tree():
			_set_anim()

@export var single_image: bool = false

@export var viewport_size: Vector2i = Vector2i(256, 256):
	set(val):
		viewport_size = val
		changed_by_code = true
		$Control.size = val
		$GridRender.size = val * 2
		changed_by_code = false

var movie_base_path: String = ProjectSettings.get_setting("editor/movie_writer/base_folder")
var movie_temp_path: String = movie_base_path.path_join("temp")
var movie_input_template: String = "_%08d.png"
var movie_output_ext: String = ".webp"
var movie_name: String = "output"
var movie_FPS: int = 48
var movie_quality: float = 0.8
var cycle_time: float = 0
var timer: float = 0


func _enter_tree():
	var files = DirAccess.get_files_at(movie_temp_path)
	for f in files:
		DirAccess.remove_absolute(movie_temp_path.path_join(f))
	DirAccess.make_dir_recursive_absolute(movie_temp_path)
	
	_set_anim()
	movie_FPS = ProjectSettings.get_setting("editor/movie_writer/fps")
	$Control.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	
	if not Engine.is_editor_hint():
		get_tree().root.size = viewport_size
	
	movie_name = PreviewCase.keys()[preview_case]
	cycle_time = case_maps[preview_case].cycle_time
	timer = cycle_time


func _set_anim():
	%AnimationPlayer.play(PreviewCase.keys()[preview_case])
	%AnimationPlayer.seek(0)
	if Engine.is_editor_hint():
		%AnimationPlayer.stop()


func _process(delta):
	# Grid
	if true:
		var _s = DebugDraw3D.new_scoped_config().set_thickness(0.025).set_center_brightness(0.7)
		DebugDraw3D.draw_grid_xf(%Grid.global_transform, Vector2i(10,10), Color.LIGHT_GRAY)
	
	match preview_case:
		PreviewCase.Thickness:
			var _s = DebugDraw3D.new_scoped_config().set_thickness(abs(cycle_time * 0.5 - timer) * 0.25)
			DebugDraw3D.draw_line(%A.global_position, %B.global_position, Color.DARK_SLATE_BLUE)
			_s.set_thickness(0)
		PreviewCase.CenterBrightness:
			var _s = DebugDraw3D.new_scoped_config().set_center_brightness(abs(cycle_time * 0.5 - timer)).set_thickness(0.2)
			DebugDraw3D.draw_line(%A.global_position, %B.global_position, Color.DARK_SLATE_BLUE)
	
	
	_end_of_frame(delta)


func _end_of_frame(delta):
	timer -= delta
	if not OS.has_feature("movie"):
		if timer < 0:
			timer = cycle_time
		return
	
	if single_image:
		_finish.call_deferred()
	else:
		if timer < 0:
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
			"-lossless", str(movie_quality), # quality 0-1
			"-frames:v", str(generated_frames), # max frames
			"\"%s\"" % (movie_base_path.path_join(movie_name) + movie_output_ext)
		]
		
		print("Executing: ffmpeg ", " ".join(cmd), "\n")
		var res = []
		OS.execute("ffmpeg", cmd, res, true, false)
		print("\n".join(res))
