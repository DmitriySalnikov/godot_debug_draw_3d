//#define NO_DEPTH
//#define FORCED_TRANSPARENT

shader_type spatial;
render_mode cull_disabled, shadows_disabled, unshaded
#if defined(FOG_DISABLED)
, fog_disabled
#endif
#if defined(NO_DEPTH)
, depth_test_disabled;
#else
;
#endif

vec3 toLinearFast(vec3 col) {
	return vec3(col.rgb*col.rgb);
}

void fragment() {
	ALBEDO = COLOR.xyz;
	if (!OUTPUT_IS_SRGB)
		ALBEDO = toLinearFast(ALBEDO);
	NORMAL = ALBEDO;
	
	#if defined(FORCED_TRANSPARENT)
	ALPHA = ALPHA;
	#endif
}
