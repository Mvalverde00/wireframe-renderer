#ifndef __FACE_H__
#define __FACE_H__

#include <vector>

/**
 * A triangular face defined by 3 or more vertices, whose indices are provided
 * in a 1-indexed manner.
 */
struct Face {
  std::vector<int> vertex_indices;
};

#endif
