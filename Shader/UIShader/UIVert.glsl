#version 440 core

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec4 uvPos;
layout(location = 2) in vec4 col;

uniform mat4 u_view;
uniform mat4 u_proj;

out vec4 o_color;

void main()
{
    gl_Position = u_proj * inPos;
    o_color = col;
}