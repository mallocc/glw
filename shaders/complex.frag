#version 400 core



// ins
in vec3 o_color;
in vec3 o_to_light;
in vec3 o_to_eye;
in vec3 o_norm;
in vec3 o_tang;
in vec3 o_binorm;
in vec2 o_uv;

// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform vec3 u_light_pos;
uniform vec3 u_eye_pos;

uniform float u_specular_scale;
uniform float u_shininess;
uniform float u_brightness;

uniform vec3 u_ambient_color;
uniform vec3 u_diffuse_color;

uniform sampler2D u_tex;
uniform sampler2D u_norm;



// hemisphereical lighting vars
vec3 up = vec3(0.0f,1.0f,0.0f);



void main() 
{	
// get distance to the light
	float r = length(o_to_light);
	float r2 = r*r; 

// work out after interpolation
	vec3 L = normalize(o_to_light);
	vec3 V = normalize(o_to_eye);
	vec3 N = normalize(o_norm);
	vec3 T = normalize(o_tang);
	vec3 B = normalize(o_binorm);

// find the tangent vector space
	mat3 tangent_space = mat3(
		T,
		B,
		N
	);

// work out the normal from the normal map in world space
	vec3 converted_normal_map = texture2D(u_norm,o_uv).rgb * 2.0f - 1.0f;
	converted_normal_map = normalize(converted_normal_map);
	vec3 worldspace_normal_map = tangent_space * converted_normal_map;
	worldspace_normal_map = normalize(worldspace_normal_map);

// add the normal map to the face normal
	vec3 new_normal = N + worldspace_normal_map * 10.0f;
	new_normal = normalize(new_normal);

	if(textureSize(u_norm, 0).x > 0)
		N = new_normal;

// hemisphereical lighting
	float NdotL	= dot(N, up);
	float light_influence = NdotL * 0.5 + 0.5;
	vec3 hemi_color = mix(u_ambient_color / 10.0f, u_ambient_color, light_influence);

// calculate ambient effects
	vec3 amb = hemi_color;

// calculate diffuse effects
	vec3 dif = u_diffuse_color * clamp(dot(N, L), 0, 1) * u_brightness / r2;

// calculate specular effects
	float specular_term = 0;
// calculate specular reflection only if
// the surface is oriented to the light source
	if(dot(N, L) > 0)
	{
		// half vector
		vec3 H = normalize(L + V);
		specular_term = pow(dot(N, H), u_shininess);
	}
	float spe = u_specular_scale * specular_term;

// calculate final phong color
	vec3 final_color = (o_color + texture2D(u_tex,o_uv).rgb) * (amb + dif + spe);

// apply fragment color
	gl_FragColor = vec4(N,1);
}
