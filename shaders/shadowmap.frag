#version 400 core



// ins
in vec3 o_color;
in vec2 o_uv;

in vec2 o_shadow_uv;

// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform sampler2D u_tex;

uniform sampler2D u_final_depth;
uniform sampler2D u_shadow_depth;
uniform sampler2D u_shadow_color;

out vec4 out_color;

void main() 
{	

	vec3 final_color = vec3(0.0f,0.0f,0.0f);
	if(texture2D(u_shadow_depth, o_shadow_uv).r < texture2D(u_final_depth, o_shadow_uv).r)
		final_color = (o_color + texture2D(u_shadow_color, o_shadow_uv).rgb);
	final_color = (o_color + texture2D(u_shadow_color, o_uv).rgb);
// apply fragment color
	out_color = vec4(final_color,1);
}
