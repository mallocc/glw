#version 400 core

// ins
layout(location = 0) in vec3 i_vert;
layout(location = 1) in vec3 i_color;
layout(location = 2) in vec3 i_norm;
layout(location = 3) in vec2 i_uv;

// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

// outs
out vec3 o_color;
out vec2 o_uv;


void main()
{
    // calculate world position of vertex
    vec4 world_pos  = u_m * vec4(i_vert, 1.0f);

    // color of vertex
    o_color         = i_color;

    // uv tex coord
    o_uv            = i_uv;

    // set projected point
    gl_Position     = u_p * u_v * world_pos;
}
