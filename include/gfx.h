#ifndef __GFX_H__
#define __GFX_H__

#include "ppm.h"
#include "color.h"

/* Draws a line from (x0, y0) to (x1, y1) with Color c */
void draw_line(int x0, int y0, int x1, int y1, PPM& ppm, Color c);

#endif
