//
// Created by walte on 16/08/2024.
//

#include "BezierCurve.h"

#include <glm/exponential.hpp>


BezierCurve::BezierCurve(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4) {
    this->points[0] = p1; this->points[1] = p2; this->points[2] = p3; this->points[3] = p4;
}

glm::vec4 BezierCurve::calcTrajectory(float t) {

    //polinomios de Bernstein
    float b03 = glm::pow(1-t,3);
    float b13 = 3.0f * t * glm::pow(1-t,2);
    float b23 = 3.0f * glm::pow(1-t,2) * (1-t);
    float b33 = glm::pow(t,3);
    return b03 * points[0] + b13 * points[1] + b23 * points[2] + b33 * points[3];
}

