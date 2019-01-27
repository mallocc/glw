#version 400 core



// ins
in vec3 o_color;
in vec3 o_to_light;
in vec3 o_to_eye;
in vec3 o_light;
in vec3 o_eye;
in vec3 o_norm;
in vec3 o_tang;
in vec3 o_binorm;
in vec2 o_uv;
in vec3 o_tfrag;
in vec3 o_frag;

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
uniform sampler2D u_height;

// hemisphereical lighting vars
vec3 up = vec3(0.0f,1.0f,0.0f);
float height_scale = 0.5f;
 

vec2 ParallaxMapping(in vec2 T, in vec3 V)
{
   // determine number of layers from angle between V and N
   const float minLayers = 5;
   const float maxLayers = 10;
   float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0, 1, 0), V)));

   // height of each layer
   float layerHeight = 1.0 / numLayers;
   // depth of current layer
   float currentLayerHeight = layerHeight * maxLayers;
   // shift of texture coordinates for each iteration
   vec2 dtex = height_scale * V.xz / numLayers;

   // current texture coordinates
   vec2 currentTextureCoords = T;

   // get first depth from heightmap
   float heightFromTexture = texture(u_height, currentTextureCoords).r;

   // while point is above surface
   while(heightFromTexture < currentLayerHeight) 
   {
      // to the next layer
      currentLayerHeight -= layerHeight;
      // shift texture coordinates along vector V
      currentTextureCoords += dtex;
      // get new depth from heightmap
      heightFromTexture = texture(u_height, currentTextureCoords).r;
   }

   // return results
   return currentTextureCoords;
}

vec2 ParallaxMapping1(vec2 texCoords, vec3 viewDir)
{ 
    float height = texture2D(u_height, texCoords).r;
    vec2 p = viewDir.xy * (height * height_scale);
    return texCoords + p;    
} 

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

	mat3 tangent_space = mat3(
		o_tang,
		o_binorm,
		o_norm
	);

	// offset texture coordinates with Parallax Mapping
    vec2 texCoords = ParallaxMapping1(o_uv, tangent_space*V);
if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
    discard;





// work out the normal from the normal map in world space
	vec3 converted_normal_map = texture2D(u_norm,texCoords).rgb * 2.0f - 1.0f;
	converted_normal_map = normalize(converted_normal_map);
	vec3 worldspace_normal_map = mat3(o_tang,o_binorm,o_norm) * converted_normal_map;
	worldspace_normal_map = normalize(worldspace_normal_map);

// add the normal map to the face normal
	vec3 new_normal = N + worldspace_normal_map * 10.0f;
	new_normal = normalize(new_normal);

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
	vec3 final_color = texture2D(u_tex,texCoords).rgb * (amb + dif + spe);

// apply fragment color
	gl_FragColor = vec4(final_color,1);
}
