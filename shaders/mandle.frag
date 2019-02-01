#version 400 core

// ins
in vec3 o_color;
in vec3 o_pos;

// uniforms
uniform vec3 u_target;
uniform vec3 u_mandle_properties;

float max_itr = 100.0f;
float scale = 0.25f;

float radius =  8.0f;
vec2 window = vec2(2.0f,2.0f);
vec3 color_map = vec3(4.5, 5.0, 5.5);

// outs
out vec4 out_color;


vec2 square_complex(vec2 a)
{
    float _re = a.x*a.x - a.y*a.y;
    float _im = a.x*a.y * 2.0f;
    return vec2(_re, _im);
}

float get_complex_point(float component,
                        float frame_start,
                        float axis_length,
                        float target_component,
                        float frame_length)
{
    float f = (component - frame_start)/ frame_length;
    float comp = axis_length * f / u_mandle_properties.y;
    return target_component + comp - (axis_length / 2.0 / u_mandle_properties.y);
}

void main() 
{
    vec2 c = vec2(get_complex_point(o_pos.x, -1.0f, 2.0f, u_target.x, window.x),
                  get_complex_point(-o_pos.y, -1.0f, 2.0f, u_target.y, window.y));
    vec2 z = vec2(0.0f,0.0f);

    int i = 0;
    while(z.x*z.x + z.y*z.y < radius && i < u_mandle_properties.x)
    {
//        z = vec2(abs(z.x), abs(z.y));
        z = square_complex(z) + c;
        i++;
    }

    // calculate the angle from the iteration
    float theta = i / 50.0f * 2.0 * 3.141;
    vec3 color;
    // calculate RGB
    color.x = (sin(theta + color_map.x / 3.0 * 3.141) + 1.0) / 2.0;
    color.y = (sin(theta + color_map.y / 3.0 * 3.141) + 1.0) / 2.0;
    color.z = (sin(theta + color_map.z / 3.0 * 3.141) + 1.0) / 2.0;

    if(i == u_mandle_properties.x)
        color = vec3(0.0f,0.0f,0.0f);

    float greyscale = i / u_mandle_properties.x;

    // apply fragment color
    out_color = vec4(color,1);
}
