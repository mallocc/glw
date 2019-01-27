#version 400 core



// ins
in vec3 o_color;
in vec2 o_uv;

// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform sampler2D u_tex;

uniform vec3 u_glow_res;

out vec4 out_color;

void main() 
{	
	vec3 final_color;
	for(float x = -u_glow_res.z* u_glow_res.x; x < u_glow_res.z* u_glow_res.x; x+= u_glow_res.x)
		for(float y = -u_glow_res.z* u_glow_res.y; y < u_glow_res.z* u_glow_res.y; y+= u_glow_res.y)
		{	
			
			vec2 t = o_uv + vec2(x,y);
			if(t.x >= 0.0f && t.x <= 1.0f && t.y >= 0.0f && t.y <= 1.0f)
			{	
				float i = 5.0f / sqrt(max(1, pow(x / u_glow_res.x,2) + pow(y / u_glow_res.y,2)));
				final_color += texture2D(u_tex,o_uv + vec2(x,y)).rgb * i;
			}
		}
	//texture2D(u_tex,o_uv).rgb +
	//texture2D(u_tex,o_uv + vec2(1,0) * u_glow_res.x).rgb + 
	//texture2D(u_tex,o_uv + vec2(-1,0) * u_glow_res.x).rgb + 
	//texture2D(u_tex,o_uv + vec2(-0,1) * u_glow_res.y).rgb + 
	//texture2D(u_tex,o_uv + vec2(0,1) * u_glow_res.y).rgb;

	final_color /= u_glow_res.z * u_glow_res.z / 2.0f;
	final_color += texture2D(u_tex,o_uv).rgb;
	final_color /= 2.0f;

// apply fragment color
	out_color = vec4(final_color,1);
}
