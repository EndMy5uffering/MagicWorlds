#version 440 core

layout(location = 0) out vec4 color;

in vec4 oColor;

void main()
{
    color = oColor;
}