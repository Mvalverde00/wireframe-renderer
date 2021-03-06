#include "material.h"

#include <fstream>
#include <string>
#include <sstream>
#include <cassert>

Material::Material() {}

Material::Material(std::ifstream& stream) {
  std::string line;
  std::string temp;
  std::stringstream sstream;

  double r, g, b;

  getline(stream, line);
  sstream = std::stringstream(line);
  sstream >> temp;
  assert(temp == "ambient");
  sstream >> r >> g >> b;
  this->ambient = Color {(uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255)};

  getline(stream, line);
  sstream = std::stringstream(line);
  sstream >> temp;
  assert(temp == "diffuse");
  sstream >> r >> g >> b;
  this->diffuse = Color {(uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255)};

  getline(stream, line);
  sstream = std::stringstream(line);
  sstream >> temp;
  assert(temp == "specular");
  sstream >> r >> g >> b;
  this->specular = Color {(uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255)};


  getline(stream, line);
  sstream = std::stringstream(line);
  sstream >> temp;
  assert(temp == "shininess");
  sstream >> this->shininess;
}

Color Material::get_ambient() {
  return this->ambient;
}

Color Material::get_diffuse() {
  return this->diffuse;
}

Color Material::get_specular() {
  return this->specular;
}
double Material::get_shininess() {
  return this->shininess;
}
