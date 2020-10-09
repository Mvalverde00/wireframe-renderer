#include "gfx.h"

#include "ppm.h"
#include "color.h"

void draw_line(int x0, int y0, int x1, int y1, PPM& ppm, Color c, bool swap) {
  int e = 0;
  int y = y0;
  int dx = x1 - x0;
  int dy = y1 - y0;

  int changeY = 1;
  if (dy < 0) {
    dy *= -1;
    changeY = -1;
  }

  for (int x = x0; x <= x1; x++) {
    if (swap) {
      ppm.set_pixel(y, x, c);
    } else {
      ppm.set_pixel(x, y, c);
    }
    if ( ((e + dy) << 1) < dx) {
      e += dy;
    } else {
      e += dy - dx;
      y += changeY;
    }
  }
}

void draw_line(int x0, int y0, int x1, int y1, PPM& ppm, Color c) {
  int dy = y1 - y0;
  int dx = x1 - x0;
  if (dy < 0) dy *= -1;
  if (dx < 0) dx *= -1;

  if (dy - dx >= 0) {
    if (y1 > y0) {
      draw_line(y0, x0, y1, x1, ppm, c, true);
    } else {
      draw_line(y1, x1, y0, x0, ppm, c, true);
    }
  } else {
    if (x1 > x0) {
      draw_line(x0, y0, x1, y1, ppm, c, false);
    } else {
      draw_line(x1, y1, x0, y0, ppm, c, false);
    }
  }

}
