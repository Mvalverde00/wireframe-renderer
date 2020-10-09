#ifndef __FACE_H__
#define __FACE_H__

/**
 * A triangular face defined by 3 vertices, whose indices are provided
 * in a 1-indexed manner.
 */
struct Face {
  int v1;
  int v2;
  int v3;
};

#endif
