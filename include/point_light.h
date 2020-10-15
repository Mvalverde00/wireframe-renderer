#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#include <string>

#include "color.h"

class PointLight {
  float x;
  float y;
  float z;

  Color c;
  float k; // attenuation

public:
  PointLight();
  PointLight(std::string& line);
};

#endif
