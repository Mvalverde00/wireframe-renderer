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

  std::string line;
  std::stringstream sstream;
  while (getline(file_stream, line)) {
    sstream = std::stringstream(line.substr(2));
    std::string token = line.substr(0,2);

    if (token == "v ") {
      Vertex vert;
      sstream >> vert.x >> vert.y >> vert.z;

      this->vertices.push_back(vert);
    } else if (token == "vn") {

    } else if (token == "f ") {
      Face face;
      int vertex_index;
      while (sstream >> vertex_index) {
        face.vertex_indices.push_back(vertex_index);
      }

      this->faces.push_back(face);
    } else if (token == "# ") {

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
  for (size_t i = 1; i < this->vertices.size(); i++) {
    Vertex v = this->vertices[i];
    printf("v %f %f %f\n", v.x, v.y, v.z);
  }

  for (Face& face : this->faces) {
    printf("f ");
    for (int idx : face.vertex_indices) {
      printf("%d ", idx);
    }
    printf("\n");
  }
}

void SceneObject::print_coords() {
  printf("\n%s\n", this->name.c_str());

  // Start at one to skip zeroth padding vertex
  for (size_t i = 1; i < this->vertices.size(); i++) {
    Vertex v = this->vertices[i];
    std::cout << v.x << " " << v.y << " " << v.z << "\n";
  }
}

void SceneObject::draw_face(Face& face, PPM& ppm) {
  Color c = {255,0,0};
  for (size_t i = 0; i < face.vertex_indices.size() - 1; i++) {
    int curr = face.vertex_indices[i];
    int next = face.vertex_indices[i + 1];
    Vertex& v1 = this->vertices[curr];
    Vertex& v2 = this->vertices[next];

    draw_line(v1.x, v1.y, v2.x, v2.y, ppm, c);
  }
  Vertex& last = this->vertices[face.vertex_indices[face.vertex_indices.size() - 1]];
  Vertex& first = this->vertices[face.vertex_indices[0]];
  draw_line(last.x, last.y, first.x, first.y, ppm, c);
}

void SceneObject::draw(PPM& ppm) {
  for (Face& face : this->faces) {
    this->draw_face(face, ppm);
  }
}
