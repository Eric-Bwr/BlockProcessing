#pragma once
#include <iostream>
#include <GL/glew.h>

namespace gfx{
    void _GLGetError(const char* file, int line, const char* call);

//#define GLCALL(call) call; _GLGetError(__FILE__, __LINE__, #call)
#define GLCALL(call) call;
}
