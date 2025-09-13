#version 460 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texture_coords;

layout (location = 0) out vec2 vtexture_coords;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    vtexture_coords = texture_coords;
}