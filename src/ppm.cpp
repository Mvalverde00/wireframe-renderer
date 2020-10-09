#include "ppm.h"

#include <string>
#include <cassert>

PPM::PPM(int width, int height) {
  this->width = width;
  this->height = height;

  this->pixels = (Color*) calloc(width * height, sizeof(Color));
}

PPM::PPM() : PPM(0,0) {};

PPM::~PPM() {
  free(this->pixels);
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

std::string PPM::serialize() {
  std::string result = "";

  result += "P3\n";
  result += std::to_string(this->width) + " " + std::to_string(this->height) + "\n";
  result += "255\n";

  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
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
