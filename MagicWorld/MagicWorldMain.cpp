#include<GLEW/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include "Logger.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "InputSystem.h"
#include "Texture.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

#include "Mesh.h"

int SCREEN_WIDTH = 900;
int SCREEN_HEIGHT = 900;


struct Vertex 
{
    glm::vec4 pos;
    glm::vec2 uv;
};

void updateScreenSize(GLFWwindow* window, int width, int height)
{
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    
    //glfwWindowHint(GLFW_MAXIMIZED, 1);
    //glfwWindowHint(GLFW_SAMPLES, 16);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Magic Worlds", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(0); //Disable vsync

    if (glewInit() != GLEW_OK)
    {
        std::cout << "[Error]: Glew init" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    glfwSetWindowSizeCallback(window, updateScreenSize);

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    glClearColor(0.34901960784313724f, 0.3843137254901961f, 0.4588235294117647f, 1.0f);

    Renderer g_renderer;

    Texture tex2{};
    tex2.LoadFromFile("../Textures/WoodFloor/01.png");
    tex2.SetMagFilter(MAG_LINEAR);
    tex2.Pack();
    tex2.BindToSlot(1);

    Texture tex{};
    tex.LoadFromFile("../Textures/WoodFloor/00.png");
    tex.SetMagFilter(MAG_NEAREST);

    tex.SetPixel(1, 1, Color{ 0,0,0,255 });
    tex.SetPixel(2, 2, Color{ 0,0,0,255 });
    tex.SetPixel(3, 3, Color{0,0,0,255});

    tex.DrawLine({ 0,4 }, {16,16}, Color{ 50,0,0,255 });

    tex.DrawHCicle({ 5, 16 }, 5, Color{ 0,50,0,255 });
    tex.DrawCicle({ 13+8, 16 }, 8, Color{ 0,50,0,255 });

    tex.DrawHRect({ 13 + 8, 16 }, { 10, 10 }, Color{ 0,0,150,255 });
    tex.DrawRect({ 13 + 8, 16 }, { 5, 5 }, Color{150,0,150,255});

    tex.Pack();
    tex.BindToSlot(0);

    Shader texShader{};
    texShader.AddShaderSource("../Shader/TextureShader/TextrueFrag.glsl", GL_FRAGMENT_SHADER);
    texShader.AddShaderSource("../Shader/TextureShader/TextrueVert.glsl", GL_VERTEX_SHADER);
    texShader.BuildShader();

    std::vector<Vertex> vxbuffer;
    vxbuffer.push_back({ { 1, 1, 0, 1 }, { 1, 0 } });
    vxbuffer.push_back({ { -1, 1, 0, 1 }, { 0, 0 } });
    vxbuffer.push_back({ { -1, -1, 0, 1 }, { 0, 1 } });
    vxbuffer.push_back({ { 1, -1, 0, 1 }, { 1, 1 } });
    std::vector<unsigned int> idxbuff{ 0, 1, 2, 2, 3, 0 };
    VertexBufferLayout vbl;
    vbl.Push<glm::vec4>();
    vbl.Push<glm::vec2>();

    Mesh<Vertex> mesh{vxbuffer, idxbuff, vbl};

    Camera cam{900,900};
    cam.SetFar(1000);
    cam.SetNear(0.001);
    cam.SetFOV(60);

    double time = 0;
    while (!glfwWindowShouldClose(window))
    {
        double angle = 0;
        glm::mat4 model{
            cos(glm::radians(angle)) * 1, -sin(glm::radians(angle)) * 1, 0, 1,
                sin(glm::radians(angle)) * 1, cos(glm::radians(angle)) * 1, 0, 1,
                0, 0, 0, 0,
                0, 0, 0, 1
        };

        /* Render here */
        g_renderer.Clear();

        cam.HandleInput(window);

        texShader.Bind();
        texShader.SetUniform1i("u_tex0", 0);
        texShader.SetUniform1i("u_tex1", 1);
        texShader.SetUniformMat4f("u_viewproj", cam.GetViewProjMatrix() * glm::transpose(model));

        mesh.Bind();
        //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        mesh.Draw(g_renderer);

        g_renderer.DrawAxis(cam);
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());

        if (time > 360.0) 
        {
            time = 0;
        }
        time += 0.8;
    }

    glfwTerminate();
    return 0;
}