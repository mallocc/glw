#version 400 core
precision highp float;


// ins
in vec3 o_color;
in vec2 o_uv;
varying  vec3 o_v_pos;

varying  vec3 o_norm;
varying  vec3 o_tang;
varying  vec3 o_binorm;

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
uniform sampler2D u_norm;

out vec4 out_color;

void main() 
{

	float u_brightness = u_light_properties.x;
	float u_specular_scale = u_light_properties.y;
	float u_shininess = u_light_properties.z;

	vec3 N = normalize(o_norm); 
	
	if(textureSize(u_norm, 0).x > 2)
	{
		vec3 converted_normal_map = texture2D(u_norm,o_uv).rgb * 2.0f - 1.0f;
		// find the tangent vector space
		mat3 tangent_space = mat3(
			normalize(o_tang),
			normalize(o_binorm),
			N
		);
		converted_normal_map = normalize(converted_normal_map);
		vec3 worldspace_normal_map = tangent_space * converted_normal_map;
		worldspace_normal_map = normalize(worldspace_normal_map);

		//N += worldspace_normal_map;
		N = worldspace_normal_map;
	}

	//determine surface to light direction
	vec4 lightPosition = u_v * vec4(u_light_pos, 1.0);
	vec3 lightVector =  normalize(lightPosition.xyz - o_v_pos);

	vec4 eyePosition = u_v * vec4(u_eye_pos, 1.0);
	vec3 eyeVector = normalize(eyePosition.xyz  - o_v_pos);
	vec3 reflectDir = reflect(-lightVector, N); 
  
	vec3 L = lightVector;
  
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
