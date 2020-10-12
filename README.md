# Wireframe Renderer

### Usage
Create the executable by running `make all` in the root directory.  This should create an executable called
`wireframe` inside of the `/data` directory.  Running the executable takes 3 arguments, filename, xres, and yres.
Thus an example of correct usage would be `./wireframe scene.txt 600 600`, where `scene.txt` and any of
its dependent .obj files are in the same directory as `wireframe`.  By default, the wireframe
executable prints its output to stdout.  In order to actually display an image, we recommend
using Image Magick, which can be piped the output to display an image.
For example, `./wireframe scene.txt 600 600 | display` will display the rendered image using Image Magick

### Notes
A description of the line drawing algorithm used can be found in `gfx.cpp`.
