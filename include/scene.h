#ifndef __SCENE_H__
#define __SCENE_H__

#include <string>
#include <vector>
#include <stdbool.h>

#include "Eigen/Dense"
#include "camera.h"
#include "scene_object.h"
#include "ppm.h"
#include "point_light.h"

/*
 * Defines a world filled with the specified SceneObjects and a Camera detailing
 * how those SceneObjects should be rendered.
 */
class Scene {
  std::vector<SceneObject> objs;
  std::vector<PointLight> lights;
  Camera cam;

  Color calc_lighting(Eigen::Vector3d& point, Eigen::Vector3d& normal, Material& mat);

public:
  Scene();
  /* A file in the format specified in the assignment instructions */
  Scene(std::string filename);

  void print();

  /* Converts a SceneObject's vertices to NDC using the camera */
  void convert_NDC(SceneObject& obj);
  Eigen::Vector3d convert_NDC(Eigen::Vector3d v);

  void draw_gouraud(PPM& ppm);

  /* Renders all of the SceneObjects to the given PPM.  Can be done with or
  * without antialiasing. */
  void draw(PPM& ppm, bool aa);
};

#endif
