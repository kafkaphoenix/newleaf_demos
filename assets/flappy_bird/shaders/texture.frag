#version 460 core

layout (location = 0) in vec2 vtexture_coords;

layout (location = 0) out vec4 frag_color;

// TEXTURE
uniform sampler2D texture_diffuse_1;
// HITBOX
uniform bool display_hitbox;
uniform vec4 hitbox_color;

void calculate_color() {
    frag_color = texture(texture_diffuse_1, vtexture_coords);
}

void calculate_transparency() {
    if (frag_color.a < 0.1) {
        discard;
    }
}

void calculate_hitbox() {
    if (display_hitbox) {
        frag_color = hitbox_color;
    }
}

void main() {
    calculate_color();
    calculate_hitbox();
    calculate_transparency();
}
