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
#include <BezierCurve.h>
#include <chrono>
#include <complex>
#include <ctime>
#include <EntityController.h>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
#include "ProjectileController.h"
#include "Collisions.h"
#include "PowerupController.h"

#define MAX_TORCHLIGHTS 100

using namespace std;

int main(int argc, char *argv[])
{
    std::srand(std::time(0));

    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // e
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
    LoadTextureImage("../../data/wall-tile1.png");        // TextureImage0
    LoadTextureImage("../../data/wall-tile2.png");        // TextureImage1
    LoadTextureImage("../../data/skin_txt.jpg");          // TextureImage2
    LoadTextureImage("../../data/water.png");             // TextureImage3
    LoadTextureImage("../../data/health_potion_txt.jpg"); // TextureImage4
    LoadTextureImage("../../data/portal.png");            // TextureImage5
    LoadTextureImage("../../data/bar-health.png");        // TextureImage6
    LoadTextureImage("../../data/bar-mana.png");          // TextureImage7

    /* BUILDING OBJECTS */
    ObjModel planemodel("../../data/plane.obj");
    ComputeNormals(&planemodel);
    BuildTrianglesAndAddToVirtualScene(&planemodel);

    ObjModel projectilemodel("../../data/sphere.obj");
    ComputeNormals(&projectilemodel);
    BuildTrianglesAndAddToVirtualScene(&projectilemodel);

    ObjModel right_arm("../../data/right_arm.obj");
    ComputeNormals(&right_arm);
    BuildTrianglesAndAddToVirtualScene(&right_arm);

    ObjModel left_arm("../../data/left_arm.obj");
    ComputeNormals(&left_arm);
    BuildTrianglesAndAddToVirtualScene(&left_arm);

    ObjModel left_arm_casting("../../data/left_arm_casting.obj");
    ComputeNormals(&left_arm_casting);
    BuildTrianglesAndAddToVirtualScene(&left_arm_casting);

    ObjModel enemy_1("../../data/bunny.obj");
    ComputeNormals(&enemy_1);
    BuildTrianglesAndAddToVirtualScene(&enemy_1);

    ObjModel potion_health("../../data/potion_health.obj");
    ComputeNormals(&potion_health);
    BuildTrianglesAndAddToVirtualScene(&potion_health);

    uint16_t currentLevel = 1;

    /* INITIALIZING ENTITIES */
    auto projectiles = ProjectileController();
    auto player = Player();
    auto level = Level(currentLevel);
    auto enemies = EntityController(level);
    auto power_ups = PowerupController();

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

        bool gameplayIsActive = true;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(g_GpuProgramID);

        /* Set camera mode */
        glm::vec4 view_vector;
        glm::vec4 camera_position;
        if (cam.GetMode() == CAMERA_FIRST_PERSON)
        {
            view_vector = cam.getViewVec();
            gameplayIsActive = true;
        }
        else if (cam.GetMode() == CAMERA_LOOK_AT)
        {
            glm::vec4 center = level.GetCenterWorldPosition();
            view_vector = -glm::vec4(mapCameraDistance * cam.getViewVec());
            camera_position = center - view_vector;
            gameplayIsActive = false;
        }

        /* Gameplay macros */
        if (gameplayIsActive)
        {
            // Calculate free camera movements
            glm::vec4 movementDirection = cam.getViewVec();
            movementDirection.y = 0.0f; // Fixate vertical movement
            movementDirection = normalize(movementDirection);
            if (isKeyDown_W)
            {
                player.move(timeDelta, movementDirection, level);
            }
            if (isKeyDown_S)
            {
                player.move(timeDelta, -movementDirection, level);
            }
            if (isKeyDown_A)
            {
                player.move(timeDelta, -cam.getSideVec(), level);
            }
            if (isKeyDown_D)
            {
                player.move(timeDelta, cam.getSideVec(), level);
            }
            if (g_LeftMouseButtonPressed)
            {
                if (player.getMana() >= 5)
                {
                    if (!projectiles.onCooldown(PLAYER_ID, now))
                    {
                        projectiles.shoot(
                            0,
                            PLAYER_ID,
                            0.15f,
                            cam.getPosition() - 0.38f * cam.getSideVec() + 0.6f * cam.getViewVec() - 0.10f * cam.getUpVec(),
                            cam.getViewVec(),
                            20.0f, -40.0f,
                            5.0f,
                            0.5f,
                            now,
                            glm::vec3(0.2f, 0.2f, 0.2f));
                        player.setMana(player.getMana() - 5);
                    }
                }
            }
        }

        /* Debugging macros */
        // If "H" key was pressed: log players current position on console (for debugging)
        if (debugLogPlayerPosition)
        {
            glm::vec4 ppos = player.getPosition();
            int mposx = level.WorldPositionToMapPositionX(ppos.x);
            int mposz = level.WorldPositionToMapPositionZ(ppos.z);
            printf("World Position: (%.3f, %.3f, %.3f)\t Map Position (%d, %d)\n",
                   ppos.x, ppos.y, ppos.z, mposx, mposz);
            debugLogPlayerPosition = false;
        }
        // If "F5" key was pressed: Reload GLSL shaders
        if (reloadShaders)
        {
            LoadShadersFromFiles();
            printf("Shaders Reloaded.\n");
            reloadShaders = false;
        }

        // Level progression
        if (level.ObjectiveReached(player.getPosition()))
        {
            currentLevel++;
            player = Player();
            level = Level(currentLevel);
            enemies = EntityController(level);
        }

        /* Step game entities */
        if (gameplayIsActive)
        {
            cam.setPosition(player.getPosition());
            camera_position = cam.getPosition();
            projectiles.step(now, timeDelta);
            player.update(timeDelta);
            enemies.step(now, timeDelta, &player, &projectiles);
            power_ups.step(&player, timeDelta);
        }

        /* check collisions */

        // player projetiles with enemies
        Projectile *proj = projectiles.getProjectiles();
        GameEntity *enem = enemies.getEntities();
        for (int i = 0; i < projectiles.getSize(); i++)
        {
            if (proj[i].status && proj[i].shooter_id == PLAYER_ID)
            {
                for (int j = 0; j < MAX_ENTITIES; j++)
                {
                    if (enem[j].status)
                    {
                        if (isColliding(proj[i].bbox, enem[j].bbox))
                        {
                            proj[i].status = false;
                            enem[j].health -= proj[i].damage;
                            if (enem[j].health <= 0)
                            {
                                power_ups.spawn(
                                    enem[j].position,
                                    20.0f,
                                    50.0f,
                                    0);
                                enem[j].status = false;
                            }
                        }
                    }
                }
            }
        }

        // enemy projectiles with player
        for (int i = 0; i < projectiles.getSize(); i++)
        {
            if (proj[i].status && proj[i].shooter_id != PLAYER_ID)
            {
                if (player.GetCollision().IsCollidingAABB(proj[i].bbox))
                {
                    player.damage(proj[i].damage);
                    proj[i].status = false;
                }
            }
        }

        glm::mat4 view = Matrix_Camera_View(camera_position, view_vector, cam.getUpVec());
        glm::mat4 projection;
        float field_of_view = 3.141592 / 3.0f;
        projection = Matrix_Perspective(field_of_view, g_ScreenRatio, cam.getNearPlane(), cam.getFarPlane());

        glm::mat4 model = Matrix_Identity();
        glUniformMatrix4fv(g_view_uniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(g_projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));

        // Enable texture transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        /* Draw Map*/
        // Draw map planes
        glm::vec4 torchlight_position[MAX_TORCHLIGHTS] = {};
        glm::vec3 torchlight_color[MAX_TORCHLIGHTS] = {};
        uint16_t torchlight_count = 0;
        for (int i = 0; i < level.GetMapHeight(); i++)
        {
            for (int j = 0; j < level.GetMapWidth(); j++)
            {
                // Transform and draw planes for tile (j, i)
                for (const Plane &plane : level.GetPlanesAtTile(j, i))
                {
                    model = Matrix_Translate(plane.position.x, plane.position.y, plane.position.z);
                    model *= Matrix_Rotate_X(glm::radians(plane.rotation.x));
                    model *= Matrix_Rotate_Y(glm::radians(plane.rotation.y));
                    model *= Matrix_Rotate_Z(glm::radians(plane.rotation.z));
                    DrawObjectModel(model, plane.texture, "the_plane");

                    if (plane.hasTorch && torchlight_count < MAX_TORCHLIGHTS)
                    {
                        // Calculate torch light information
                        glm::vec4 tpos = glm::vec4(plane.position, 1.0f) + plane.normal * 0.1f;
                        torchlight_position[torchlight_count] = tpos;
                        torchlight_color[torchlight_count] = plane.torchColor;
                        torchlight_count++;

                        // Draw torch model
                        model = Matrix_Translate(tpos.x, tpos.y, tpos.z) * Matrix_Scale(0.07, 0.07, 0.07);
                        DrawObjectModel(model, TORCH, "the_sphere");
                    }
                }
            }
        }
        // Send torch light information to shader
        glUniform4fv(g_torchlight_position_uniform, MAX_TORCHLIGHTS, glm::value_ptr(torchlight_position[0]));
        glUniform3fv(g_torchlight_color_uniform, MAX_TORCHLIGHTS, glm::value_ptr(torchlight_color[0]));
        glUniform1i(g_torchlight_count_uniform, torchlight_count);

        // Draw objective portal, using billboarding
        glm::vec4 portalPosition = level.GetObjectivePosition();
        glm::vec4 portalNormal = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
        glm::vec4 portalDirection = glm::normalize(cam.getPosition() - portalPosition);
        float rotationAngle = glm::acos(glm::dot(portalNormal, portalDirection));

        glm::vec4 crossProduct = crossproduct(portalNormal, portalDirection);
        glm::vec4 upVector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

        if (glm::dot(crossProduct, upVector) < 0.0f)
        {
            rotationAngle = 2.0f * glm::pi<float>() - rotationAngle;
        }

        model = Matrix_Translate(portalPosition.x, 0.0f, portalPosition.z);
        model *= Matrix_Rotate_Y(rotationAngle);
        model *= Matrix_Rotate_X(glm::radians(90.0f));
        model *= Matrix_Scale(0.65625f, 1.0f, 1.0f);
        DrawObjectModel(model, OBJECTIVE_PORTAL, "the_plane");

        /* Draw Entities */
        // Draw projectiles
        glm::vec4 projectile_position[MAX_PROJECTILES] = {};
        uint16_t projectile_count = 0;
        glBindVertexArray(g_VirtualScene["the_sphere"].vertex_array_object_id);
        glUniform1i(g_object_id_uniform, PROJECTILE_WATER);
        for (int i = 0; i < projectiles.getSize(); i++)
        {
            model = Matrix_Translate(proj[i].position.x, proj[i].position.y, proj[i].position.z) * Matrix_Scale(0.07, 0.07, 0.07);
            glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            if (proj[i].status)
                model = Matrix_Translate(proj[i].position.x, proj[i].position.y, proj[i].position.z) * Matrix_Scale(0.07, 0.07, 0.07);
            if (proj[i].status)
            {
                // Draw projectile's lighting
                projectile_position[projectile_count] = proj[i].position;
                projectile_count++;

                // Draw projectile
                DrawObjectModel(model, PROJECTILE_WATER, "the_sphere");
            }
        }
        glUniform4fv(g_waterproj_position_uniform, MAX_PROJECTILES, glm::value_ptr(projectile_position[0]));
        glUniform1i(g_waterproj_count_uniform, projectile_count);

        // draw enemies
        GameEntity *enms = enemies.getEntities();
        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            if (enms[i].status)
            {
                model =
                    Matrix_Translate(enms[i].position.x, enms[i].position.y, enms[i].position.z) *
                    Matrix_Scale(0.2, 0.2, 0.2) *
                    Matrix_Rotate_Y(3.14f - angleAroundY(enms[i].direction));
                DrawObjectModel(model, ENEMY_TYPE_1, "the_bunny");
            }
        }

        // draw powerups
        Powerup *pwrs = power_ups.getPowerUps();
        glm::vec4 powerup_pos;
        for (int i = 0; i < MAX_POWERUPS; i++)
        {
            if (pwrs[i].isActive)
            {
                powerup_pos = pwrs[i].trajectory.calcTrajectory(pwrs[i].step);
                model =
                    Matrix_Translate(powerup_pos.x, powerup_pos.y, powerup_pos.z) *
                    Matrix_Scale(0.2, 0.2, 0.2);
                DrawObjectModel(model, POTION_HEALTH, "potion_health");
            }
        }

        // First person mode rendering phase (arms and HUD)
        if (cam.GetMode() == CAMERA_FIRST_PERSON)
        {
            // Draw enemy health bars
            for (int i = 0; i < MAX_ENTITIES; i++)
            {
                glm::vec4 barPosition = enms[i].position + glm::vec4(0.0f, 0.5f, 0.0f, 0.0f);
                glm::vec4 barNormal = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
                glm::vec4 barDirection = glm::normalize(cam.getPosition() - barPosition);
                float rotationAngleY = glm::acos(glm::dot(barNormal, barDirection));
                glm::vec4 crossProduct = crossproduct(barNormal, barDirection);

                upVector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

                if (glm::dot(crossProduct, upVector) < 0.0f)
                    rotationAngle = 2.0f * glm::pi<float>() - rotationAngle;

                model = Matrix_Translate(barPosition.x, barPosition.y, barPosition.z);
                model *= Matrix_Rotate_Y(rotationAngle);
                model *= Matrix_Rotate_X(glm::radians(90.0f));
                model *= Matrix_Scale(enms[i].health / enms[i].maxhealth * 0.3, 0.03f, 0.03f);
                DrawObjectModel(model, HUD_HEALTH_BAR, "the_plane");
            }

            // Draw right arm
            glm::vec4 arm_pos = cam.getPosition() + 0.25f * cam.getSideVec() + 0.3f * cam.getViewVec() - 0.15f * cam.getUpVec();
            if (isKeyDown_W || isKeyDown_A || isKeyDown_S || isKeyDown_D)
            {
                arm_pos.y += 0.02f * cos(4 * now);
            }
            else
            {
                arm_pos.y += 0.02f * cos(now);
            }
            model = Matrix_Translate(arm_pos.x, arm_pos.y, arm_pos.z) * Matrix_Scale(0.01f, 0.01f, 0.01f) *
                    Matrix_Rotate(cam.getPhi(), cam.getSideVec()) *
                    Matrix_Rotate(3.0f, cam.getSideVec()) *
                    Matrix_Rotate(cam.getTheta(), cam.getUpVec());
            DrawObjectModel(model, RIGHT_ARM, "right_arm");

            // Draw left arm
            arm_pos = cam.getPosition() - 0.25f * cam.getSideVec() + 0.3f * cam.getViewVec() - 0.15f * cam.getUpVec();
            if (isKeyDown_W || isKeyDown_A || isKeyDown_S || isKeyDown_D)
            {
                arm_pos.y += 0.02f * cos(4 * now);
            }
            else
            {
                arm_pos.y += 0.02f * cos(now);
            }
            model = Matrix_Translate(arm_pos.x, arm_pos.y, arm_pos.z) * Matrix_Scale(0.01f, 0.01f, 0.01f) *
                    Matrix_Rotate(-cam.getPhi(), cam.getSideVec()) *
                    Matrix_Rotate(3.0f, cam.getSideVec()) *
                    Matrix_Rotate(3.0f, cam.getViewVec()) *
                    Matrix_Rotate(cam.getTheta(), cam.getUpVec());
            if (g_LeftMouseButtonPressed)
            {
                DrawObjectModel(model, LEFT_ARM, "left_arm_casting");
            }
            else
            {
                DrawObjectModel(model, LEFT_ARM, "left_arm");
            }

            // Draw HUD
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glUniformMatrix4fv(g_view_uniform, 1, GL_FALSE, glm::value_ptr(Matrix_Identity()));
            glUniformMatrix4fv(g_projection_uniform, 1, GL_FALSE, glm::value_ptr(Matrix_Identity()));

            DrawHUD(window, player);
        }
        // Map viewing mode rendering phase
        else if (cam.GetMode() == CAMERA_LOOK_AT)
        {
            glm::vec4 playerPosition = player.getPosition();
            model = Matrix_Scale(0.5f, 0.5f, 0.5f);
            model *= Matrix_Translate(playerPosition.x, playerPosition.y, playerPosition.z);
            DrawObjectModel(model, TORCH, "the_sphere");
        }

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