Zak Olyarnik
CS-432 A3

Windows 10
VS 2017
OpenGL Version: 4.5.0 - Build 22.20.16.4836
GLSL Version: 4.50 - Build 22.20.16.4836
Status: Using GLEW 2.1.0

To run, unzip and open the A3.sln file with Visual Studio.  Click the "run" button.
Output is demonstrated in the A3.mp4 file.

NOTE: The bug shown with adjusting the yaw at the very end of the video has been fixed,
it was just a missing break statement in my switch. I didn't want to make another video
just for that...

I created an OctagonalPrism class to extend Drawable, and used the lecture notes to
create a flat-shaded shape and a single-colored frame using indexing (the extra credit).
I tried to derive a programmatic way of filling the array of indices, but that got so
messy that I just hardcoded the frame indices. I drew the main shape parametrically and
used a separate vertex shader for the frame. The draw call combines the model, view, and
projection matrices into one and pushes a single uniform mat4 to the shader. Making my
shape was tedious, but not difficult.

The camera caused me more problems. I wasn't sure whether to start with the LookAt API
or the u-v-n-eye one, which is why there's still a lot of commented out code of me trying
different things. The roll, pitch, and yaw calculations were a lot easier when taking the
latter approach, and while LookAt made strafing and zooming conceptually easier for me,
the code ended up being simpler for u-v-n-eye for that also. The camera bounds for both
ortho and perspective are hardcoded and I wrote a method that switches between them.
Finally, getViewMatrix manually recomputes based on u-v-n-eye each time it is called.

This assignment involved a lot of trying things to see what worked, and iterating based on
what I observed in the viewport. Next time, I'll likely just hardcode all of the numbers
for the vertices and indexes.