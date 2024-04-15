#version 440 core

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec4 inColor;

uniform mat4 u_view;
uniform mat4 u_proj;
//uniform mat4 u_model;

out vec4 oColor;
//out vec4 oCamPos;

void main()
{
    gl_Position = u_proj * u_view * inPos;
    oColor = inColor;
    //oCamPos = vec4(u_view[0][3],u_view[1][3],u_view[2][3], 0)
}