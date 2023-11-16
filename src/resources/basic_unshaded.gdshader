shader_type spatial;
render_mode cull_disabled, shadows_disabled, unshaded;

void fragment() {
	ALBEDO = COLOR.xyz;
}
