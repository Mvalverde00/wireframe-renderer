#include "point_light.h"

#include <string>
#include <cassert>
#include <sstream>

PointLight::PointLight() {
  this->x = 0;
  this->y = 0;
  this->z = 0;
  this->c = Color {0, 0, 0};
  this->k = 0;
}

PointLight::PointLight(std::string& line) {
  std::stringstream sstream = std::stringstream(line);
  std::string temp;
  
  sstream >> temp;
  assert(temp == "light");

  sstream >> this->x >> this->y >> this->z;

  sstream >> temp;
  assert(temp == ",");

  float r, g, b;
  sstream >> r >> g >> b;
  this->c = Color {(uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255)};

  sstream >> this->k;
}
