//           Universidade Federal do Rio Grande do Sul
// Trabalho Final da Disciplina de Fundamentos de Computação Gráfica
//
//                        Sandro Viégas
//                        Walter Frank

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include <map>
#include <stack>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tiny_obj_loader.h>
#include <stb_image.h>

#include "utils.h"
#include "matrices.h"
#include "Camera.h"
#include "Renderer.h"
#include "Window.h"
#include "Input.h"

using namespace std;

const float freeCameraSpeed = 1.0f;

int main(int argc, char *argv[])
{
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    float now = (float)glfwGetTime();
    float last = 0;
    float timeDelta = 0;

    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(screenWidth, screenHeight, "Trabalho Final - FCG", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window, KeyCallback);                 // Keypress
    glfwSetMouseButtonCallback(window, MouseButtonCallback); // Mouse click
    glfwSetCursorPosCallback(window, CursorPosCallback);     // Mouse cursor position
    glfwSetScrollCallback(window, ScrollCallback);           // Mouse scroll

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    FramebufferSizeCallback(window, screenWidth, screenHeight);

    // Output GPU information in terminal
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *glversion = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    LoadShadersFromFiles();

    /* LOADING TEXTURES */
    LoadTextureImage("../../data/tc-earth_daymap_surface.jpg");      // TextureImage0
    LoadTextureImage("../../data/tc-earth_nightmap_citylights.gif"); // TextureImage1

    /* BUILDING OBJECTS */
    ObjModel planemodel("../../data/plane.obj");
    ComputeNormals(&planemodel);
    BuildTrianglesAndAddToVirtualScene(&planemodel);

    if (argc > 1)
    {
        ObjModel model(argv[1]);
        BuildTrianglesAndAddToVirtualScene(&model);
    }

    TextRendering_Init();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    /* MAIN LOOP */
    while (!glfwWindowShouldClose(window))
    {
        now = (float)glfwGetTime();
        timeDelta = now - last;
        last = now;

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(g_GpuProgramID);

        // Calculating free camera movements
        glm::vec4 movementDirection = cam.getViewVec();
        movementDirection.y = 0.0f; // Fixate vertical movement
        glm::vec4 side = crossproduct(movementDirection, cam.getUpVec());
        if (isKeyDown_W)
        {
            cam._move(timeDelta * freeCameraSpeed * normalize(movementDirection));
        }
        if (isKeyDown_S)
        {
            cam._move(timeDelta * -freeCameraSpeed * normalize(movementDirection));
        }
        if (isKeyDown_A)
        {
            cam._move(timeDelta * -freeCameraSpeed * normalize(side));
        }
        if (isKeyDown_D)
        {
            cam._move(timeDelta * freeCameraSpeed * normalize(side));
        }

        glm::mat4 view = Matrix_Camera_View(cam.getPosition(), cam.getViewVec(), cam.getUpVec());
        glm::mat4 projection;

        if (g_UsePerspectiveProjection)
        {
            float field_of_view = 3.141592 / 3.0f;
            projection = Matrix_Perspective(field_of_view, g_ScreenRatio, cam.getNearPlane(), cam.getFarPlane());
        }
        else
        {
            float t = 1.5f * g_CameraDistance / 2.5f;
            float b = -t;
            float r = t * g_ScreenRatio;
            float l = -r;
            projection = Matrix_Orthographic(l, r, b, t, cam.getNearPlane(), cam.getFarPlane());
        }

        glm::mat4 model = Matrix_Identity();

        glUniformMatrix4fv(g_view_uniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(g_projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));

#define PLANE 2

        /* TESTING */

        float planesT[3][3] = {
            {0.0f, -1.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}};

        float planesR[3][3] = {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, -1.5708f},
            {0.0f, 0.0f, 1.5708f}};

        for (int i = 0; i < 3; i++)
        {
            model = Matrix_Translate(planesT[i][0], planesT[i][1], planesT[i][2]) * Matrix_Rotate_Z(planesR[i][2]);
            glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(g_object_id_uniform, PLANE);
            DrawVirtualObject("the_plane");
        }

        glm::vec4 view_dir = glm::normalize(cam.getViewVec());
        glm::vec4 cam_pos = cam.getPosition();
        side = normalize(crossproduct(cam.getUpVec(), view_dir));

        TextRendering_ShowFramesPerSecond(window);

        glfwSwapBuffers(window);

        glfwPollEvents();
        Sleep(10);
    }

    glfwTerminate();

    return 0;
}

// set makeprg=cd\ ..\ &&\ make\ run\ >/dev/null
// vim: set spell spelllang=pt_br :