#include "point_light.h"

#include <string>
#include <cassert>
#include <sstream>

PointLight::PointLight() {
  this->pos = Eigen::Vector3d {0.0, 0.0, 0.0};
  this->color = Color {0, 0, 0};
  this->k = 0;
}

PointLight::PointLight(std::string& line) {
  std::stringstream sstream = std::stringstream(line);
  std::string temp;

  sstream >> temp;
  assert(temp == "light");

  sstream >> this->pos.x() >> this->pos.y() >> this->pos.z();

  sstream >> temp;
  assert(temp == ",");

  double r, g, b;
  sstream >> r >> g >> b;
  this->color = Color {(uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255)};

  sstream >> this->k;
}
