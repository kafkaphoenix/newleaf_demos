#version 460 core

layout (location = 0) in vec2 vtexture_coords;

layout (location = 0) out vec4 frag_color;

// TEXTURE ATLAS
uniform sampler2D texture_diffuse_1;
uniform float texture_atlas_rows;
uniform vec2 texture_atlas_offset;
// HITBOX
uniform bool display_hitbox;
uniform vec4 hitbox_color;

void calculate_color() {
    vec2 offset_texture = vtexture_coords / texture_atlas_rows + texture_atlas_offset;
    frag_color = texture(texture_diffuse_1, offset_texture);
}

void calculate_hitbox() {
    if (display_hitbox) {
        frag_color = hitbox_color;
    }
}

void calculate_transparency() {
    if (frag_color.a < 0.1) {
        discard;
    }
}

void main() {
    calculate_color();
    calculate_hitbox();
    calculate_transparency();
}
