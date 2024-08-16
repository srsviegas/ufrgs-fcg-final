#ifndef _UTILS_H
#define _UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
#include <ctime>

static GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        const char* error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;

            default:                               error = "UNKNOWN"; break;
        }
        fprintf(stderr, "ERROR: OpenGL \"%s\" in file \"%s\" (line %d)\n", error, file, line);
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

/* Returns True with `percentage`% chance; False otherwise. */
static bool chance(int percentage)
{
    int randomNumber = std::rand() % 100; 
    return randomNumber < percentage; 
}

/* Returns an integer in the interval [0, max) */
static int random(int max)
{
    return std::rand() % max;
}


#endif // _UTILS_H
