#version 330 core

out vec4 frag_color;

in vec2 v_uv;
in vec3 v_tint;

uniform bool use_texture = false;           // true if use texture, false if solid color;
uniform sampler2D u_texture;                // the texture
uniform vec3 u_tint = vec3(1.0, 1.0, 1.0);  // baseline is no tint
uniform float u_light = 1.0;                // baseline is fully illuminated
uniform float u_opacity = 1.0;              // baseline is fully opaque
uniform float u_alpha_cutoff = 0.0;         // baseline is no alpha cutoff

void main() {
    vec4 color;

    // Texture
    if (use_texture) {
        color = texture(u_texture, v_uv);
    } else {
        color = vec4(1.0, 1.0, 1.0, 1.0);
    }

    // Alpha
    if (color.a < u_alpha_cutoff) discard;

    // Vertex Values
    //color.rgb *= v_tint;

    // Shader Values
    color.rgb *= u_tint;
    color.rgb *= u_light;
    color.a *= u_opacity;

    frag_color = color;
}