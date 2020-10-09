#include "scene_object.h"

#include <fstream>
#include <exception>
#include <string>
#include <iostream>

#include "gfx.h"
#include "ppm.h"
#include "vertex.h"
#include "face.h"
#include "util.h"

SceneObject::SceneObject() {
  this->name = "";
  this->vertices.push_back(Vertex{0,0,0});
}

SceneObject::SceneObject(std::string name, const char* filename) {
  this->name = name;

  std::ifstream file_stream;
  file_stream.open(filename);

  if (!file_stream) {
    std::string error_msg = std::string("Failed to open the file ") + filename;
    throw std::ios_base::failure(error_msg);
  }

  // vertices are one-indexed, so make a padding zeroth vertex.
  this->vertices.push_back(Vertex{0,0,0});

  char type;
  while (file_stream >> type) {
    if (type == 'v') {
      Vertex vert;
      file_stream >> vert.x >> vert.y >> vert.z;

      this->vertices.push_back(vert);
    } else if (type == 'f') {
      Face face;
      file_stream >> face.v1 >> face.v2 >> face.v3;

      this->faces.push_back(face);
    } else {
      throw std::ios_base::failure(
          "File formatted incorrectly, unexpected identifier encountered.");
    }
  }
};

void SceneObject::transform(Eigen::Matrix4d t_matrix) {
  for (size_t i = 0; i < this->vertices.size(); i++) {
    Vertex v = this->vertices[i];

    Eigen::Vector4d vertex_vector(v.x, v.y, v.z, 1);

    Eigen::Vector4d transformed = t_matrix * vertex_vector;

    v.x = transformed(0);
    v.y = transformed(1);
    v.z = transformed(2);

    this->vertices[i] = v;
  }
}

void SceneObject::print() {
  printf("\n%s\n\n", this->name.c_str());

  // Start at one to skip zeroth padding vertex
  for (int i = 1; i < this->vertices.size(); i++) {
    Vertex v = this->vertices[i];
    printf("v %f %f %f\n", v.x, v.y, v.z);
  }

  for (Face face : this->faces) {
    printf("f %d %d %d\n", face.v1, face.v2, face.v3);
  }
}

void SceneObject::print_coords() {
  printf("\n%s\n", this->name.c_str());

  // Start at one to skip zeroth padding vertex
  for (int i = 1; i < this->vertices.size(); i++) {
    Vertex v = this->vertices[i];
    std::cout << v.x << " " << v.y << " " << v.z << "\n";
  }
}

void SceneObject::draw_face(Face& face, PPM& ppm) {
  Vertex v1 = this->vertices[face.v1];
  Vertex v2 = this->vertices[face.v2];
  Vertex v3 = this->vertices[face.v3];

  if (!in_ndc(v1) || !in_ndc(v2) || !in_ndc(v3)) {
    return;
  }
  ndc_to_pixel(v1.x, v1.y, ppm.get_width(), ppm.get_height(), (int*)&v1.x, (int*)&v1.y);
  ndc_to_pixel(v2.x, v2.y, ppm.get_width(), ppm.get_height(), (int*)&v2.x, (int*)&v2.y);
  ndc_to_pixel(v3.x, v3.y, ppm.get_width(), ppm.get_height(), (int*)&v3.x, (int*)&v3.y);

  Color c = random_color();
  draw_line(*((int*)&v1.x), *((int*)&v1.y), *((int*)&v2.x), *((int*)&v2.y), ppm, c);
  draw_line(*((int*)&v2.x), *((int*)&v2.y), *((int*)&v3.x), *((int*)&v3.y), ppm, c);
  draw_line(*((int*)&v3.x), *((int*)&v3.y), *((int*)&v1.x), *((int*)&v1.y), ppm, c);
}

void SceneObject::draw(PPM& ppm) {
  for (Face& face : this->faces) {
    this->draw_face(face, ppm);
  }
}
