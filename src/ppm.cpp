#include "ppm.h"

#include <string>
#include <cassert>
#include <limits>

PPM::PPM(int width, int height) {
  this->width = width;
  this->height = height;

  this->pixels = (Color*) calloc(width * height, sizeof(Color));
  this->zbuffer = (double*) calloc(width * height, sizeof(double));

  for (int x = 0; x < width; x++) {
    for (int y = 0; y< height; y++) {
      this->zbuffer[y*this->width + x] = std::numeric_limits<double>::max();
    }
  }
}

PPM::PPM() : PPM(0,0) {};

PPM::~PPM() {
  free(this->pixels);
  free(this->zbuffer);
}

void PPM::set_pixel(int x, int y, Color c) {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return;
  }

  pixels[y*this->width + x] = c;
}

Color PPM::get_color(int x, int y) {
  assert(x >= 0 && x < width);
  assert(y >= 0 && y < height);

  return pixels[y*this->width + x];
}

bool PPM::buffer(int x, int y, double z) {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return false;
  }

  if (z <= this->zbuffer[y*this->width + x]) {
    this->zbuffer[y*this->width + x] = z;
    return true;
  }
  return false;
}

std::string PPM::serialize() {
  std::string result = "";

  result += "P3\n";
  result += std::to_string(this->width) + " " + std::to_string(this->height) + "\n";
  result += "255\n";

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Color c = get_color(x, y);
      result += c.serialize() + "\n";
    }
  }

  return result;
}

int PPM::get_width() {
  return this->width;
}

int PPM::get_height() {
  return this->height;
}
