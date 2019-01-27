#version 400 core



// ins
in vec3 o_color;
in vec2 o_uv;

// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform sampler2D u_tex;

uniform vec4 u_c;

uniform int u_flag;



out vec4 out_color;

void main() 
{		
	vec4 texture_color = texture2D(u_tex, o_uv);
	vec4 font_color = u_c;
	font_color *= texture_color;
	font_color.a = texture_color.r;

// apply fragment color
	out_color = u_flag == 1 ? font_color : (u_flag == 2 ? texture_color : u_c);
}
