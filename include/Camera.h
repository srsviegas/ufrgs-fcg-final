#ifndef Camera_H
#define Camera_H

#include <vector>
#include <string.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "matrices.h"
#include "Level.h"

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
    glm::vec4 getSideVec() const;
    float getNearPlane() const;
    float getFarPlane() const;

    void SetLookAt(glm::vec4 point);
    glm::vec4 GetLookAt();

    int GetMode();
    void SwitchMode();

private:
    float g_Theta;
    float g_Phi;
    glm::vec4 cam_up;
    glm::vec4 cam_pos;
    float near_plane;
    float far_plane;
    float fov;
    glm::vec4 lookat;
    int mode;
};

#endif
