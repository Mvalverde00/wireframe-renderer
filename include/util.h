#ifndef __MY_UTIL_H__
#define __MY_UTIL_H__

#include "vertex.h"
#include "color.h"

void ndc_to_pixel(float x, float y, int width, int height, int* x_out, int* y_out);

bool in_ndc(float x, float y, float z);
bool in_ndc(Vertex& v);

Color random_color();

#endif
