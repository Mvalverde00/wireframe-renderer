#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <fstream>

#include "Eigen/Dense"
#include "perspective.h"

class Camera {

  Eigen::Matrix4d m;
  Perspective perspective;
  Eigen::Vector3d pos;


public:
  Camera();
  Camera(std::ifstream& stream);
  void translate(float x, float y, float z);
  void rotate(float ux, float uy, float uz, float ang);
  void print();

  Eigen::Matrix4d getMatrix();
  Eigen::Matrix4d getInverse();

  Eigen::Matrix4d getPMatrix();


};

#endif
