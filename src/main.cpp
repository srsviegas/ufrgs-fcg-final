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
#include "Level.h"
#include "Player.h"
#include "Projectile.h"

#define HUD_HEALTH 4

using namespace std;

int main(int argc, char *argv[])
{
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    float now;
    float last = 0;
    float timeDelta;

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
    LoadTextureImage("../../data/wall-tile1.png");
    LoadTextureImage("../../data/wall-tile2.png");

    /* BUILDING OBJECTS */
    ObjModel planemodel("../../data/plane.obj");
    ComputeNormals(&planemodel);
    BuildTrianglesAndAddToVirtualScene(&planemodel);

#define SPHERE 5
    ObjModel projectilemodel("../../data/sphere.obj");
    ComputeNormals(&projectilemodel);
    BuildTrianglesAndAddToVirtualScene(&projectilemodel);

    /* initializing entities */
    auto player = Player(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    Projectile projectile = Projectile();

    if (argc > 1)
    {
        ObjModel model(argv[1]);
        BuildTrianglesAndAddToVirtualScene(&model);
    }

    GLuint vertex_array_object_id = DrawHealthHUD();

    TextRendering_Init();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    std::vector<Plane> levelData = BuildLevelData(5, 6, mapData);

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
            player.move(timeDelta * player.getWalkspeed() * normalize(movementDirection));
        }
        if (isKeyDown_S)
        {
            player.move(timeDelta * -player.getWalkspeed() * normalize(movementDirection));
        }
        if (isKeyDown_A)
        {
            player.move(timeDelta * -player.getWalkspeed() * normalize(side));
        }
        if (isKeyDown_D)
        {
            player.move(timeDelta * player.getWalkspeed() * normalize(side));
        }
        if(g_LeftMouseButtonPressed) {
            projectile.shoot(cam.getPosition(),cam.getViewVec(),0.0f,20.0f,10.0f, now);
        }
        cam.setPosition(player.getPosition());

        /* step game entities */
        if(projectile.isActive()) {
            projectile.step(timeDelta);
            if(now - projectile.getStartTime() > projectile.getLifeTime()) {
                projectile.deactivate();
            }
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

        /* TESTING */

        for (const Plane &plane : levelData)
        {
            float tx = plane.position[0];
            float ty = plane.position[1];
            float tz = plane.position[2];
            model = Matrix_Translate(tx, ty, tz) * Matrix_Rotate_X(glm::radians(plane.rotation[0])) * Matrix_Rotate_Y(glm::radians(plane.rotation[1])) * Matrix_Rotate_Z(glm::radians(plane.rotation[2]));
            glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(g_object_id_uniform, plane.texture);
            DrawVirtualObject("the_plane");
        }

        model = Matrix_Translate(projectile.getPosition().x,projectile.getPosition().y,projectile.getPosition().z) * Matrix_Scale(0.1,0.1,0.1);
        glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, SPHERE);
        if(projectile.isActive()) {
            DrawVirtualObject("the_sphere");
        }


        /* renderizacao da HUD */
        glDisable(GL_DEPTH_TEST);

        glDisable(GL_CULL_FACE);
        glDisable(GL_FRAGMENT_SHADER);

        glUniformMatrix4fv(g_view_uniform, 1, GL_FALSE, glm::value_ptr(Matrix_Identity()));
        glUniformMatrix4fv(g_projection_uniform, 1, GL_FALSE, glm::value_ptr(Matrix_Identity()));
        model = Matrix_Translate(0.0, -0.9, 0.0) * Matrix_Scale(player.getHealthPercent(), 1.0f, 1.0f);
        glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, HUD_HEALTH);
        glBindVertexArray(vertex_array_object_id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_FRAGMENT_SHADER);

        TextRendering_ShowFramesPerSecond(window);

        glfwSwapBuffers(window);

        glEnable(GL_CULL_FACE);

        glfwPollEvents();
        Sleep(10);
    }

    glfwTerminate();

    return 0;
}

// set makeprg=cd\ ..\ &&\ make\ run\ >/dev/null
// vim: set spell spelllang=pt_br :