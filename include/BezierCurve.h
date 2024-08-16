//
// Created by walte on 16/08/2024.
//

#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <glm/vec4.hpp>

class BezierCurve{
public:
   BezierCurve(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4);
   glm::vec4 calcTrajectory(float t);
private:

   glm::vec4 points[4]{};

};

#endif //BEZIERCURVE_H

