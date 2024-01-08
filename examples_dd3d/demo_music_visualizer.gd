@tool
extends VBoxContainer

@export_range(1, 128)  var bars_count := 32
var transform: Transform3D:
	get:
		return %AudioVisualizer.global_transform
@export_exp_easing("inout") var motion_smoothing := 0.025
@export_range(0, 0.5) var bar_thickness := 0.065
@export_range(0, 10) var bars_separation := 0.325
@export_exp_easing("inout") var color_offset_speed := 0.4
@export var colors: Gradient = null

var MusicAnalyzerBus := &"MusicAnalyzer"
var MasterBus := &"Master"
var MAX_HZ := 16000.0
var MIN_HZ := 20.0
var MIN_DB := 60.0
var spectrum: AudioEffectSpectrumAnalyzerInstance = null

var smoothed_energy: Array[float] = []
var color_offset := 0.0

var _on_data_loaded_callback = null

# TODO remove after moving to 4.2
var is_4_2_and_higher = Engine.get_version_info()["major"] >= 4 && Engine.get_version_info()["minor"] >= 2


func _ready():
	var bus = AudioServer.get_bus_index(MusicAnalyzerBus)
	if bus == -1:
		print("'MusicVisualizer' audio bus not found.\nSet 'VisualizerAudioBus.tres' as the default bus to use the audio visualizer.")
	
	spectrum = AudioServer.get_bus_effect_instance(bus, 0)
	%MuteMaster.button_pressed = AudioServer.is_bus_mute(AudioServer.get_bus_index(MasterBus))
	%VolumeSlider.value = db_to_linear(AudioServer.get_bus_volume_db(AudioServer.get_bus_index(MasterBus)))
	
	if OS.has_feature('web'):
		motion_smoothing = motion_smoothing * 1.5
		
		_on_data_loaded_callback = JavaScriptBridge.create_callback(_on_data_loaded)
		# Retrieve the 'gd_callbacks' object
		var gdcallbacks: JavaScriptObject = JavaScriptBridge.get_interface("gd_callbacks")
		# Assign the callbacks
		gdcallbacks.dataLoaded = _on_data_loaded_callback


func _process(_delta):
	if %MusicPlayer.playing:
		draw_spectrum()


func _pressed():
	var open_file = func(filepath: String):
		print("Opening '%s'" % filepath)
		var file = FileAccess.open(filepath, FileAccess.READ)
		var data = file.get_buffer(file.get_length())
		open_stream(filepath.get_extension(), data)
	
	if is_4_2_and_higher and DisplayServer.has_feature(DisplayServer.FEATURE_NATIVE_DIALOG):
		# TODO remove call() and get() after moving to 4.2
		DisplayServer.call("file_dialog_show", "Select audio file", "", "", true, DisplayServer.get("FILE_DIALOG_MODE_OPEN_FILE"), ["*.mp3"], 
			func (status: bool, selected: PackedStringArray, _fileter: int):
				if status and selected.size():
					open_file.call(selected[0])
		)
	elif OS.has_feature('web'):
		JavaScriptBridge.eval("loadData()")
	else:
		var fd := FileDialog.new()
		add_child(fd)
		
		fd.title = "Select audio file"
		fd.access = FileDialog.ACCESS_FILESYSTEM
		fd.file_mode = FileDialog.FILE_MODE_OPEN_FILE
		fd.current_dir = OS.get_system_dir(OS.SYSTEM_DIR_DOWNLOADS)
		fd.add_filter("*.mp3")
		fd.popup_centered_ratio(0.8)
		
		fd.file_selected.connect(func(path: String):
			open_file.call(path)
		)
		
		fd.visibility_changed.connect(func():
			if not fd.visible:
				fd.queue_free()
		)


func _on_data_loaded(data: Array) -> void:
	# Make sure there is something
	if (data.size() == 0):
		return
	
	var file_name: String = data[0]
	print("Opening '%s'" % file_name)
	
	var arr: PackedByteArray = JavaScriptBridge.eval("gd_callbacks.dataLoadedResult;")
	open_stream(file_name.get_extension(), arr)


func open_stream(file_ext: String, data: PackedByteArray):
	var stream: AudioStream = null
	if file_ext == "mp3":
		stream = AudioStreamMP3.new()
		stream.data = data
		
		if not stream.data:
			print("Failed to load MP3!")
			return
	
	if not stream:
		print("Failed to load music!")
		return
	
	%MusicPlayer.stream = stream
	%MusicPlayer.bus = MusicAnalyzerBus
	%MusicPlayer.play()
	
	# Debugging frequencies
	for ih in range(1, bars_count + 1):
		var _hz: float = log_freq(ih / float(bars_count), MIN_HZ, MAX_HZ)
		#print("%.0f hz %.2f" % [_hz, ih / float(bars_count)])


func draw_spectrum():
	var _s1 = DebugDraw3D.scoped_config().set_thickness(bar_thickness).set_center_brightness(0.9)
	var prev_hz = MIN_HZ
	smoothed_energy.resize(bars_count)
	
	var xf := transform
	var y := xf.basis.y
	var h := y.length()
	var x := xf.basis.x
	var z := xf.basis.z
	var origin := xf.origin - (x * bars_count + (x * bars_separation) * (bars_count - 1)) * 0.5
	var sum := 0.0
	
	for ih in range(1, bars_count + 1):
		var i := ih - 1
		var hz: float = log_freq(ih / float(bars_count), MIN_HZ, MAX_HZ)
		var magnitude: float = spectrum.get_magnitude_for_frequency_range(prev_hz, hz, AudioEffectSpectrumAnalyzerInstance.MAGNITUDE_AVERAGE).length()
		var energy: float = clampf((MIN_DB + linear_to_db(magnitude)) / MIN_DB, 0, 1)
		var e: float = lerp(smoothed_energy[i], energy, clampf(get_process_delta_time() / motion_smoothing if motion_smoothing else 1.0, 0, 1))
		smoothed_energy[i] = e
		var height: float = e * h
		sum += e
		
		var s := x * bars_separation
		
		var a := origin + x * i + s * i + (z * 0.5)
		var b := origin + x * (i + 1) + s * i + (z * -0.5) + xf.basis.y.normalized() * clampf(height, 0.001, h)
		var c := Color.HOT_PINK
		if colors:
			c = colors.sample(wrapf(float(ih) / bars_count + color_offset, 0, 1))
			c.s = clamp(c.s - smoothed_energy[i] * 0.3, 0, 1.0)
		
		DebugDraw3D.draw_box_ab(a, b, y, c)
		
		prev_hz = hz
	
	color_offset = wrapf(color_offset + sum / smoothed_energy.size() * clampf(get_process_delta_time() / color_offset_speed if color_offset_speed else 1.0, 0, 1), 0, 1)


func log10(val: float) -> float:
	return log(val) / 2.302585


func log_freq(pos: float, min_hz: float, max_hz: float) -> float:
	return pow(10, log10(min_hz) + (log10(max_hz) - log10(min_hz)) * pos)


func _on_volume_slider_value_changed(value):
	AudioServer.set_bus_volume_db(AudioServer.get_bus_index(MasterBus), linear_to_db(value))


func _on_mute_master_toggled(toggled_on):
	AudioServer.set_bus_mute(AudioServer.get_bus_index(MasterBus), toggled_on)
