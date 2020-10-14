#include <stdio.h>
#include <iostream>
#include <stdbool.h>

#include "scene.h"
#include "ppm.h"
#include "color.h"
#include "gfx.h"

int main(int argc, char* argv[]) {
  if (!(argc == 4 || argc == 5)) {
    printf("Invalid usage!  Correct usage: ./my_prog <filename> <xres> <yres> (anti-aliasing)\n");
    exit(0);
  }

  Scene s = Scene(argv[1]);
  PPM ppm = PPM(std::stoi(argv[2]), std::stoi(argv[3]));

  bool aa = false;
  if (argc == 5) {
    aa = std::stoi(argv[4]) == 1;
  }

  s.draw(ppm, aa);


  std::cout << ppm.serialize();
}
