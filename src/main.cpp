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
#include "PointLight.h"
#include "Projectile.h"
#include "GameEntity.h"

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

    ObjModel projectilemodel("../../data/sphere.obj");
    ComputeNormals(&projectilemodel);
    BuildTrianglesAndAddToVirtualScene(&projectilemodel);

    /* initializing entities */

    cam.setFarPlane(-20.0f);
    // juntar tudo numa classe posteriormente
    auto player = Player(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    Projectile projectiles[NUM_PROJECTILES] = {};
    for (int i = 0; i < NUM_PROJECTILES; i++)
    {
        projectiles[i] = Projectile();
    }
    int last_shot = 0;
    float last_shot_time = 0;

    GameEntity test_enemy(glm::vec4(2.0f, 0.0f, 0.0f, 1.0f), 0, 100, 1, 5);

    if (argc > 1)
    {
        ObjModel model(argv[1]);
        BuildTrianglesAndAddToVirtualScene(&model);
    }

    GLuint vertex_array_object_id = DrawHealthHUD();

    PointLight projectile_lights[NUM_PROJECTILES] = {};
    for (int i = 0; i < NUM_PROJECTILES; i++)
    {
        projectile_lights[i] = PointLight();
    }

    TextRendering_Init();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    Level currentLevel = Level(mapData1, 5);

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
        movementDirection = normalize(movementDirection);
        if (isKeyDown_W)
        {
            player.move(timeDelta, movementDirection, currentLevel);
        }
        if (isKeyDown_S)
        {
            player.move(timeDelta, -movementDirection, currentLevel);
        }
        if (isKeyDown_A)
        {
            player.move(timeDelta, -cam.getSideVec(), currentLevel);
        }
        if (isKeyDown_D)
        {
            player.move(timeDelta, cam.getSideVec(), currentLevel);
        }
        if (g_LeftMouseButtonPressed && now - last_shot_time > 0.1)
        {

            if (player.getMana() >= 5)
            {
                projectiles[last_shot % NUM_PROJECTILES].shoot(
                    cam.getPosition() - 0.2f * cam.getSideVec() + 0.3f * cam.getViewVec() - 0.1f * cam.getUpVec(),
                    cam.getViewVec(),
                    0.0f, 20.0f, 0.5f,
                    now);
                last_shot++;
                player.setMana(player.getMana() - 5);
                last_shot_time = now;
            }
        }
        cam.setPosition(player.getPosition());

        /* step game entities */
        for (auto & projectile : projectiles)
        {
            if (projectile.isActive())
            {
                projectile.step(timeDelta);
                if (now - projectile.getStartTime() > projectile.getLifeTime())
                {
                    projectile.deactivate();
                }
            }
        }

        player.update(timeDelta);

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

        float lights[NUM_PROJECTILES * 7] = {};
        for (int i = 0; i < NUM_PROJECTILES; i++)
        {
            lights[i * 7 + 0] = projectile_lights[i].getPosition().x;
            lights[i * 7 + 1] = projectile_lights[i].getPosition().y;
            lights[i * 7 + 2] = projectile_lights[i].getPosition().z;
            lights[i * 7 + 3] = projectile_lights[i].getColor().x;
            lights[i * 7 + 4] = projectile_lights[i].getColor().y;
            lights[i * 7 + 5] = projectile_lights[i].getColor().z;
            lights[i * 7 + 6] = projectile_lights[i].getIntensity();
        }

        glUniform4fv(g_ligths_uniform, 1, lights);

        for (int i = 0; i < currentLevel.GetMapHeight(); i++)
        {
            for (int j = 0; j < currentLevel.GetMapWidth(); j++)
            {
                for (const Plane &plane : currentLevel.GetPlanesAtTile(j, i))
                {
                    float tx = plane.position[0];
                    float ty = plane.position[1];
                    float tz = plane.position[2];
                    model = Matrix_Translate(tx, ty, tz) * Matrix_Rotate_X(glm::radians(plane.rotation[0])) * Matrix_Rotate_Y(glm::radians(plane.rotation[1])) * Matrix_Rotate_Z(glm::radians(plane.rotation[2]));
                    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
                    glUniform1i(g_object_id_uniform, plane.texture);
                    DrawVirtualObject("the_plane");
                }
            }
        }

        glBindVertexArray(g_VirtualScene["the_sphere"].vertex_array_object_id);
        for (int i = 0; i < NUM_PROJECTILES; i++)
        {
            model = Matrix_Translate(projectiles[i].getPosition().x, projectiles[i].getPosition().y, projectiles[i].getPosition().z) * Matrix_Scale(0.05, 0.05, 0.05);
            glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(g_object_id_uniform, SPHERE);
            if (projectiles[i].isActive())
            {
                glDrawElements(g_VirtualScene["the_sphere"].rendering_mode, g_VirtualScene["the_sphere"].num_indices, GL_UNSIGNED_INT, (void *)(g_VirtualScene["the_sphere"].first_index * sizeof(GLuint)));
            }
        }

        model = Matrix_Translate(test_enemy.get_position().x, test_enemy.get_position().y, test_enemy.get_position().z) * Matrix_Scale(0.5, 0.5, 0.5);
        glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, ENEMY_TYPE_1);
        glDrawElements(g_VirtualScene["the_sphere"].rendering_mode, g_VirtualScene["the_sphere"].num_indices, GL_UNSIGNED_INT, (void *)(g_VirtualScene["the_sphere"].first_index * sizeof(GLuint)));

        /* renderizacao da HUD */
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glUniformMatrix4fv(g_view_uniform, 1, GL_FALSE, glm::value_ptr(Matrix_Identity()));
        glUniformMatrix4fv(g_projection_uniform, 1, GL_FALSE, glm::value_ptr(Matrix_Identity()));

        // Health bar
        model = Matrix_Translate(0.0f, -0.9f, 0.0f) * Matrix_Scale(player.getHealthPercent(), 1.0f, 1.0f);
        glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, HUD_HEALTH);
        glBindVertexArray(vertex_array_object_id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

        // Mana bar
        model = Matrix_Translate(-0.7f, -0.9f, 0.0f) * Matrix_Scale(player.getManaPercent(), 1.0f, 1.0f);
        glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, HUD_MANA);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

        glEnable(GL_DEPTH_TEST);
        TextRendering_ShowFramesPerSecond(window);
        glfwSwapBuffers(window);
        glEnable(GL_CULL_FACE);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

// set makeprg=cd\ ..\ &&\ make\ run\ >/dev/null
// vim: set spell spelllang=pt_br :