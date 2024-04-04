#version 440 core

layout(location = 0) out vec4 color;

in vec2 o_tex_coord;

uniform sampler2D u_tex0;
uniform sampler2D u_tex1;

void main()
{
    if(o_tex_coord.x < 0.5) 
    {
        color = texture(u_tex0, o_tex_coord);
    }
    else
    {
        color = texture(u_tex1, o_tex_coord);
    }
    
        color = texture(u_tex0, o_tex_coord);
}