//
// Created by walte on 16/08/2024.
//

#include "BezierCurve.h"

#include <glm/exponential.hpp>


BezierCurve::BezierCurve(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4) {
    this->p1 = p1; this->p2 = p2; this->p3 = p3; this->p4 = p4;
}

BezierCurve::BezierCurve() {
    p1 = glm::vec4(0.0f,0.0f,0.0f,0.0f);
    p2 = glm::vec4(0.0f,0.0f,0.0f,0.0f);
    p3 = glm::vec4(0.0f,0.0f,0.0f,0.0f);
    p4 = glm::vec4(0.0f,0.0f,0.0f,0.0f);
}

void BezierCurve::setP1(glm::vec4 p1) {
    this->p1 = p1;
}
void BezierCurve::setP2(glm::vec4 p2) {
    this->p2 = p2;
}
void BezierCurve::setP3(glm::vec4 p3) {
    this->p3 = p3;
}
void BezierCurve::setP4(glm::vec4 p4) {
    this->p4 = p4;
}


glm::vec4 BezierCurve::calcTrajectory(float t) {

    //polinomios de Bernstein
    float b03 = glm::pow(1-t,3);
    float b13 = 3.0f * t * glm::pow(1-t,2);
    float b23 = 3.0f * glm::pow(t,2) * (1-t);
    float b33 = glm::pow(t,3);
    return b03 * p1 + b13 * p2 + b23 * p3 + b33 * p4;
}

