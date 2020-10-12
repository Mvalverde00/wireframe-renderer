#include <stdio.h>
#include <iostream>

#include "scene.h"
#include "ppm.h"
#include "color.h"

int main(int argc, char* argv[]) {
  if (argc != 4) {
    printf("Invalid usage!  Correct usage: ./my_prog <filename> <xres> <yres>\n");
    exit(0);
  }

  Scene s = Scene(argv[1]);
  PPM ppm = PPM(std::stoi(argv[2]), std::stoi(argv[3]));

  s.draw(ppm);

  std::cout << ppm.serialize();
}
