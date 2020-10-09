#ifndef __SCENE_H__
#define __SCENE_H__

#include <string>
#include <vector>

#include "camera.h"
#include "scene_object.h"
#include "ppm.h"

class Scene {

  std::vector<SceneObject> objs;

  Camera cam;

public:
  Scene();
  Scene(std::string filename);

  void print();

  void convert_NDC();
  void draw(PPM& ppm);
};

#endif
