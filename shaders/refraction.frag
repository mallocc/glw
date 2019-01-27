#version 400 core



// ins
in vec3 o_color;
in vec2 o_uv;
in vec3 o_norm;
in vec3 o_v_pos;

// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform vec3 u_eye_pos;

uniform sampler2D u_tex;

out vec4 out_color;

void main() 
{
	vec3 N = normalize(o_norm);

	vec4 eyePosition = u_v * vec4(u_eye_pos, 1.0);
	vec3 eyeVector = normalize(eyePosition.xyz  - o_v_pos);
  
  
	float reflectance = dot(eyeVector, N);
	
	vec2 new_uv = (u_p * vec4(o_v_pos,1)).xy + 1;

	out_color = texture2D(u_tex, (o_uv + 5.0f) / 10);
}
