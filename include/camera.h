#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <fstream>

#include "Eigen/Dense"
#include "perspective.h"

/*
 * Stores all the information about how the scene should be rendered:
 * from where, in what direction, and with what perspective.
 */
class Camera {
  /* Matrix representing the location and rotation of the camera */
  Eigen::Matrix4d m;
  Perspective perspective;
  Eigen::Vector3d pos;

public:
  Camera();
  /* Given a file stream pointing to a properly formatted camera description,
    read until we hit a blank line */
  Camera(std::ifstream& stream);

  /* Translate the camera by the specified deltas */
  void translate(float x, float y, float z);

  /* Rotates the camera about the given unit vector by the given angle.  Will
    work even if the camera is not at the origin */
  void rotate(float ux, float uy, float uz, float ang);
  void print();

  /* Matrix and Inverse Matrix of this.m */
  Eigen::Matrix4d getMatrix();
  Eigen::Matrix4d getInverse();

  /* Projection Matrix */
  Eigen::Matrix4d getPMatrix();


};

#endif
