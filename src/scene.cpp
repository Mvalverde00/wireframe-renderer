#include "scene.h"

#include <stdio.h>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <vector>
#include <cmath>

#include "Eigen/Dense"
#include "scene_object.h"
#include "parse_matrix.h"

#include "util.h"
#include "material.h"

Scene::Scene() {
  this->cam = Camera();
};

Scene::Scene(std::string filename) {
  std::ifstream stream = std::ifstream(filename);
  if (!stream) {
    printf("File '%s' could not be opened\n", filename.c_str());
  }

  this->cam = Camera(stream);

  std::string line;
  while (getline(stream, line)) {
    if (line.empty()) {
      break;
    }
    PointLight light = PointLight(line);
    this->lights.push_back(light);
  }

  // Associates every object name with a corresponding SceneObject. These
  // have no transformations associated with them.  The int is used to track
  // how many copies of the object we have made.
  std::unordered_map<std::string, std::pair<SceneObject, int>> bases;

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

    Material mat = Material(stream);
    copy.mat = mat;

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

void Scene::draw(PPM& ppm, bool aa) {
  for (SceneObject& obj : this->objs) {
    // Create a copy of the vertices we can use to restore them,
    // so that the draw call isn't destructive.
    std::vector<Eigen::Vector3d> copy = obj.vertices;

    this->convert_NDC(obj);
    ndc_to_pixel(obj, ppm);
    obj.draw(ppm, aa);

    obj.vertices = copy;
  }
}

void Scene::convert_NDC(SceneObject& obj) {
  Eigen::Matrix4d cam_inv = this->cam.getInverse();
  Eigen::Matrix4d pers = this->cam.getPMatrix();

  for (Eigen::Vector3d& v : obj.vertices) {
    Eigen::Vector4d homo(v.x(), v.y(), v.z(), 1);
    Eigen::Vector4d c_coords = cam_inv * homo;
    Eigen::Vector4d p_coords = pers * c_coords;

    p_coords *= (1 / (-c_coords(2))); // account for homogeneous component = -z_c

    v.x() = p_coords(0);
    v.y() = p_coords(1);
    v.z() = p_coords(2);
  }
}

Eigen::Vector3d Scene::convert_NDC(Eigen::Vector3d v) {
  Eigen::Vector4d homo = homogeneous(v);
  Eigen::Vector4d c_coords = this->cam.getInverse() * homo;
  Eigen::Vector4d p_coords = this->cam.getPMatrix() * c_coords;

  p_coords *= (1 / (-c_coords(2)));

  return Eigen::Vector3d(p_coords.x(), p_coords.y(), p_coords.z());
}


Color Scene::calc_lighting(Eigen::Vector3d& point, Eigen::Vector3d& normal, Material& mat) {
  Color c_d = mat.get_diffuse();
  Color c_a = mat.get_ambient();
  Color c_s = mat.get_specular();
  float shininess = mat.get_shininess();

  Color diffuse_sum = Color {0, 0, 0};
  Color specular_sum = Color {0, 0, 0};

  Eigen::Vector3d cam_dir = (this->cam.get_pos() - point).normalized();

  for (PointLight& light : this->lights) {
    Eigen::Vector3d l_p = light.pos;
    Color l_c = light.color;
    Eigen::Vector3d l_dir = (l_p - point).normalized();

    float dist_sqd = (point - l_p).norm();
    l_c = l_c * (1 / (1 + light.k * dist_sqd));

    double dot = normal.dot(l_dir);
    Color l_diffuse = l_c * std::max(0.0, dot);
    diffuse_sum += l_diffuse;

    Color l_specular = l_c * std::pow(std::max(0.0, normal.dot( (cam_dir + l_dir).normalized() )), shininess);
    specular_sum += l_specular;
  }

  Color left = Color {255, 255, 255};
  Color right = (c_a + (diffuse_sum * c_d) + (specular_sum * c_s));
  return cwise_min(left, right);
}

void Scene::draw_gouraud(PPM& ppm) {
  for (SceneObject& obj : this->objs) {
    std::vector<Eigen::Vector3d> copy = obj.vertices;

    for (Face& face : obj.faces) {
      Eigen::Vector3d& v0 = obj.vertices[face.vertex_indices[0]];
      Eigen::Vector3d& v1 = obj.vertices[face.vertex_indices[1]];
      Eigen::Vector3d& v2 = obj.vertices[face.vertex_indices[2]];
      Eigen::Vector3d& n0 = obj.normals[face.normal_indices[0]];
      Eigen::Vector3d& n1 = obj.normals[face.normal_indices[1]];
      Eigen::Vector3d& n2 = obj.normals[face.normal_indices[2]];

      Color c0 = calc_lighting(v0, n0, obj.mat);
      Color c1 = calc_lighting(v1, n1, obj.mat);
      Color c2 = calc_lighting(v2, n2, obj.mat);

      draw_triangle(this->convert_NDC(v0), this->convert_NDC(v1), this->convert_NDC(v2), c0, c1, c2, ppm);
    }

    obj.vertices = copy;
  }
}
