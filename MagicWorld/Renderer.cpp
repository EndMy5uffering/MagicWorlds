#include "Renderer.h"

#include <iostream>



void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(VertexArray& va, IndexBuffer& ib) const
{
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawLine(const glm::vec3 v0, const glm::vec3 v1, const glm::vec4 color, Camera& cam, float thickness)
{
    m_Line3D.Bind();
    m_Line3D.SetUniformMat4f("uViewProj", cam.GetViewProjMatrix());
    m_Line3D.SetUniform4f("uColor", color);

    float vertBuff[6] = {
        v0.x, v0.y, v0.z,
        v1.x, v1.y, v1.z
    };

    GLuint vertexbuffer;
    GLCall(glGenBuffers(1, &vertexbuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertBuff), vertBuff, GL_STATIC_DRAW));

    GLuint array;
    glGenVertexArrays(1, &array);
    glBindVertexArray(array);


    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(
        0,
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized
        0,                  // stride
        (void*)0            // array buffer offset
    ));

    GLCall(glLineWidth(thickness));
    GLCall(glDrawArrays(GL_LINES, 0, 2));

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &array);
}

void Renderer::DrawLine(const glm::vec3 v0, const glm::vec3 v1, const glm::vec4 color, Camera& cam)
{
    DrawLine(v0, v1, color, cam, 1.0f);
}

void Renderer::DrawLine(const glm::vec2 v0, const glm::vec2 v1, const glm::vec4 color)
{
    m_Line2D.Bind();
    m_Line2D.SetUniform4f("uColor", color);

    float vertBuff[4] = {
        v0.x, v0.y,
        v1.x, v1.y
    };

    GLuint vertexbuffer;
    GLCall(glGenBuffers(1, &vertexbuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertBuff), vertBuff, GL_STATIC_DRAW));

    GLuint array;
    glGenVertexArrays(1, &array);
    glBindVertexArray(array);


    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(
        0,
        2,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized
        0,                  // stride
        (void*)0            // array buffer offset
    ));

    GLCall(glLineWidth(3.0f));
    GLCall(glDrawArrays(GL_LINES, 0, 2));

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &array);
}

void Renderer::DrawAxis(Camera& cam, float length, float thickness)
{
    glm::vec3 lv0{0, 0, 0};
    glm::vec3 lv1{10000, 0, 0};
    glm::vec3 lv2{0, 10000, 0};
    glm::vec3 lv3{0, 0, 10000};
    DrawLine(lv0, lv1, glm::vec4{1.0, 0.0, 0.0, 1.0}, cam, thickness);
    DrawLine(lv0, lv2, glm::vec4{0.0, 1.0, 0.0, 1.0}, cam, thickness);
    DrawLine(lv0, lv3, glm::vec4{0.0, 0.0, 1.0, 1.0}, cam, thickness);
}


