//#define NO_DEPTH
//#define FORCED_OPAQUE

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

varying vec4 custom;

void vertex(){
	custom = INSTANCE_CUSTOM;
}

vec3 toLinearFast(vec3 col) {
	return vec3(col.rgb*col.rgb);
}

void fragment() {
	// // Use interleaved gradient noise, which is fast but still looks good.
	// const vec3 magic = vec3(0.06711056f, 0.00583715f, 52.9829189f);
	// float fade = COLOR.a;
	// fade = 0.5;
	// // Use a hard cap to prevent a few stray pixels from remaining when past the fade-out distance.
	// if (fade < 0.001f || fade < fract(magic.z * fract(dot(FRAGCOORD.xy+vec2(MODEL_MATRIX[3].x, MODEL_MATRIX[3].y), magic.xy)))) {
	// 	discard;
	// }
	
	if (FRONT_FACING){
		ALBEDO = COLOR.xyz;
		#if !defined(FORCED_OPAQUE)
		ALPHA = COLOR.a;
		#endif
	}else{
		ALBEDO = custom.xyz;
		#if !defined(FORCED_OPAQUE)
		ALPHA = custom.a;
		#endif
	}
	
	if (!OUTPUT_IS_SRGB)
		ALBEDO = toLinearFast(ALBEDO);
	NORMAL = ALBEDO;
}
