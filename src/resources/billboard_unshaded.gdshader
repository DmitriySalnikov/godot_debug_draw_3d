shader_type spatial;
render_mode shadows_disabled, unshaded, depth_prepass_alpha;

uniform sampler2D depth_value : hint_depth_texture;

void vertex()
{
	MODELVIEW_MATRIX = VIEW_MATRIX * mat4(INV_VIEW_MATRIX[0], INV_VIEW_MATRIX[1], INV_VIEW_MATRIX[2], MODEL_MATRIX[3]);
	MODELVIEW_MATRIX = MODELVIEW_MATRIX * mat4(vec4(length(MODEL_MATRIX[0].xyz), 0.0, 0.0, 0.0), vec4(0.0, length(MODEL_MATRIX[1].xyz), 0.0, 0.0), vec4(0.0, 0.0, length(MODEL_MATRIX[2].xyz), 0.0), vec4(0.0, 0.0, 0.0, 1.0));
	//MODELVIEW_NORMAL_MATRIX = mat3(MODELVIEW_MATRIX);
}

vec3 toLinearFast(vec3 col) {
	return vec3(col.rgb*col.rgb);
}

void fragment() {
	ALBEDO = COLOR.xyz;
	if (!OUTPUT_IS_SRGB)
		ALBEDO = toLinearFast(ALBEDO);
	NORMAL = ALBEDO;
}
