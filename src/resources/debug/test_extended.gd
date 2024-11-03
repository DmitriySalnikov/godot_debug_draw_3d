@tool
extends Node3D

@export_range(0, 100) var instances = 1
@export_range(0, 1) var thickness = 0.1
@export_range(0, 1) var brightness = 0.8

func _ready() -> void:
	await get_tree().process_frame
	
	var f32_to_v3 = func (arr):
		var v3: PackedVector3Array
		for i in range(0, arr.size(), 3):
			v3.push_back(Vector3(arr[i], arr[i+1], arr[i+2]))
		return v3
	
	var m: ArrayMesh = $MMesh.multimesh.mesh
	var arrs: Array = m.surface_get_arrays(0)
	print("Vertexes %d\nIndexes %d\nCustom0 %d\nVertexes %s\nIndexes %s\nCustom0 %s" % [
		arrs[ArrayMesh.ARRAY_VERTEX].size(),
		arrs[ArrayMesh.ARRAY_INDEX].size(),
		arrs[ArrayMesh.ARRAY_CUSTOM0].size(),
		arrs[ArrayMesh.ARRAY_VERTEX],
		arrs[ArrayMesh.ARRAY_INDEX],
		f32_to_v3.call(arrs[ArrayMesh.ARRAY_CUSTOM0])
	])

func _process(delta: float) -> void:
	var mm:MultiMesh = $MMesh.multimesh
	mm.instance_count = instances
	#  Color(p_cfg->thickness, p_cfg->center_brightness, 0, 0)
	for i in mm.instance_count:
		mm.set_instance_transform(i, Transform3D($MMesh.basis, Vector3(i * 2, 0, 0)))
		mm.set_instance_color(i, Color.CORNFLOWER_BLUE)
		mm.set_instance_custom_data(i, Color(thickness, brightness, 0, 0))
