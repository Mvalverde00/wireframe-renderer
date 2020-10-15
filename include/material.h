#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <fstream>

#include "color.h"

class Material {
  Color ambient;
  Color diffuse;
  Color specular;
  float shininess;

public:
  Material();
  Material(std::ifstream& stream);
};

#endif
