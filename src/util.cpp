#include "util.h"

#include <stdlib.h>

#include "vertex.h"
#include "color.h"

void ndc_to_pixel(float x, float y, int width, int height, int* x_out, int* y_out) {
  *x_out = (x + 1) * (width - 1) / 2;
  // Need to flip y_out around because in pixel coords, y increases moving downward.
  *y_out = height - 1 - ((y + 1) * (height - 1) / 2);
}

bool in_ndc(float x, float y, float z) {
  // ignore z for now
  return (x >= -1 && x <= 1) && (y >= -1 && y <= 1);
}

bool in_ndc(Vertex& v) {
  return in_ndc(v.x, v.y, v.z);
}

Color random_color() {
  return Color {std::rand() % 256, std::rand() % 256, std::rand() % 256};
};
