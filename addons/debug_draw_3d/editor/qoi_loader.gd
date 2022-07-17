tool
extends ResourceFormatLoader
class_name QOILoader

var qoi_utils = preload("qoi_utils.gdns").new()

func get_recognized_extensions() -> PoolStringArray:
	return PoolStringArray(["qoi", "qoi_import"])

func get_resource_type(path: String) -> String:
	return "ImageTexture" if path.get_extension().to_lower() in get_recognized_extensions() else ""

func handles_type(typename: String) -> bool:
	return typename == "ImageTexture"

func load(path: String, original_path: String):
	return qoi_utils.load_resource(path, original_path)
