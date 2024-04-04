#version 440 core

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec2 uvPos;

uniform mat4 u_viewproj;

out vec2 o_tex_coord;

void main()
{
    gl_Position = u_viewproj * inPos;
    o_tex_coord = uvPos;
}