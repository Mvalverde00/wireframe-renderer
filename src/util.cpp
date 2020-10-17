#include "util.h"

#include <stdlib.h>
#include <cstdint>


#include "color.h"

Eigen::Vector2d ndc_to_screen(Eigen::Vector3d point, int width, int height) {
  Eigen::Vector2d result;
  result.x() = (point.x() + 1) * (width - 1) / 2;
  result.y() = height - 1 - ((point.y() + 1) * (height - 1) / 2);

  return result;
}

void ndc_to_pixel(Eigen::Vector3d& point, int width, int height) {
  point.x() = (point.x() + 1) * (width - 1) / 2;
  // Need to flip y_out around because in pixel coords, y increases moving downward.
  point.y() = height - 1 - ((point.y() + 1) * (height - 1) / 2);
}

void ndc_to_pixel(SceneObject& obj, PPM& ppm) {
  for (Eigen::Vector3d& v : obj.vertices) {
    ndc_to_pixel(v, ppm.get_width(), ppm.get_height());
  }
}

bool in_ndc(double x, double y, double z) {
  return in_range(x, -1, 1) && in_range(y, -1, 1) && in_range(z, -1, 1);
}

bool in_ndc(Eigen::Vector3d& v) {
  return in_ndc(v.x(), v.y(), v.z());
}

bool in_range(double a, double min, double max) {
  return a >= min && a <= max;
}

Color random_color() {
  return Color {(uint8_t) (std::rand() % 256), (uint8_t) (std::rand() % 256), (uint8_t) (std::rand() % 256)};
};


Eigen::Vector4d homogeneous(Eigen::Vector3d& v) {
  return Eigen::Vector4d {v.x(), v.y(), v.z(), 1};
}

Eigen::Vector3d cartesian3d(Eigen::Vector4d& v) {
  return Eigen::Vector3d {v.x() / v.w(), v.y() / v.w(), v.z() / v.w()};
}

Color cwise_min(const Color& c1, const Color& c2) {
  Color result;
  result.r = std::min(c1.r, c2.r);
  result.g = std::min(c1.g, c2.g);
  result.b = std::min(c1.b, c2.b);

  return result;
}








double barycentric_helper(int x, int y, Eigen::Vector2d& i, Eigen::Vector2d& j) {
  return (i.y() - j.y())*x + (j.x() - i.x())*y + i.x()*j.y() - j.x()*i.y();
}

double compute_alpha(Eigen::Vector2d& a, Eigen::Vector2d& b, Eigen::Vector2d& c, int x, int y) {
  return barycentric_helper(x, y, b, c) / barycentric_helper(a.x(), a.y(), b, c);
}

double compute_beta(Eigen::Vector2d& a, Eigen::Vector2d& b, Eigen::Vector2d& c, int x, int y) {
  return barycentric_helper(x, y, a, c) / barycentric_helper(b.x(), b.y(), a, c);
}

double compute_gamma(Eigen::Vector2d& a, Eigen::Vector2d& b, Eigen::Vector2d& c, int x, int y) {
  return barycentric_helper(x, y, a, b) / barycentric_helper(c.x(), c.y(), a, b);
}
