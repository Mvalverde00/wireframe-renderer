#ifndef __MY_UTIL_H__
#define __MY_UTIL_H__

#include "vector3f.h"
#include "color.h"
#include "scene_object.h"
#include "ppm.h"

/* Given an (x,y) coordinate in NDC and a target (width, height), maps the (x, y)
 smoothly in the range (0, width) and (0, height) and stores the output in x_out and y_out */
void ndc_to_pixel(float x, float y, int width, int height, float* x_out, float* y_out);

/* Maps all of a SceneObject's vertices (which are already in NDC) to screen pixel coordinates */
void ndc_to_pixel(SceneObject& obj, PPM& ppm);


/* Checks if a given point/vertex is inside the NDC bounding box (-1, 1) */
bool in_ndc(float x, float y, float z);
bool in_ndc(Vector3f& v);

/* Generates a random color */
Color random_color();

#endif
