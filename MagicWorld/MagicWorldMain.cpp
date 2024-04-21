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
#include "Noise.hpp"
#include "UILib.h"

#define IDX(x,y,w) ((y*w) + x)


int SCREEN_WIDTH = 900;
int SCREEN_HEIGHT = 900;

Camera cam{ 900,900, { 104.148, -201.413, 137.056},
{ 0.0553711, 0.913142, -0.403852},
{ -0.025391, 0.405631, 0.913684} };


struct Vertex 
{
    glm::vec4 pos;
    glm::vec2 uv;
};

struct Vertex_Map
{
    glm::vec4 pos;
    //glm::vec4 normal;
    glm::vec4 color;
};

void updateScreenSize(GLFWwindow* window, int width, int height)
{
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    cam.SetWidth(width);
    cam.SetHeight(height);
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
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sim", NULL, NULL);
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

    Shader mapShader{};
    mapShader.AddShaderSource("../Shader/MeshShader/MeshShaderFrag.glsl", GL_FRAGMENT_SHADER);
    mapShader.AddShaderSource("../Shader/MeshShader/MeshShaderVert.glsl", GL_VERTEX_SHADER);
    mapShader.BuildShader();

    Shader uiShader{};
    uiShader.AddShaderSource("../Shader/UIShader/UIFrag.glsl", GL_FRAGMENT_SHADER);
    uiShader.AddShaderSource("../Shader/UIShader/UIVert.glsl", GL_VERTEX_SHADER);
    uiShader.BuildShader();


    InputSystem is{window};
    UILib::UIManager manager{uiShader, g_renderer, cam, is};
    UILib::GUIElement el0{};
    el0.SetColor({0, 0, 0, 1});
    el0.GetTransform().SetPosition({-10, 0, 0});
    el0.GetTransform().SetScale({0.5, 0.5, 0});
    manager.AddElement(el0);

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


    int width = 64;
    int height = 64;
    float mulHight = 100.0;
    std::vector<Vertex_Map> vxbuffer_map;
    Noise::Perlin perlin = Noise::Perlin(666, 0.01, 0.5, 2.0);
    perlin.SetOctaves(5);
    for (int i = 0; i < width * height; ++i)
    {
        int x = i % width;
        int y = i / width;

        double noise = perlin.at2D_octives(double(x), double(y));
        double noise_clam = noise * 0.5 + 0.5;
        //std::cout << "X: " << x << " Y: " << y << " h: " << noise_clam << " h2: " << noise << "\n";
        Color c = Color::lerp(
            { 120, 0, 0, 255 },
            { 0, 255, 0, 255 },
            noise_clam);
        vxbuffer_map.push_back({ {double(x) * 2.0, double(y) * 2.0, noise * mulHight, 1.0}, {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, 1.0} });
    }

    std::vector<unsigned int> idxbuff_map;
    for (int y = 0; y < height-1; ++y)
    {
        for (int x = 0; x < width-1; ++x)
        {
            idxbuff_map.push_back(IDX(x,y,width));
            idxbuff_map.push_back(IDX(x, y, width) + width);
            idxbuff_map.push_back(IDX(x, y, width) + 1);
            idxbuff_map.push_back(IDX(x, y, width) + width + 1);
            idxbuff_map.push_back(IDX(x, y, width) + 1);
            idxbuff_map.push_back(IDX(x, y, width) + width);
        }
    }

    VertexBufferLayout vbl_map;
    vbl_map.Push<glm::vec4>();
    vbl_map.Push<glm::vec4>();

    Mesh<Vertex_Map> mesh_map{vxbuffer_map, idxbuff_map, vbl_map};
    

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
        //mesh.Draw(g_renderer);

        glEnable(GL_DEPTH_TEST);
        mapShader.Bind();
        mapShader.SetUniformMat4f("u_view", cam.GetViewMatrix());
        mapShader.SetUniformMat4f("u_proj", cam.GetProjMatrix());
        mesh_map.Bind();
        mesh_map.Draw(g_renderer);

        g_renderer.DrawAxis(cam);


        manager.RenderUIElements();


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