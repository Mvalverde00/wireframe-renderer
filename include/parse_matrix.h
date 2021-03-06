#ifndef __PARSE_MATRIX_H__
#define __PARSE_MATRIX_H__

#include "Eigen/Dense"

#include <fstream>

/* Creates a 4d translation matrix for the given values */
Eigen::Matrix4d create_translation_matrix(double dx, double dy, double dz);

/* Creates a 4d rotation matrix about the given unit vector by the specified angle */
Eigen::Matrix4d create_rotation_matrix(double ux, double uy, double uz, double ang);

/*Creates a 4d scaling matrix for the given scaling factors */
Eigen::Matrix4d create_scalar_matrix(double sx, double sy, double sz);


/* Given a file stream pointing to a series of transformations, processes them into
  matrices and multiplies them, resulting in a final transformation matrix.
  Stops after reading in a new/blank line, or on end of file*/
Eigen::Matrix4d parse_file(std::ifstream& stream);

#endif
