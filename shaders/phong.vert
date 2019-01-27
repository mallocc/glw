#version 400 core
precision highp float;


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
out vec3 o_v_pos;
out vec3 o_norm;
out vec2 o_uv;

void main()
{
	mat4 n4           = transpose(inverse(u_v*u_m));
        mat3 n            = mat3(n4);

	mat4 v_m          = u_v * u_m;
        vec4 m_pos	  = v_m * vec4(i_vert, 1.0f);

// color of vertex
        o_color           = i_color;

// normal in world space
	o_norm            = n * i_norm;

// view position	
	o_v_pos           = m_pos.xyz;
	
// uv tex coord
        o_uv		  = i_uv;

// set projected point
        gl_Position	  = u_p * m_pos;
}
