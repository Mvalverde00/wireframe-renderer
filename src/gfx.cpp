#include "gfx.h"

#include "ppm.h"
#include "color.h"

void draw_line_helper(int x0, int y0, int x1, int y1, PPM& ppm, Color c, bool swap) {
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

/*
 * First, we wish to ensure we always progress from left to right (lower x to higher x)
 * If we are asked to draw a line where x1 < x0, we effectively swap the two points (x0,y0) and (x1,y1)
 * to ensure that x0 < x1. This covers scenarios in octants 1 and 5.
 * Next, we wish to cover lines with a negative slope.  A line with negative slope will have
 * dy < 0 (we ensure dx is positive because we always draw from left to right by the above).
 * If the line has negative slope, we want to actually decrease y by 1 each time exceed our error.
 * thus we track a "changeY" which is 1 by default, but set to -1 if dy is negative.  We also negate dy
 * to make it positive such that we can still use it with our error checking.  This covers
 * scenarios in octants 3 and 7.
 * Lastly, we must cover scenarios where |m| >= 1 which implies |dy| - |dx| >= 0
 * In this case 1 <= |m| <= infinity.  But this implies 1 >= |1/m| >= 0, which is similar
 * to the condition of our original algorithm.  In fact, the solution is to swap the
 * x and y coordinate of each point, effectively doing the calculation using 1/m.
 * Putting all of these changes together, we can draw any line of any slope in any octant!
 */
void draw_line(int x0, int y0, int x1, int y1, PPM& ppm, Color c) {
  int dy = y1 - y0;
  int dx = x1 - x0;
  if (dy < 0) dy *= -1;
  if (dx < 0) dx *= -1;

  if (dy - dx >= 0) {
    if (y1 > y0) {
      draw_line_helper(y0, x0, y1, x1, ppm, c, true);
    } else {
      draw_line_helper(y1, x1, y0, x0, ppm, c, true);
    }
  } else {
    if (x1 > x0) {
      draw_line_helper(x0, y0, x1, y1, ppm, c, false);
    } else {
      draw_line_helper(x1, y1, x0, y0, ppm, c, false);
    }
  }
}

void draw_line_aa(int x0, int y0, int x1, int y1, PPM& ppm, Color c, bool swap) {
  float y = (float) y0;
  int dx = x1 - x0;
  int dy = y1 - y0;
  if (dx == 0) {
    dx = 1;
  }

  float m = (float)dy / (float)dx;
  for (int x = x0; x <= x1; x++) {
    int yi = (int) y;
    float f = y - yi;

    if (swap) {
      ppm.set_pixel(y, x, c * (1-f));
      ppm.set_pixel(y + 1, x, c * f);
    } else {
      ppm.set_pixel(x, y, c * (1-f));
      ppm.set_pixel(x, y + 1, c * f);
    }
    y += m;
  }
}

void draw_line_aa(int x0, int y0, int x1, int y1, PPM& ppm, Color c) {
  int dy = y1 - y0;
  int dx = x1 - x0;
  if (dy < 0) dy *= -1;
  if (dx < 0) dx *= -1;

  if (dy - dx >= 0) {
    if (y1 > y0) {
      draw_line_aa(y0, x0, y1, x1, ppm, c, true);
    } else {
      draw_line_aa(y1, x1, y0, x0, ppm, c, true);
    }
  } else {
    if (x1 > x0) {
      draw_line_aa(x0, y0, x1, y1, ppm, c, false);
    } else {
      draw_line_aa(x1, y1, x0, y0, ppm, c, false);
    }
  }
}

void draw_line(int x0, int y0, int x1, int y1, PPM& ppm, Color c, bool aa) {
  if (aa) {
    draw_line_aa(x0, y0, x1, y1, ppm, c);
  } else {
    draw_line(x0, y0, x1, y1, ppm, c);
  }
}
