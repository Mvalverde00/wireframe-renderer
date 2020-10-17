#include "color.h"

#include <string>
#include <iostream>

std::string Color::serialize() {
  std::string result;

  result += std::to_string((int) r) + " ";
  result += std::to_string((int) g) + " ";
  result += std::to_string((int) b) + " ";
  result += " ";

  return result;
}

void Color::set_r(double r) {
  if (r < 0) {
    r = 0.0;
  }
  if (r > 255) {
    r = 255.0;
  }

  this->r = (uint8_t)r;
}

void Color::set_g(double g) {
  if (g < 0) {
    g = 0.0;
  }
  if (g > 255) {
    g = 255.0;
  }

  this->g = g;
}

void Color::set_b(double b) {
  if (b < 0) {
    b = 0.0;
  }
  if (b > 255) {
    b = 255.0;
  }

  this->b = (uint8_t)b;
}

Color Color::operator*(double intensity) const {
  return Color {(uint8_t)(this->r * intensity), (uint8_t)(this->g * intensity), (uint8_t)(this->b * intensity)};
}

Color Color::operator*(const Color& other) const {
  Color result;
  result.r = (uint8_t)((this->r/255.0 * other.r/255.0) * 255.0);
  result.g = (uint8_t)((this->g/255.0 * other.g/255.0) * 255.0);
  result.b = (uint8_t)((this->b/255.0 * other.b/255.0) * 255.0);

  return result;
}

Color& Color::operator+=(const Color& other) {
  this->set_r(this->r + other.r);
  this->set_g(this->g + other.g);
  this->set_b(this->b + other.b);
  return *this;
}

Color Color::operator+(const Color& other) const {
  Color result;
  result.set_r(this->r + other.r);
  result.set_g(this->g + other.g);
  result.set_b(this->b + other.b);
  return result;
}

Color operator*(double intensity, const Color& c) {
  return c * intensity;
}
