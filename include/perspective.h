#ifndef __PERSPECTIVE_H__
#define __PERSPECTIVE_H__

#include "Eigen/Dense"

struct Perspective {
  float near;
  float far;
  float left;
  float right;
  float top;
  float bottom;

  Perspective();
  Perspective(float n, float f, float l, float r, float t, float b);
  Eigen::Matrix4d matrix();

  void print();
};

#endif
