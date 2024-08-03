#ifndef Camera_H
#define Camera_H

#include <string.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "matrices.h"

class Camera
{
    public:
        Camera();
        Camera(glm::vec4 start_pos, glm::vec4 up_vec, float near_val, float far_val);
        void setPosition(glm::vec4 new_pos);
        void setPhi(float angle);
        void setTheta(float angle);
        void setUpVector(glm::vec4 up_vec);
        void setNearPlane(float near_dist);
        void setFarPlane(float far_dist);
        void _move(glm::vec4 displacement);
        glm::mat4 calcViewMatrix();
        float getPhi();
        float getTheta();
        glm::vec4 getPosition() const;
        glm::vec4 getViewVec() const;
        glm::vec4 getUpVec() const;
        float getNearPlane() const;
        float getFarPlane() const;

    private:
        float g_Theta;
        float g_Phi;
        glm::vec4 cam_up;
        glm::vec4 cam_pos;
        float near_plane;
        float far_plane;
        float fov;
};

Camera::Camera(){
    cam_pos = glm::vec4(0.0f,0.0f,0.0f,1.0f);
    cam_up = glm::vec4(0.0f,1.0f,0.0f,0.0f);
    near_plane = -0.1f;
    far_plane = -7.5f;
    g_Theta = 3.141592f / 4;
    g_Phi = 3.141592f / 6;
}

Camera::Camera(glm::vec4 start_pos, glm::vec4 up_vec, float near_val, float far_val){
    if(near_val > 0){
        near_plane = -near_val;
    } else {near_plane = near_val;}
    if(far_val > 0){
        far_plane = -far_val;
    } else {far_plane = far_val;}
    cam_up = up_vec;
    cam_pos = start_pos;

}

void Camera::setPosition(glm::vec4 new_pos){
    cam_pos = new_pos;
}

void Camera::setUpVector(glm::vec4 up_vec){
    cam_up = up_vec;
}

void Camera::setNearPlane(float near_val){
    near_plane = near_val;
}

void Camera::setFarPlane(float far_val){
    far_plane = far_val;
}

void Camera::_move(glm::vec4 displacement){
    cam_pos = cam_pos + displacement;
}

inline glm::vec4 Camera::getPosition() const{
    return cam_pos;
}

inline float Camera::getNearPlane() const{
    return near_plane;
}

inline float Camera::getFarPlane() const{
    return far_plane;
}

inline glm::vec4 Camera::getViewVec() const{
    return glm::vec4(glm::cos(g_Phi) * glm::sin(g_Theta),
                     glm::sin(g_Phi),
                     glm::cos(g_Phi) * glm::cos(g_Theta),
                     0.0f);
}

inline glm::vec4 Camera::getUpVec() const{
    return cam_up;
}

void Camera::setTheta(float angle){
    g_Theta = angle;
}

void Camera::setPhi(float angle){
    g_Phi = angle;
}

float Camera::getPhi(){
    return g_Phi;
}

float Camera::getTheta(){
    return g_Theta;
}

glm::mat4 Camera::calcViewMatrix(){
    return Matrix_Camera_View(cam_pos, glm::vec4(glm::cos(g_Phi) * glm::sin(g_Theta),
                     glm::sin(g_Phi),
                     glm::cos(g_Phi) * glm::cos(g_Theta),
                     0.0f), cam_up);
}

#endif
