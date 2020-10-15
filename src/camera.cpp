#include "camera.h"

#include <fstream>
#include <cassert>
#include <iostream>
#include <string>

#include "parse_matrix.h"
#include "vector3f.h"
#include "perspective.h"

Camera::Camera() {
  this->m = Eigen::MatrixXd::Identity(4,4);
  this->pos << 0, 0, 0;
}

Camera::Camera(std::ifstream& stream) : Camera::Camera() {
  std::string temp;
  Vector3f point;
  float aux;

  stream >> temp; // Camera:
  assert(temp.compare("camera:") == 0);
  stream >> temp >> point.x >> point.y >> point.z; //position x y z
  this->translate(point.x, point.y, point.z);

  stream >> temp >> point.x >> point.y >> point.z >> aux; // orientation x y z ang
  this->rotate(point.x, point.y, point.z, aux);

  float n, f, l, r, t, b;
  stream >> temp >> n >> temp >> f >> temp >> l >> temp >> r >> temp >> t
      >> temp >> b;
  this->perspective = Perspective(n, f, l, r, t, b);

  // Deplete the newline character at the end of camera section
  getline(stream, temp);
  getline(stream, temp);
}

void Camera::translate(float x, float y, float z) {
  Eigen::Matrix4d t = create_translation_matrix(x, y, z);
  this->pos(0) += x;
  this->pos(1) += y;
  this->pos(2) += z;
  this->m = t * m;
}

void Camera::rotate(float ux, float uy, float uz, float ang) {
  Eigen::Matrix4d r = create_rotation_matrix(ux, uy, uz, ang);
  Eigen::Matrix4d t_inv = create_translation_matrix(-this->pos(0), -this->pos(1), -this->pos(2));
  Eigen::Matrix4d t = create_translation_matrix(this->pos(0), this->pos(1), this->pos(2));

  this->m = t * r * t_inv * m;
}

void Camera::print() {
  std::cout << "Cam translation: " << this->pos(0) << ", "
      << this->pos(1) << ", " << this->pos(2) << "\n";
  std::cout << "Cam Matrix: \n" << this->m << "\n";

  this->perspective.print();
}

Eigen::Matrix4d Camera::getMatrix() {
  return this->m;
}

Eigen::Matrix4d Camera::getInverse() {
  return this->m.inverse();
}

Eigen::Matrix4d Camera::getPMatrix() {
  return this->perspective.matrix();
}
