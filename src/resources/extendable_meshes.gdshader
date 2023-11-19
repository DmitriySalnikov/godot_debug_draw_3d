shader_type spatial;
render_mode cull_disabled, shadows_disabled, unshaded, world_vertex_coords;

uniform float extend : hint_range(0.0, 1.0, 0.001) = 0.1;

mat3 orthonormalize(mat3 m) {
    vec3 x = normalize(m[0]);
    vec3 y = normalize(m[1] - dot(m[1], x) * x);
    vec3 z = normalize(m[2] - dot(m[2], x) * x - dot(m[2], y) * y);
    return mat3(x, y, z);
}

void vertex() {
	VERTEX = VERTEX + (CUSTOM0.xyz * (extend * 0.5)) * orthonormalize(inverse(mat3(normalize(MODEL_MATRIX[0].xyz), normalize(MODEL_MATRIX[1].xyz), normalize(MODEL_MATRIX[2].xyz))));
}

void fragment() {
	if (FRONT_FACING)
		ALBEDO = COLOR.xyz;
	else
		ALBEDO = COLOR.xyz * 0.85f;
}
