#version 400 core



// ins
in vec3 o_color;
in vec2 o_uv;

// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform sampler2D u_tex;
uniform sampler2D u_tex0;
uniform sampler2D u_tex1;
uniform sampler2D u_tex2;
uniform sampler2D u_tex3;
uniform sampler2D u_tex4;
uniform sampler2D u_tex5;
uniform sampler2D u_tex6;
uniform sampler2D u_tex7;
uniform sampler2D u_tex8;
uniform sampler2D u_tex9;

uniform int u_current_frame;
uniform int u_motion_length;

out vec4 out_color;

float factor(int frame)
{
	int x = u_motion_length - u_current_frame;
	int y = frame + x;
	y = y % u_motion_length;
	return y;
}

void main() 
{	
	vec3 final_color  = 
		  texture2D(u_tex0, o_uv).rgb	* factor(0) 
		+ texture2D(u_tex1, o_uv).rgb	* factor(1)
		+ texture2D(u_tex2, o_uv).rgb	* factor(2)
		+ texture2D(u_tex3, o_uv).rgb	* factor(3)
		+ texture2D(u_tex4, o_uv).rgb	* factor(4)
		+ texture2D(u_tex5, o_uv).rgb	* factor(5) 
		+ texture2D(u_tex6, o_uv).rgb	* factor(6)
		+ texture2D(u_tex7, o_uv).rgb	* factor(7)
		+ texture2D(u_tex8, o_uv).rgb	* factor(8)
		+ texture2D(u_tex9, o_uv).rgb	* factor(9);
		final_color /= 45.0f;
	out_color = vec4(final_color,1);
}
