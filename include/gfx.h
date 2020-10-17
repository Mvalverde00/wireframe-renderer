#ifndef __GFX_H__
#define __GFX_H__

#include "Eigen/Dense"
#include "ppm.h"
#include "color.h"

/* Draws a line from (x0, y0) to (x1, y1) with Color c */
void draw_line(int x0, int y0, int x1, int y1, PPM& ppm, Color c);

void draw_line_aa(int x0, int y0, int x1, int y1, PPM& ppm, Color c);

void draw_line(int x0, int y0, int x1, int y1, PPM& ppm, Color c, bool aa);

void draw_triangle(Eigen::Vector3d ndc0, Eigen::Vector3d ndc1, Eigen::Vector3d ndc2, Color c0, Color c1, Color c2, PPM& ppm);

#endif
