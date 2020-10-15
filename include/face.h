#ifndef __FACE_H__
#define __FACE_H__

#include <vector>

/**
 * A triangular face defined by 3 or more vertices, whose indices are provided
 * in a 1-indexed manner.  Vertices can have normal indices, denoted by a corresping
 * index in the array.  E.g. normal_indices[i] is the normal index for vertex_indices[i].
 */
struct Face {
  std::vector<int> vertex_indices;
  std::vector<int> normal_indices;
};

#endif
