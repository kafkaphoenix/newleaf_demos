#version 460 core

layout (location = 0) in vec2 vtexture_coords;

layout (location = 0) out vec4 frag_color;

uniform sampler2D texture_diffuse_1;
uniform float use_texture_atlas;
uniform float num_rows;
uniform vec2 offset;
uniform float use_color;
uniform vec4 color;

void load_texture() {
    if (int(use_color) == 0) {
        vec2 offset_texture = vtexture_coords;
        if (int(use_texture_atlas) == 1) {
            offset_texture = vtexture_coords / num_rows + offset;
        }
        frag_color = texture(texture_diffuse_1, offset_texture);
    } else {
        frag_color = color;
    }
}

void calculate_transparency() {
    if (frag_color.a < 0.1) {
        discard;
    }
}

void main() {
    load_texture();
    calculate_transparency();
}
