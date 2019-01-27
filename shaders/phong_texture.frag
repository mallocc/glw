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

uniform vec3 u_light_pos;
uniform vec3 u_eye_pos;
uniform vec3 u_light_properties;

uniform vec3 u_ambient_color;
uniform vec3 u_light_color;

uniform sampler2D u_tex;

out vec4 out_color;

void main() 
{

	float u_brightness = u_light_properties.x;
	float u_specular_scale = u_light_properties.y;
	float u_shininess = u_light_properties.z;

	vec3 N = normalize(o_norm);

	//determine surface to light direction
	vec4 lightPosition = u_v * vec4(u_light_pos, 1.0);
	vec3 lightVector =  normalize(lightPosition.xyz - o_v_pos);

	vec4 eyePosition = u_v * vec4(u_eye_pos, 1.0);
	vec3 eyeVector = normalize(eyePosition.xyz  - o_v_pos);
	vec3 reflectDir = reflect(-lightVector, N); 
  
	vec3 L = normalize(lightVector);              //light direction
  
	float spec = pow(max(dot(eyeVector, reflectDir), 0.0), u_shininess);
	vec3 specular = u_specular_scale * spec * u_light_color;  

	// calculate diffuse effects
	vec3 diffuse = u_light_color * clamp(dot(N, L), 0, 1) * u_brightness;

	// calculate final phong color
	vec3 final_color = (o_color + texture2D(u_tex,o_uv).rgb) * (diffuse + specular);

	//final_color = dif;

	// apply fragment color
	out_color = vec4(final_color,1);
}
