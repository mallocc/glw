#version 400 core
precision highp float;


// ins
in vec3 o_color;
in vec3 o_v_pos;
in vec3 o_norm;
in vec2 o_uv;


// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform sampler2D u_tex;

uniform vec3 u_light_pos;
uniform vec3 u_eye_pos;

uniform vec3 u_light_properties;
uniform vec3 u_ambient_color;
uniform vec3 u_light_color;



layout(location = 0) out vec4 out_color;

void main() 
{
    float u_brightness      = u_light_properties.x;
    float u_specular_scale  = u_light_properties.y;
    float u_shininess       = u_light_properties.z;

    vec3 N                  = normalize(o_norm);

    //determine surface to light direction
    vec4 lightPosition      = u_v * vec4(u_light_pos, 1.0);
    vec3 lightVector        =  normalize(lightPosition.xyz - o_v_pos);

    vec4 eyePosition        = u_v * vec4(u_eye_pos, 1.0);
    vec3 eyeVector          = normalize(eyePosition.xyz  - o_v_pos);
    vec3 reflectDir         = reflect(-lightVector, N);

    vec3 L                  = lightVector;

    float spec              = pow(max(dot(eyeVector, reflectDir), 0.0), u_shininess);
    vec3 specular           = u_specular_scale * spec * u_light_color;

    // calculate diffuse effects
    vec3 diffuse            = u_light_color * clamp(dot(N, L), 0, 1) * u_brightness;

    // calculate final phong color
    //vec3 final_color        = (o_color + vec3(texture2D(u_tex, o_uv))) * (diffuse + specular);
    vec3 final_color        = (o_color + vec3(texture2D(u_tex, o_uv))) * (diffuse + specular) + u_ambient_color;
    //final_color = final_color / 2.0f;

    //final_color = dif;

    // apply fragment color
    out_color               = vec4(final_color,1);
}
