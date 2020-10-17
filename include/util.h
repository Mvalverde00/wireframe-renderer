#ifndef __MY_UTIL_H__
#define __MY_UTIL_H__

#include "Eigen/Dense"
#include "color.h"
#include "scene_object.h"
#include "ppm.h"


/* Given an point coordinate in NDC and a target (width, height), maps the point
 smoothly in the range (0, width) and (0, height) and stores the output in the point */
void ndc_to_pixel(Eigen::Vector3d& point, int width, int height);

/* Same as above, but returns result as a new vector instead of modifying input */
Eigen::Vector2d ndc_to_screen(Eigen::Vector3d point, int width, int height);


/* Maps all of a SceneObject's vertices (which are already in NDC) to screen pixel coordinates */
void ndc_to_pixel(SceneObject& obj, PPM& ppm);


/* Checks if a given point/vertex is inside the NDC bounding box (-1, 1) */
bool in_ndc(double x, double y, double z);
bool in_ndc(Eigen::Vector3d& v);

// Checks if a is in the range [min, max] (inclusive)
bool in_range(double a, double min, double max);

/* Generates a random color */
Color random_color();

Eigen::Vector4d homogeneous(Eigen::Vector3d& v);

Eigen::Vector3d cartesian3d(Eigen::Vector4d& v);

Color cwise_min(const Color& c1, const Color& c2);

double compute_alpha(Eigen::Vector2d& a, Eigen::Vector2d& b, Eigen::Vector2d& c, int x, int y);
double compute_beta(Eigen::Vector2d& a, Eigen::Vector2d& b, Eigen::Vector2d& c, int x, int y);
double compute_gamma(Eigen::Vector2d& a, Eigen::Vector2d& b, Eigen::Vector2d& c, int x, int y);

#endif
