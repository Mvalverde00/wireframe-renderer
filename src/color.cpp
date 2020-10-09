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
