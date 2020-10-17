#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#include <string>

#include "color.h"
#include "Eigen/Dense"

class PointLight {
public:
  Eigen::Vector3d pos;
  Color color;
  float k; // attenuation

  PointLight();
  PointLight(std::string& line);

};

#endif
