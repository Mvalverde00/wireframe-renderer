#include <stdio.h>
#include <iostream>

#include "scene.h"
#include "ppm.h"
#include "color.h"
#include "gfx.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Invalid usage!  Correct usage: ./my_prog <filename>\n");
    exit(0);
  }

  Scene s = Scene(argv[1]);

  s.convert_NDC();
  PPM ppm = PPM(700, 700);
  s.draw(ppm);
/*
  draw_line(0, 0, 100, 199, ppm, RED);
  draw_line(100, 199, 199, 0, ppm, RED);
  draw_line(0, 0, 100, 50, ppm, GREEN);
  draw_line(0, 0, 100, 100, ppm, RED);
  draw_line(0,0,199,0,ppm,BLUE);
  draw_line(0,199,199,199,ppm,BLUE);
*/
  std::cout << ppm.serialize();
}
