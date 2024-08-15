#ifndef Input_H
#define Input_H

#include "Window.h"

extern float g_ScreenRatio;

extern const int screenWidth;
extern const int screenHeight;

extern float g_AngleX;
extern float g_AngleY;
extern float g_AngleZ;

extern bool g_LeftMouseButtonPressed;
extern bool g_RightMouseButtonPressed;
extern bool g_MiddleMouseButtonPressed;

extern float g_CameraDistance;

extern bool isKeyDown_W;
extern bool isKeyDown_A;
extern bool isKeyDown_S;
extern bool isKeyDown_D;

extern bool debugLogPlayerPosition;

void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
void ErrorCallback(int error, const char *description);
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

#endif