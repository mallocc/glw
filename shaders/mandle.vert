#version 400 core



// ins
layout(location = 0) in vec3 i_vert;
layout(location = 1) in vec3 i_color;

// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

// outs
out vec3 o_color;
out vec3 o_pos;


void main()
{
// color of vertex
	o_color        = i_color;
// color of vertex
	o_pos          = i_vert;

// set projected point
	gl_Position    = u_p * u_v * u_m * vec4(i_vert, 1.0f);	
}