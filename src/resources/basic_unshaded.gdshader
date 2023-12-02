shader_type spatial;
render_mode cull_disabled, shadows_disabled, unshaded, depth_prepass_alpha;

vec3 toLinearFast(vec3 col) {
	return vec3(col.rgb*col.rgb);
}

void fragment() {
	ALBEDO = COLOR.xyz;
	if (!OUTPUT_IS_SRGB)
		ALBEDO = toLinearFast(ALBEDO);
	NORMAL = ALBEDO;
}
