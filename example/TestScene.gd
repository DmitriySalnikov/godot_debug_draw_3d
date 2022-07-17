tool
extends Control

const img_dir = "res://testsuite/images/"
export var run_tests = false
export var run_benchmark = true
export(int, 0, 256) var bench_runs = 5
export(int, 1, 256) var frames_to_render = 99
export(int, 1, 8192) var image_width = 1920
export(int, 1, 8192) var image_height = 1080
export var render_test_images : bool = false setget set_render

onready var grid := $GridContainer

func _ready() -> void:
	if not Engine.editor_hint:
		$ViewportContainer.queue_free()
		test_api()
		if run_benchmark:
			yield(get_tree(), "idle_frame")
			start_bench()

func test_api():
	var qoi = load("res://addons/qoi/qoi.gdns").new()
	qoi.print_errors = true
	
	qoi.write("user://example.qoi", load("res://icon.png").get_data())
	var img = qoi.read("user://example.qoi")
	var enc = qoi.encode(img)
	var dec = qoi.decode(enc)
	
	var tex: = ImageTexture.new()
	tex.create_from_image(dec)
	$TextureRect.texture = tex
	
	# More tests
	
	if !run_tests:
		return
	
	if tex.get_data().is_empty():
		printerr("Tests failed. Example is not working...")
		return
	
	#################
	# prepare
	var dir = Directory.new()
	var file = File.new()
	var test_dir = "res://testsuite/images/tests/"
	#var locked_file = "locked.qoi"
	var good_qoi = "good.qoi"
	var broken_qoi = "broken.qoi"
	var good_data = PoolByteArray()
	var good_image : Image = load("res://icon.png").get_data()
	var good_tex : ImageTexture = ImageTexture.new()
	var bad_tex : ImageTexture = ImageTexture.new()
	dir.make_dir_recursive(test_dir)
	file.open(test_dir.plus_file(".gdignore"), File.WRITE)
	file.close()
	
	qoi.write(test_dir.plus_file(good_qoi), good_image)
	file.open(test_dir.plus_file(broken_qoi), File.WRITE)
	file.store_buffer("definitely broken qoi data...".to_utf8())
	file.close()
	
	file.open(test_dir.plus_file(good_qoi), File.READ)
	good_data = file.get_buffer(file.get_len())
	file.close()
	good_tex.create_from_image(good_image)
	
	#################
	### qoi_wrapper
	
	# read/decode
	assert(!qoi.read(test_dir.plus_file("not existing file.qoi")), "Image must be empty when loading not existing file.")
	assert(!qoi.read(test_dir.plus_file(broken_qoi)), "Image must be empty when loading broken qoi file.")
	assert(!qoi.decode(PoolByteArray()), "Image must be empty when loading from empty array.")
	assert(qoi.read(test_dir.plus_file(good_qoi)), "Image must be loaded correctly from correct qoi file.")
	assert(qoi.decode(good_data), "Image must be loaded correctly from correct qoi data.")
	
	# write/encode
	
	assert(qoi.write("not matter", null) == ERR_INVALID_PARAMETER, "Write null Image can't be performed.")
	assert(qoi.write("not matter", Image.new()) == ERR_INVALID_PARAMETER, "Write empty Image can't be performed.")
	
	assert(qoi.encode(null).size() == 0, "Encode null Image can't be performed.")
	assert(qoi.encode(Image.new()).size() == 0, "Encode empty Image can't be performed.")
	
	var unsupported_image : Image = good_image.duplicate()
# warning-ignore:return_value_discarded
	unsupported_image.compress(Image.COMPRESS_S3TC, Image.COMPRESS_SOURCE_GENERIC, 0.5)
	assert(qoi.write(test_dir.plus_file("unsupported format.qoi"), unsupported_image) != OK, "Write can't be performed with usupported Image format.")
	
	assert(qoi.encode(good_image).size() != 0, "Encode correct Image must be performed.")
	
	#################
	### qoi_utils
	var qoi_utils = preload("res://addons/qoi/editor/qoi_utils.gdns").new()
	
	assert(qoi_utils.save_resource("nope", bad_tex, 0) != OK, "Can't save bad texture")
	var other_type_tex := NoiseTexture.new()
	other_type_tex.noise = OpenSimplexNoise.new()
	yield(get_tree(), "idle_frame")
	assert(qoi_utils.save_resource(test_dir.plus_file("saved_tex.qoi"), other_type_tex, 0) == OK, "Correct texture must be saved")
	
	assert(!qoi_utils.load_resource(test_dir.plus_file(broken_qoi), test_dir.plus_file(broken_qoi)).get_data(), "Broken file can't be loaded")
	assert(qoi_utils.load_resource(test_dir.plus_file("saved_tex.qoi"), test_dir.plus_file("saved_tex.qoi")), "Correct file must be loaded")
	
	# can't test everything :(

func start_bench():
	var result = {}
	var dir = img_dir.plus_file("%dx%d_%d" % [image_width, image_height, frames_to_render])
	
	for ext in ["png", "qoi"]:
		result[ext] = []
		
		for i in bench_runs:
			var start_time = OS.get_ticks_usec()
			fill_grid(dir, ext)
			var time = OS.get_ticks_usec() - start_time
			result[ext].append(time)
			print("Run: %d, Ext: %s, %.3f ms" % [i, ext, time / 1000.0])
			
			# show images for 1 frame
			yield(get_tree(), "idle_frame")
			# Clear
			var ch = grid.get_children()
			for c in ch:
				c.queue_free()
			# wait 1 more frame to make sure the resources have been released
			yield(get_tree(), "idle_frame")
	
	# textures can be imported as png or webp or as vram compressed
	var is_vram = false
	var vram_format = ""
	if ProjectSettings.has_setting("importer_defaults/texture"):
		is_vram = ProjectSettings.get_setting("importer_defaults/texture")["compress/mode"] == 2
		if is_vram:
			var _file := File.new()
			if _file.file_exists(dir.plus_file("0.png")):
				var f = load(dir.plus_file("0.png")).get_data().get_format()
				vram_format = f
				
				if f in [Image.FORMAT_DXT1, Image.FORMAT_DXT3, Image.FORMAT_DXT5]:
					vram_format = "s3tc"
				elif f in [Image.FORMAT_BPTC_RGBA, Image.FORMAT_BPTC_RGBF, Image.FORMAT_BPTC_RGBFU]:
					vram_format = "bptc"
				elif OS.has_feature("Android"):
					vram_format = "etc" if OS.get_current_video_driver() == OS.VIDEO_DRIVER_GLES2 else "etc2"
				elif OS.has_feature("iOS"):
					vram_format = "pvrtc"
	
	var is_webp
	var is_lossy
	if ProjectSettings.has_setting("rendering/misc/lossless_compression/force_png"):
		is_lossy = ProjectSettings.has_setting("importer_defaults/texture") && ProjectSettings.get_setting("importer_defaults/texture")["compress/mode"] == 1
		is_webp = !ProjectSettings.get_setting("rendering/misc/lossless_compression/force_png") || is_lossy
	
	if is_vram:
		print("Note 'importer_defaults/texture[compress/mode]' is not equal to 0. PNG was imported as VRAM compressed texture inside .import folder")
	else:
		if is_webp:
			print("Note 'rendering/misc/lossless_compression/force_png' is off or 'importer_defaults/texture[compress/mode]' is lossy. PNG was imported as WebP inside .import folder")
	
	print("Platform: %s" % OS.get_name())
	print("Avg for %d runs, with %dx%d %d frames" % [bench_runs, image_width, image_height, frames_to_render])
	for ext in ["png", "qoi"]:
		var sum = 0
		for t in result[ext]:
			sum += t
		var e = ext if ext != "png" else (vram_format if is_vram else (("webp lossy" if is_lossy else "webp") if is_webp else "png" ))
		print("%s:\t%.3f ms" % [e, (sum / float(result[ext].size())) / 1000.0])

func render_images():
	var dir = Directory.new()
	var fin_dir = img_dir.plus_file("%dx%d_%d" % [image_width, image_height, frames_to_render])
	dir.make_dir_recursive(fin_dir)
	
	$ViewportContainer/Viewport.size = Vector2(image_width, image_height)
	$ViewportContainer/Viewport/AnimationPlayer.stop()
	yield(get_tree(), "idle_frame")
	yield(get_tree(), "idle_frame")
	
	var tex = $ViewportContainer/Viewport.get_texture()
	$ViewportContainer/Viewport/AnimationPlayer.play("Noise")
	
	var qoi = preload("res://addons/qoi/qoi.gdns").new()
	for i in frames_to_render:
		# save png and qoi for tests
		var format = "%d.%s"
		tex.get_data().save_png(fin_dir.plus_file(format % [i, "png"]))
		qoi.write(fin_dir.plus_file(format % [i, "qoi"]), tex.get_data())
		yield(get_tree(), "idle_frame")
	
	$ViewportContainer/Viewport.size = Vector2(128, 128)
	$ViewportContainer/Viewport/AnimationPlayer.stop()

func fill_grid(path, ext):
	var ch = grid.get_children()
	for c in ch:
		c.queue_free()
	
	var total_found = 0
	
	var dir = Directory.new()
	if dir.open(path) == OK:
		dir.list_dir_begin()
		var file_name = dir.get_next()
		while file_name != "":
			if not dir.current_is_dir():
				if file_name.ends_with(ext + ".import" if OS.has_feature("standalone") else ext):
					var tmp = TextureRect.new()
					tmp.texture = load(path.plus_file(file_name.replace(".import", "")))
					tmp.expand = true
					tmp.size_flags_horizontal = SIZE_EXPAND_FILL
					tmp.size_flags_vertical = SIZE_EXPAND_FILL
					grid.add_child(tmp)
					tmp.owner = self
					total_found += 1
			file_name = dir.get_next()
		
		grid.columns = int(sqrt(nearest_po2(total_found)))

func set_render(_val):
	if _val && Engine.editor_hint:
		render_images()
