#version 460 core

layout (location = 0) in vec3 vtexture_coords;

layout (location = 0) out vec4 frag_color;

uniform samplerCube texture_diffuse_1;
// BLEND
uniform bool blend_texture_enabled;
uniform samplerCube blend_texture;
uniform float blend_texture_factor;
uniform bool blend_color_enabled;
uniform vec4 blend_color;
uniform float blend_color_factor;
// FOG
uniform bool fog_enabled;
uniform vec4 fog_color;
uniform float fog_lower_limit;
uniform float fog_upper_limit;

void main() {
	vec4 texture1 = texture(texture_diffuse_1, vtexture_coords);
	vec4 texture2 = texture(blend_texture, vtexture_coords);
	vec4 final_color = texture2;

	vec4 fog_color = fog_color;
	if (blend_texture_enabled) {
  		final_color = mix(texture1, texture2, blend_texture_factor);
		fog_color = mix(vec4(0.f, 0.f, 0.f, 1.f), fog_color, blend_texture_factor);
	}

	if (fog_enabled) {
		float fog_factor = clamp((vtexture_coords.y - fog_lower_limit) / (fog_upper_limit - fog_lower_limit), 0.f, 1.f);
		final_color = mix(vec4(fog_color.rgb, 0.9f), final_color, fog_factor);
	}

	frag_color = final_color;
}