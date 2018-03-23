Zak Olyarnik
CS-432 A1

Windows 10
VS 2017
OpenGL Version: 4.5.0 - Build 22.20.16.4836
GLSL Version: 4.50 - Build 22.20.16.4836
Status: Using GLEW 2.1.0

To run, unzip and open the A1.sln file with Visual Studio.  Click the "run" button.
Output is demonstrated in the A1.mp4 file.

This assignment was pretty easy.  The code for generating the square was given and
it was trivial to create Triangle.h and .cpp files from that.  The hardest part was
to make the circle, and the trick was to realize that you need an extra two vertices
on top of however many you need for the desired resolution: the circle's center point
and the first vertex repeated at the end to close the cirlce.  This +2 shift needs to
be applied anywhere you deal with the vertices, especially the glDrawArrays call in the
draw() method.  The code for coloring the circle was achieved by using unique per-vertex
colors (and a new set of shaders to match), taken directly from the lecture notes.  I
did have to look up how to generate random numbers because it's been a while since I've
worked in C++, and I credited the source I used for that.