#include "parse_matrix.h"
#include "Eigen/Dense"

#include <math.h>
#include <fstream>
#include <string>
#include <sstream>

Eigen::Matrix4d create_translation_matrix(float dx, float dy, float dz) {
  Eigen::Matrix4d m;
  m << 1, 0, 0, dx,
       0, 1, 0, dy,
       0, 0, 1, dz,
       0, 0, 0, 1;
  return m;
}

Eigen::Matrix4d create_rotation_matrix(float ux, float uy, float uz, float ang) {
  Eigen::Matrix4d m;

  // Improve runtine by precomputing some common values
  float c = cos(ang);
  float s = sin(ang);
  float one_minus_cos = 1 - c;

  m << ux*ux + (1 - ux*ux)*c, ux*uy*one_minus_cos - uz*s, ux*uz*one_minus_cos + uy*s, 0,
       uy*ux*one_minus_cos + uz*s, uy*uy + (1-uy*uy)*c, uy*uz*one_minus_cos - ux*s, 0,
       uz*ux*one_minus_cos - uy*s, uz*uy*one_minus_cos + ux*s, uz*uz + (1 - uz*uz)*c, 0,
       0, 0, 0, 1;
  return m;
}

Eigen::Matrix4d create_scalar_matrix(float sx, float sy, float sz) {
  Eigen::Matrix4d m;

  m << sx, 0, 0, 0,
       0, sy, 0, 0,
       0, 0, sz, 0,
       0, 0, 0, 1;

  return m;
}

Eigen::Matrix4d parse_file(std::ifstream& stream) {
  Eigen::Matrix4d base = Eigen::MatrixXd::Identity(4,4);

  std::string line;
  while (getline(stream, line)) {
    if (line.empty()) {
      break;
    }

    Eigen::Matrix4d temp;
    char type;
    std::stringstream str_stream(line);
    str_stream >> type;

    float x, y, z, ang;
    str_stream >> x >> y >> z;
    switch(type) {
      case 't':
        temp = create_translation_matrix(x, y, z);
        break;
      case 'r':
        str_stream >> ang;
        temp = create_rotation_matrix(x, y, z, ang);
        break;
      case 's':
        temp = create_scalar_matrix(x, y, z);
        break;
      default:
        throw "Invalid file format";
        break;
    }
    base = temp * base;
  }

  return base;
}
