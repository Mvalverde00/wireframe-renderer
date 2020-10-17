#ifndef __PERSPECTIVE_H__
#define __PERSPECTIVE_H__

#include "Eigen/Dense"


/*
 * A Camera Perspective
 */
struct Perspective {
  double near;
  double far;
  double left;
  double right;
  double top;
  double bottom;

  Perspective();
  Perspective(double n, double f, double l, double r, double t, double b);
  Eigen::Matrix4d matrix();

  void print();
};

#endif
