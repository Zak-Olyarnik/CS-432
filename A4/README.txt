Zak Olyarnik
CS-432 A4

Windows 10
VS 2017
OpenGL Version: 4.5.0 - Build 22.20.16.4836
GLSL Version: 4.50 - Build 22.20.16.4836
Status: Using GLEW 2.1.0

To run, unzip and open the A4.sln file with Visual Studio.  Click the "run" button.
Output is demonstrated in the A4.mp4 file.


The plane object was created just using two triangles, and the subdivided sphere came
directly out of the lecture notes.  I wrote a testCollision() function for the plane,
which, while not being very useful based on the way we wanted to use it for the sphere,
was a little easier to start with and helped me figure out if the shape was hit at all,
if not which triangle was.  The sphere's testCollision() was then easy to write, and
the only other issue I had with it was not knowing that I had to re-bind the VBO before
attemtping to make changes to it.

However, picking did expose some hidden bugs with my camera class.  While it moved and
behaved correctly, picking would not work except from the starting, default camera view.
Prof. Burlick helped me find that the problem was with my vectors' w-coordinates not
being zero.  I knew that they needed to be, but didn't realize that doing operations
like the cross-product wouldn't automatically be "safe" and would require a manual reset
of w.  Once I added that, everything worked as expected.