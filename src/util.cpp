#include "util.h"

#include <stdlib.h>
#include <cstdint>

#include "vector3f.h"
#include "color.h"

void ndc_to_pixel(float x, float y, int width, int height, float* x_out, float* y_out) {
  *x_out = (x + 1) * (width - 1) / 2;
  // Need to flip y_out around because in pixel coords, y increases moving downward.
  *y_out = height - 1 - ((y + 1) * (height - 1) / 2);
}

void ndc_to_pixel(SceneObject& obj, PPM& ppm) {
  for (Vector3f& v : obj.vertices) {
    ndc_to_pixel(v.x, v.y, ppm.get_width(), ppm.get_height(), &v.x, &v.y);
  }
}

bool in_ndc(float x, float y, float z) {
  // ignore z for now
  return (x >= -1 && x <= 1) && (y >= -1 && y <= 1);
}

bool in_ndc(Vector3f& v) {
  return in_ndc(v.x, v.y, v.z);
}

Color random_color() {
  return Color {(uint8_t) (std::rand() % 256), (uint8_t) (std::rand() % 256), (uint8_t) (std::rand() % 256)};
};
