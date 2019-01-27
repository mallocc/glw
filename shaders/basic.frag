#version 400 core



// ins
in vec3 o_color;

out vec4 out_color;

void main() 
{
// apply fragment color
	out_color = vec4(o_color,1);
}