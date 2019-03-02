#version 400 core

// ins
in vec3 o_color;
in vec2 o_uv;

// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform sampler2D u_tex;

uniform vec4 u_color;
uniform int u_is_font;
uniform int u_is_texture;

out vec4 out_color;

void main() 
{	
    vec4 textureColor = texture2D(u_tex, o_uv);
    vec4 fontColor = u_color;
    fontColor *= textureColor;
    fontColor.a = textureColor.r;
    fontColor.a *= u_color.a;

    vec4 final_color = vec4(o_color, 1);

    if (u_is_texture == 1)
    {
        final_color = textureColor;
    }
    else if (u_is_font == 1)
    {
        final_color = fontColor;
    }
    else
    {
        final_color = u_color;
    }

    // apply fragment color
    out_color = final_color;
}
