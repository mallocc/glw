#version 400 core
precision highp float;


// ins
layout(location = 0) in vec3 i_vert;
layout(location = 1) in vec3 i_color;
layout(location = 2) in vec3 i_norm;
layout(location = 3) in vec2 i_uv;
layout(location = 4) in vec3 i_tang;

// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform vec3 u_light_pos;
uniform vec3 u_eye_pos;

// outs
out vec3 o_color;
out vec2 o_uv;
varying  vec3 o_v_pos;

varying  vec3 o_norm;
varying  vec3 o_tang;
varying  vec3 o_binorm;

void main()
{
	mat4 n4          = transpose(inverse(u_v*u_m));
	mat3 n = mat3(n4);

	mat4 v_m        = u_v * u_m;
	vec4 m_pos		= v_m * vec4(i_vert, 1.0f);

// color of vertex
	o_color			= i_color;

// uv tex coord
	o_uv			= i_uv;

// normal in world space
	o_norm          = n * i_norm;
// normal in world space
	o_tang          = n * i_tang;
//	binormal in world space
	o_binorm        = -cross(o_norm, o_tang);

// view position	
	o_v_pos         = m_pos.xyz;

// set projected point
	gl_Position		= u_p * m_pos;	
}