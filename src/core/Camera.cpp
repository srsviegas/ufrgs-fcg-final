#include "Camera.h"

Camera::Camera()
{
    cam_pos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    cam_up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    near_plane = -0.1f;
    far_plane = -50.0f;
    g_Theta = 3.141592f / 4;
    g_Phi = 3.141592f / 6;
    lookat = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    mode = CAMERA_FIRST_PERSON;
}

Camera::Camera(glm::vec4 start_pos, glm::vec4 up_vec, float near_val, float far_val)
{
    if (near_val > 0)
    {
        near_plane = -near_val;
    }
    else
    {
        near_plane = near_val;
    }
    if (far_val > 0)
    {
        far_plane = -far_val;
    }
    else
    {
        far_plane = far_val;
    }
    cam_up = up_vec;
    cam_pos = start_pos;
}

void Camera::setPosition(glm::vec4 new_pos)
{
    cam_pos = new_pos;
}

glm::vec4 Camera::getPerpendicular() const
{
    return glm::normalize(crossproduct(getSideVec(), getViewVec()));
}

void Camera::setUpVector(glm::vec4 up_vec)
{
    cam_up = up_vec;
}

void Camera::setNearPlane(float near_val)
{
    near_plane = near_val;
}

void Camera::setFarPlane(float far_val)
{
    far_plane = far_val;
}

void Camera::_move(glm::vec4 displacement)
{
    cam_pos = cam_pos + displacement;
}

glm::vec4 Camera::getPosition() const
{
    return cam_pos;
}

float Camera::getNearPlane() const
{
    return near_plane;
}

float Camera::getFarPlane() const
{
    return far_plane;
}

glm::vec4 Camera::getViewVec() const
{
    return glm::vec4(glm::cos(g_Phi) * glm::sin(g_Theta),
                     glm::sin(g_Phi),
                     glm::cos(g_Phi) * glm::cos(g_Theta),
                     0.0f);
}

glm::vec4 Camera::getSideVec() const
{
    return glm::normalize(crossproduct(getViewVec(), cam_up));
}

glm::vec4 Camera::getUpVec() const
{
    return cam_up;
}

void Camera::setTheta(float angle)
{
    g_Theta = angle;
}

void Camera::setPhi(float angle)
{
    g_Phi = angle;
}

float Camera::getPhi()
{
    return g_Phi;
}

float Camera::getTheta()
{
    return g_Theta;
}

glm::mat4 Camera::calcViewMatrix()
{
    return Matrix_Camera_View(cam_pos, glm::vec4(glm::cos(g_Phi) * glm::sin(g_Theta), glm::sin(g_Phi), glm::cos(g_Phi) * glm::cos(g_Theta), 0.0f), cam_up);
}

void Camera::SetLookAt(glm::vec4 point)
{
    if (point.w == 0.0f)
    {
        printf("WARNING: Look-at should be a point, not a vector.\n");
        point.w = 1.0f;
    }
    lookat = point;
}

glm::vec4 Camera::GetLookAt()
{
    return lookat;
}

int Camera::GetMode()
{
    return mode;
}

void Camera::SwitchMode()
{
    float loadedPhi = savedRotation.x;
    float loadedTheta = savedRotation.y;

    savedRotation = glm::vec2(getPhi(), getTheta());

    setPhi(loadedPhi);
    setTheta(loadedTheta);

    if (GetMode() == CAMERA_FIRST_PERSON)
    {
        mode = CAMERA_LOOK_AT;
    }
    else
    {
        mode = CAMERA_FIRST_PERSON;
    }
}