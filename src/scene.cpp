#include "scene.h"

#include <stdio.h>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <vector>

#include "Eigen/Dense"
#include "scene_object.h"
#include "parse_matrix.h"
#include "vertex.h"
#include "util.h"

Scene::Scene() {
  this->cam = Camera();
};

Scene::Scene(std::string filename) {
  std::ifstream stream = std::ifstream(filename);
  if (!stream) {
    printf("File '%s' could not be opened\n", filename.c_str());
  }

  this->cam = Camera(stream);

  // Associates every object name with a corresponding SceneObject. These
  // have no transformations associated with them.  The int is used to track
  // how many copies of the object we have made.
  std::unordered_map<std::string, std::pair<SceneObject, int>> bases;

  std::string line;
  getline(stream, line);
  assert(line.compare("objects:") == 0);
  while (getline(stream, line)) {
    if (line.empty()) {
      break;
    }
    std::string name = line.substr(0, line.find(" "));
    std::string file = line.substr(line.find(" ") + 1);

    SceneObject obj = SceneObject(name, file.c_str());
    std::pair<SceneObject, int> pair(obj, 0);

    bases[name] = pair;
  }

  std::string name;
  while (getline(stream, name)) {
    if (name.empty()) {
      continue;
    }

    SceneObject copy = SceneObject(bases[name].first);
    bases[name].second++;
    copy.name = name + "_copy" + std::to_string(bases[name].second);

    Eigen::Matrix4d t_matrix = parse_file(stream);
    copy.transform(t_matrix);

    this->objs.push_back(copy);
  }
}

void Scene::print() {
  this->cam.print();
  for (SceneObject& obj : this->objs) {
    obj.print();
  }
}

void Scene::draw(PPM& ppm) {
  for (SceneObject& obj : this->objs) {
    std::vector<Vertex> copy = obj.vertices;

    this->convert_NDC(obj);
    ndc_to_pixel(obj, ppm);
    obj.draw(ppm);

    obj.vertices = copy;
  }
}

void Scene::convert_NDC(SceneObject& obj) {
  Eigen::Matrix4d cam_inv = this->cam.getInverse();
  Eigen::Matrix4d pers = this->cam.getPMatrix();

  for (Vertex& v : obj.vertices) {
    Eigen::Vector4d homo(v.x, v.y, v.z, 1);
    Eigen::Vector4d c_coords = cam_inv * homo;
    Eigen::Vector4d p_coords = pers * c_coords;

    p_coords *= (1 / (-c_coords(2))); // account for homogeneous component = -z_c

    v.x = p_coords(0);
    v.y = p_coords(1);
    v.z = p_coords(2);
  }
}
