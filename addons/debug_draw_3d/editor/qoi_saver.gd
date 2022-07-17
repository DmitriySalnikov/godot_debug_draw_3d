tool
extends ResourceFormatSaver
class_name QOISaver

var qoi_utils = preload("qoi_utils.gdns").new()

func get_recognized_extensions(resource: Resource) -> PoolStringArray:
	if resource is Texture:
		return PoolStringArray(["qoi"])
	return PoolStringArray()

func recognize(resource: Resource) -> bool:
	return resource is Texture and resource.get_data() and !resource.get_data().is_empty()

func save(path: String, resource: Resource, _flags: int) -> int:
	return qoi_utils.save_resource(path, resource, _flags)
