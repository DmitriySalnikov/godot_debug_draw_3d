shader_type spatial;
render_mode cull_disabled, shadows_disabled, unshaded, world_vertex_coords;
//render_mode wireframe;

uniform float extend : hint_range(0.0, 1.0, 0.001) = 0.1;

void vertex() {
	VERTEX = VERTEX + CUSTOM0.xyz * extend;
}

void fragment() {
	if (FRONT_FACING)
		ALBEDO = COLOR.xyz;
	else
		ALBEDO = COLOR.xyz * 0.65f;
}
