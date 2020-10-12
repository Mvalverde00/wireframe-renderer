#ifndef __SCENE_OBJECT_H__
#define __SCENE_OBJECT_H__

#include <vector>
#include <string>
#include "Eigen/Dense"

#include "face.h"
#include "vertex.h"
#include "ppm.h"

/*
 * A 3D object parsed from a given .obj file.  One of possibly many objects in a
 * scene.
 */
struct SceneObject {
  std::string name;
  std::vector<Vertex> vertices;
  std::vector<Face> faces;

  /* Given a name for the object and a file in which it is stored,
  parse the given vertices/faces and store them in a workable format */
  SceneObject(std::string string, const char* filename);
  SceneObject();

  /* Prints out the objects name, vertices, and faces */
  void print();

  /* Prints the object's name and vertices only*/
  void print_coords();


  /* Applies the given transformation matrix to every vertex of the object */
  void transform(Eigen::Matrix4d t_matrix);

  /* Draw the wireframe version of the scene object to the given PPM */
  void draw(PPM& ppm);
  void draw_face(Face& face, PPM& ppm);
};

#endif
