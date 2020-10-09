#ifndef __PPM_H__
#define __PPM_H__

#include <string>

#include "color.h"

class PPM {

  int width;
  int height;

  /* 1D Array of pixels of length width * height.  Can be thought of as 2D
    array with clever indexing */
  Color* pixels;

public:
  PPM();
  PPM(int width, int height);

  ~PPM(); // Needs to free the pixels

  void set_pixel(int x, int y, Color c);
  Color get_color(int x, int y);
  std::string serialize();

  int get_width();
  int get_height();
};


#endif
