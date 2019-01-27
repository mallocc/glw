#version 400 core



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
out vec3 o_to_light;
out vec3 o_to_eye;
out vec3 o_norm;
out vec3 o_tang;
out vec3 o_binorm;
out vec2 o_uv;


void main()
{
// calculate world position of vertex
	vec4 world_pos = u_m * vec4(i_vert, 1.0f);

// normal in world space
	vec4 new_norm  = u_m * vec4(i_norm,1);
	o_norm         = new_norm.xyz;

// tangent in world space
	vec4 new_tang = u_m * vec4(i_tang,1);
	o_tang = new_tang.xyz;

//	binormal in world space
	o_binorm = normalize(cross(o_norm, o_tang));

// direction to eye
	o_to_eye       = u_eye_pos - world_pos.xyz;	

// direction to light
	o_to_light     = u_light_pos - world_pos.xyz;	

// color of vertex
	o_color        = i_color;

// uv tex coord
	o_uv = i_uv;

// set projected point
	gl_Position    = u_p * u_v * world_pos;	
}