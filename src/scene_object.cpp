#include "scene_object.h"

#include <fstream>
#include <exception>
#include <string>
#include <iostream>
#include <sstream>

#include "gfx.h"
#include "ppm.h"

#include "face.h"
#include "util.h"

SceneObject::SceneObject() {
  this->name = "";
  this->vertices.push_back(Eigen::Vector3d{0,0,0});
  this->normals.push_back(Eigen::Vector3d{0,0,0});
}

SceneObject::SceneObject(std::string name, const char* filename) {
  this->name = name;

  std::ifstream file_stream;
  file_stream.open(filename);

  if (!file_stream) {
    std::string error_msg = std::string("Failed to open the file ") + filename;
    throw std::ios_base::failure(error_msg);
  }

  // vertices & normals are one-indexed, so make a padding zeroth vertex.
  this->vertices.push_back(Eigen::Vector3d{0, 0, 0});
  this->normals.push_back(Eigen::Vector3d{0, 0, 0});

  std::string line;
  std::stringstream sstream;
  while (getline(file_stream, line)) {
    sstream = std::stringstream(line.substr(2));
    std::string token = line.substr(0,2);

    if (token == "v ") {
      Eigen::Vector3d vert;
      sstream >> vert.x() >> vert.y() >> vert.z();

      this->vertices.push_back(vert);
    } else if (token == "vn") {
      Eigen::Vector3d norm;
      sstream >> norm.x() >> norm.y() >> norm.z();

      this->normals.push_back(norm);
    } else if (token == "f ") {
      Face face;
      char temp;
      int index;

      while (sstream >> index) {
        face.vertex_indices.push_back(index);

        if (sstream.peek() == '/') {
          sstream >> temp;
          if (sstream.peek() == '/') {
            sstream >> temp;
            sstream >> index;
            face.normal_indices.push_back(index);
          }
        }
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
    Eigen::Vector4d vertex = homogeneous(this->vertices[i]);
    Eigen::Vector4d transformed = t_matrix * vertex;

    this->vertices[i] = cartesian3d(transformed);
  }

  // Normal vectors don't care about translation
  t_matrix(0, 3) = 0;
  t_matrix(1, 3) = 0;
  t_matrix(2, 3) = 0;

  for (size_t i = 0; i < this->normals.size(); i++) {
    Eigen::Vector4d normal = homogeneous(this->normals[i]);
    Eigen::Matrix4d normal_t_matrix = t_matrix.inverse().transpose();

    Eigen::Vector4d transformed = normal_t_matrix * normal;

    // lighting algorithms will expect norms to be normalized.
    transformed.normalize();
    this->normals[i] = cartesian3d(transformed);
  }
}

void SceneObject::print() {
  printf("\n%s\n\n", this->name.c_str());

  // Start at one to skip zeroth padding vertex
  for (size_t i = 1; i < this->vertices.size(); i++) {
    Eigen::Vector3d v = this->vertices[i];
    printf("v %f %f %f\n", v.x(), v.y(), v.z());
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
    Eigen::Vector3d v = this->vertices[i];
    std::cout << v.x() << " " << v.y() << " " << v.z() << "\n";
  }
}

void SceneObject::draw(PPM& ppm, bool aa) {
  Color c = {255,255,255};
  for (Face& face : this->faces) {
    for (size_t i = 0; i < face.vertex_indices.size() - 1; i++) {
      int curr = face.vertex_indices[i];
      int next = face.vertex_indices[i + 1];
      Eigen::Vector3d& v1 = this->vertices[curr];
      Eigen::Vector3d& v2 = this->vertices[next];

      draw_line(v1.x(), v1.y(), v2.x(), v2.y(), ppm, c, aa);
    }
    Eigen::Vector3d& last = this->vertices[face.vertex_indices[face.vertex_indices.size() - 1]];
    Eigen::Vector3d& first = this->vertices[face.vertex_indices[0]];
    draw_line(last.x(), last.y(), first.x(), first.y(), ppm, c, aa);
  }
}
