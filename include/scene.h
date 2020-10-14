#ifndef __SCENE_H__
#define __SCENE_H__

#include <string>
#include <vector>
#include <stdbool.h>

#include "camera.h"
#include "scene_object.h"
#include "ppm.h"

/*
 * Defines a world filled with the specified SceneObjects and a Camera detailing
 * how those SceneObjects should be rendered.
 */
class Scene {
  std::vector<SceneObject> objs;
  Camera cam;

public:
  Scene();
  /* A file in the format specified in the assignment instructions */
  Scene(std::string filename);

  void print();

  /* Converts a SceneObject's vertices to NDC using the camera */
  void convert_NDC(SceneObject& obj);

  /* Renders all of the SceneObjects to the given PPM.  Can be done with or
  * without antialiasing. */
  void draw(PPM& ppm, bool aa);
};

#endif
