Zak Olyarnik
CS-432 A5

Windows 10
VS 2017
OpenGL Version: 4.5.0 - Build 22.20.16.4836
GLSL Version: 4.50 - Build 22.20.16.4836
Status: Using GLEW 2.1.0

To run, unzip and open the A5.sln file with Visual Studio.  Click the "run" button.
Output is demonstrated in the A5.mp4 file.


I subdivided my cube and plane objects further for better lighting effects, as well
as adding normals to both of them and the sphere.  I created a directional light
and a spotlight based on the lighting sample code and other methods discussed in
class, and played around with the lighting and material properties until I got
effects I liked.  The shaders took the longest to set up, only because I wanted to
make sure I understood what the sample code was doing.  I was able to simplify
some of it as well, because the flashlight is fixed to the camera so some of the
calculations became easier.