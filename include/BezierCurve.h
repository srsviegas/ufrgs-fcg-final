//
// Created by walte on 16/08/2024.
//

#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <glm/vec4.hpp>

class BezierCurve{
public:

   BezierCurve();
   BezierCurve(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4);
   glm::vec4 calcTrajectory(float t);

   glm::vec4 getP1() const;
   void setP1(glm::vec4 p1);
   glm::vec4 getP2() const;
   void setP2(glm::vec4 p2);
   glm::vec4 getP3() const;
   void setP3(glm::vec4 p3);
   glm::vec4 getP4() const;
   void setP4(glm::vec4 p4);

private:

   glm::vec4 p1,p2,p3,p4;

};

#endif //BEZIERCURVE_H

