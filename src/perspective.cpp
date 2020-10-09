#include "perspective.h"

#include <stdio.h>

#include "Eigen/Dense"

Perspective::Perspective(float n, float f, float l, float r, float t, float b) {
  this->near = n;
  this->far = f;
  this->left = l;
  this->right = r;
  this->top = t;
  this->bottom = b;
}

Perspective::Perspective() : Perspective::Perspective(1.0, 10.0, -1.0, 1.0, 1.0, -1.0) {}

Eigen::Matrix4d Perspective::matrix() {
  // TODO: Make actual perspective matrix
  Eigen::Matrix4d m;
  m << 2*near / (right - left), 0, (right + left) / (right - left), 0,
      0, 2*near / (top - bottom), (top + bottom) / (top - bottom), 0,
      0, 0, -(far + near)/(far - near), -2*far*near / (far - near),
      0, 0, -1, 0;

  return m;
}

void Perspective::print() {
  printf("n=%f, f=%f\nl=%f, r=%f\nt=%f, b=%f\n", this->near, this->far, this->left, this->right, this->top, this->bottom);
}
