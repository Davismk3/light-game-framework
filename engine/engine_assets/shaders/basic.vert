#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in uvec3 a_tint;

uniform float a_rotation_speed;
uniform vec3 a_normal = vec3(0.0, 0.0, 1.0);
uniform vec3 a_center = vec3(0.0, 0.0, 0.0);
uniform float a_initial_angle = 0.0;
uniform vec3 a_shift = vec3(0.0, 0.0, 0.0);

uniform mat4 u_model = mat4(1.0);
uniform mat4 u_view = mat4(1.0);
uniform mat4 u_projection = mat4(1.0);

out vec2 v_uv;
out vec3 v_tint;

vec3 rotateAroundAxis(vec3 point, vec3 axis, float angle) {
    float cosine = cos(angle);
    float sine = sin(angle);

    return point * cosine + cross(axis, point) * sine + axis * dot(axis, point) * (1.0 - cosine);
}

void main() {
    float angle = a_initial_angle + a_rotation_speed;
    vec3 local_position = a_position - a_center;
    vec3 rotated_position = rotateAroundAxis(local_position, normalize(a_normal), angle);
    vec3 world_position = a_center + rotated_position + a_shift;

    v_uv = a_uv;
    v_tint = vec3(a_tint) / 255.0;

    gl_Position = u_projection * u_view * u_model * vec4(world_position, 1.0);
}