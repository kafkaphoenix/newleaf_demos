#version 460 core

layout (location = 0) in vec3 position;

layout (location = 0) out vec3 vtexture_coords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    vtexture_coords = position * 500.f; // Scale up the texture coords to make the fog works
    mat4 new_view = view;
    new_view[3] = vec4(0.f, 0.f, 0.f, 1.f); // Remove translation
    vec4 world_position = model * vec4(position, 1.f);
    vec4 view_position = new_view * world_position;
    vec4 clip_position = projection * view_position;
    gl_Position = clip_position.xyww;
}