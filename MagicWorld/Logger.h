#pragma once
#include "GLEW/glew.h"
#include <iostream>

#define GL_CALL_ASSERTION

#define ASSERT(x) if (!(x)) __debugbreak();
#ifdef GL_CALL_ASSERTION
#define GLCall(x) Logger::GLClearError();\
    x;\
    ASSERT(Logger::GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif // GL_CALL_ASSERTION


namespace Logger 
{

    void GLClearError();
    bool GLLogCall(const char* function, const char* file, int line);

}

