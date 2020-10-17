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

  Color get_ambient();
  Color get_diffuse();
  Color get_specular();
  float get_shininess();
};

#endif
