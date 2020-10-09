#ifndef __COLOR_H__
#define __COLOR_H__

#include <string>
#include <cstdint>

/*
 * An RGB color where each channel can vary between 0-255
*/
struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;

  std::string serialize();
};

const Color BLACK = {0,0,0};
const Color RED = {255,0,0};
const Color GREEN = {0,255,0};
const Color BLUE = {0,0,255};
const Color GREY = {128, 128, 128};
const Color WHITE = {255, 255, 255};

#endif
