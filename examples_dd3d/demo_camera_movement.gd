extends Camera3D

@export var mouse_sensitivity := 0.25
@export var camera_speed := 10.0
@export var camera_speed_fast := 30.0

var btn_clicked := false
const hPI := PI/2
var rot_x := 0.0
var rot_y := 0.0


func _ready():
	rot_x = rotation.y
	rot_y = rotation.x


func _unhandled_input(event) -> void:
	if event is InputEventMouseButton:
		btn_clicked = event.pressed


func _input(event) -> void:
	if btn_clicked:
		if event is InputEventMouseMotion:
			if event.button_mask == MOUSE_BUTTON_LEFT:
				rot_x += -deg_to_rad(event.relative.x * mouse_sensitivity)
				rot_y += -deg_to_rad(event.relative.y * mouse_sensitivity)
				rot_y = clamp(rot_y, -hPI, hPI)
				
				transform.basis = Basis()
				rotate_object_local(Vector3.UP, rot_x)
				rotate_object_local(Vector3.RIGHT, rot_y)


func get_axis(neg : Array[Key], pos : Array[Key]) -> float:
	var pressed = func (arr: Array[Key]):
		var p: float = 0
		for k in arr:
			if Input.is_physical_key_pressed(k):
				p = 1
				break
		return p
	
	return pressed.call(pos) - pressed.call(neg)


func _process(delta) -> void:
	var motion := Vector2(get_axis([KEY_S], [KEY_W]), get_axis([KEY_A], [KEY_D]))
	var lift := get_axis([KEY_Q, KEY_CTRL], [KEY_E, KEY_SPACE])
	var speed := camera_speed_fast if Input.is_physical_key_pressed(KEY_SHIFT) else camera_speed
	motion = motion.limit_length()
	
	var b := global_transform.basis
	var v := (-b.z * motion.x) + (b.x * motion.y) + (b.y * lift)
	global_position += v.limit_length() * speed * delta
