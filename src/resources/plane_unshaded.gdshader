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
