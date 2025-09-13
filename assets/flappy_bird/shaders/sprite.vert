#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coords;

layout (location = 0) out vec2 vtexture_coords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    vec4 world_position = model * vec4(position.x, position.y, position.z, 1.f);
    vec4 view_position = view * world_position;
    vec4 clip_position = projection * view_position;
    gl_Position = clip_position;
    vtexture_coords = texture_coords;
}