#include "color.h"

#include <string>

std::string Color::serialize() {
  std::string result;

  result += std::to_string((int) r) + " ";
  result += std::to_string((int) g) + " ";
  result += std::to_string((int) b) + " ";
  result += " ";

  return result;
}

Color Color::operator*(float intensity) const {
  return Color {(uint8_t)(this->r * intensity), (uint8_t)(this->g * intensity), (uint8_t)(this->b * intensity)};
}

Color operator*(float intensity, const Color& c) {
  return c * intensity;
}
