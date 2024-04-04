#pragma once

#include <GLEW/glew.h>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Logger.h"

class Renderer
{
public:
    
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer() : m_ClearColor{0} {
        m_Line2D.AddShaderSource("../Shader/Lines/Line2DFrag.GLSL", GL_FRAGMENT_SHADER);
        m_Line2D.AddShaderSource("../Shader/Lines/Line2DVert.GLSL", GL_VERTEX_SHADER);
        m_Line2D.BuildShader();
        m_Line3D.AddShaderSource("../Shader/Lines/LineFrag.GLSL", GL_FRAGMENT_SHADER);
        m_Line3D.AddShaderSource("../Shader/Lines/LineVert.GLSL", GL_VERTEX_SHADER);
        m_Line3D.BuildShader();
    }

    Renderer operator=(Renderer&&) = delete;

    ~Renderer() {}

    void Clear() const;
    void Draw(VertexArray& va, IndexBuffer& ib) const;
    void DrawLine(glm::vec3 v0, glm::vec3 v1, glm::vec4 color, Camera& cam, float thickness);
    void DrawLine(glm::vec3 v0, glm::vec3 v1, glm::vec4 color, Camera& cam);
    void DrawLine(glm::vec2 v0, glm::vec2 v1, glm::vec4 color);
    void DrawAxis(Camera& cam, float length = 1.0f, float thickness = 1.0f);

private:
    Shader m_Line3D;
    Shader m_Line2D;
    glm::vec4 m_ClearColor;
};

