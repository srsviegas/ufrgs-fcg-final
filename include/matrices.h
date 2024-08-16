#ifndef _MATRICES_H
#define _MATRICES_H

#include <cstdio>
#include <cstdlib>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Matrix creation helper, assuming row-major input and converting to GLM's column-major
glm::mat4 Matrix(
    float m00, float m01, float m02, float m03, // ROW 1
    float m10, float m11, float m12, float m13, // ROW 2
    float m20, float m21, float m22, float m23, // ROW 3
    float m30, float m31, float m32, float m33
    // ROW 4
);

// Identity matrix
glm::mat4 Matrix_Identity();



// Translation matrix
glm::mat4 Matrix_Translate(float tx, float ty, float tz);

// Scaling matrix
glm::mat4 Matrix_Scale(float sx, float sy, float sz);

// Rotation matrices around X, Y, Z axes
glm::mat4 Matrix_Rotate_X(float angle);
glm::mat4 Matrix_Rotate_Y(float angle);
glm::mat4 Matrix_Rotate_Z(float angle);

// Generic rotation matrix around an arbitrary axis (normalized)
glm::mat4 Matrix_Rotate(float angle, glm::vec4 axis);

// View matrix for camera positioning
glm::mat4 Matrix_Camera_View(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector);

// Orthographic projection matrix
glm::mat4 Matrix_Orthographic(float l, float r, float b, float t, float n, float f);

// Perspective projection matrix
glm::mat4 Matrix_Perspective(float field_of_view, float aspect, float n, float f);

// LookAt matrix for camera positioning (alternative to Matrix_Camera_View)
glm::mat4 lookAt(glm::vec4 position, glm::vec4 target, glm::vec4 up);

// Helper functions (could be in a separate utility header if needed)

// Euclidean norm of a 3D vector
float norm(glm::vec4 v);

// Cross product of two 3D vectors
glm::vec4 crossproduct(glm::vec4 u, glm::vec4 v);

float angleAroundY(glm::vec4 v);

// Dot product of two vectors (error if used on points with w != 0)
float dotproduct(glm::vec4 u, glm::vec4 v);

// Printing functions for debugging (remove in release builds)
void PrintMatrix(glm::mat4 M);
void PrintVector(glm::vec4 v);
void PrintMatrixVectorProduct(glm::mat4 M, glm::vec4 v);
void PrintMatrixVectorProductDivW(glm::mat4 M, glm::vec4 v);

#endif // _MATRICES_H
// vim: set spell spelllang=pt_br :
