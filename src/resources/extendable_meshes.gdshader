//#define NO_DEPTH
//#define FORCED_TRANSPARENT

shader_type spatial;
render_mode cull_disabled, shadows_disabled, unshaded, world_vertex_coords
#if defined(FOG_DISABLED)
, fog_disabled
#endif
#if defined(NO_DEPTH)
, depth_test_disabled;
#else
;
#endif

varying float brightness_of_center;

mat3 orthonormalize(mat3 m) {
    vec3 x = normalize(m[0]);
    vec3 y = normalize(m[1] - dot(m[1], x) * x);
    vec3 z = normalize(m[2] - dot(m[2], x) * x - dot(m[2], y) * y);
    return mat3(x, y, z);
}

void vertex() {
	brightness_of_center = INSTANCE_CUSTOM.y;
	VERTEX = VERTEX + (CUSTOM0.xyz * INSTANCE_CUSTOM.x) * orthonormalize(inverse(mat3(normalize(MODEL_MATRIX[0].xyz), normalize(MODEL_MATRIX[1].xyz), normalize(MODEL_MATRIX[2].xyz))));
}

vec3 toLinearFast(vec3 col) {
	return vec3(col.rgb*col.rgb);
}

void fragment() {
	ALBEDO = mix(COLOR.xyz * brightness_of_center, COLOR.xyz, clamp(length(UV * 1.8 - 0.9), 0, 1));
	if (!OUTPUT_IS_SRGB)
		ALBEDO = toLinearFast(ALBEDO);
	NORMAL = ALBEDO;
	
	#if defined(FORCED_TRANSPARENT)
	ALPHA = ALPHA;
	#endif
}
