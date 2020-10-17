#include "gfx.h"

#include "Eigen/Dense"
#include "ppm.h"
#include "color.h"
#include "util.h"

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
  double y = (double) y0;
  int dx = x1 - x0;
  int dy = y1 - y0;
  if (dx == 0) {
    dx = 1;
  }

  double m = (double)dy / (double)dx;
  for (int x = x0; x <= x1; x++) {
    int yi = (int) y;
    double f = y - yi;

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


void draw_triangle(Eigen::Vector3d ndc0, Eigen::Vector3d ndc1, Eigen::Vector3d ndc2, Color c0, Color c1, Color c2, PPM& ppm) {
  Eigen::Vector3d cross = (ndc2 - ndc1).cross(ndc0 - ndc1);
  if (cross.z() < 0) {
    return;
  }

  Eigen::Vector2d p0 = ndc_to_screen(ndc0, ppm.get_width(), ppm.get_height());
  Eigen::Vector2d p1 = ndc_to_screen(ndc1, ppm.get_width(), ppm.get_height());
  Eigen::Vector2d p2 = ndc_to_screen(ndc2, ppm.get_width(), ppm.get_height());

  int xmin = std::min(std::min(p0.x(), p1.x()), p2.x());
  int xmax = std::max(std::max(p0.x(), p1.x()), p2.x());
  int ymin = std::min(std::min(p0.y(), p1.y()), p2.y());
  int ymax = std::max(std::max(p0.y(), p1.y()), p2.y());

  for (int x = xmin; x <= xmax; x++) {
    for (int y = ymin; y <= ymax; y++) {
      double alpha = compute_alpha(p0, p1, p2, x, y);
      double beta = compute_beta(p0, p1, p2, x, y);
      double gamma = compute_gamma(p0, p1, p2, x, y);
      if (in_range(alpha, 0, 1) && in_range(beta, 0, 1) && in_range(gamma, 0, 1)) {
        Eigen::Vector3d ndc_avg = (alpha * ndc0) + (beta * ndc1) + (gamma * ndc2);
        if (in_ndc(ndc_avg) && ppm.buffer(x, y, ndc_avg.z())) {
          Color result;
          result = alpha * c0 + beta * c1 + gamma * c2;
          ppm.set_pixel(x, y, result);
        }
      }
    }
  }
}
