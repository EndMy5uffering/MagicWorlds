#version 440 core

/*struct Light
{
    vec3 Pos;
    vec3 Dir;
    vec4 Color;
}

struct MaterialProps
{
    float specular;
    float diffuse;
    float ambient_ref; 
    float alpha;
}*/

layout(location = 0) out vec4 color;

in vec4 oColor;

//uniform Light lights[];
//uniform MaterialProps props;

void main()
{
    color = oColor;
}