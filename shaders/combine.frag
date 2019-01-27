#version 400 core



// ins
in vec3 o_color;
in vec2 o_uv;

// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform sampler2D u_tex;
uniform sampler2D u_tex1;

uniform vec3 u_mix_properties;

out vec4 out_color;

vec3 invert(vec3 c)
{
	return vec3(1,1,1) - c;
}

void main() 
{	
	//vec4 final_color = vec4(texture2D(u_tex,o_uv).rgb, u_mix_properties.x) + vec4(texture2D(u_tex1,o_uv).rgb, u_mix_properties.y);

	// screen
	//vec3 final_color = vec3(1,1,1) - invert(texture2D(u_tex,o_uv).rgb * u_mix_properties.x) * invert(texture2D(u_tex1,o_uv).rgb * u_mix_properties.y);

	// multiply
	//vec3 final_color = (texture2D(u_tex,o_uv).rgb * u_mix_properties.x) * (texture2D(u_tex1,o_uv).rgb * u_mix_properties.y);

	// add
	//vec3 final_color = (texture2D(u_tex,o_uv).rgb * u_mix_properties.x) + (texture2D(u_tex1,o_uv).rgb * u_mix_properties.y);

	// overlay
	//vec3 final_color = vec3(1,1,1) - 2 * invert(texture2D(u_tex,o_uv).rgb * u_mix_properties.x) * invert(texture2D(u_tex1,o_uv).rgb * u_mix_properties.y);


	//final_color *= u_mix_properties.z;

	//final_color = clamp(final_color, vec3(0,0,0), texture2D(u_tex,o_uv).rgb * u_mix_properties.x);
	//final_color = clamp(final_color, vec4(0,0,0,0), vec4(texture2D(u_tex,o_uv).rgb, u_mix_properties.x));


	//vec3 final_color = invert(texture2D(u_tex,o_uv).rgb);// + invert(texture2D(u_tex1,o_uv).rgb);
	//final_color = invert(vec3());

	vec4 final_color = vec4(texture2D(u_tex,o_uv).rgb, 1.0f) * vec4(texture2D(u_tex1,o_uv).rgb, 0.5f);
	//final_color /= 1.1f;

// apply fragment color
	//out_color = vec4(final_color,1);
	out_color = final_color;
}
