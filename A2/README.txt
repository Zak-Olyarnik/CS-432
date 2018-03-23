Zak Olyarnik
CS-432 A2

Windows 10
VS 2017
OpenGL Version: 4.5.0 - Build 22.20.16.4836
GLSL Version: 4.50 - Build 22.20.16.4836
Status: Using GLEW 2.1.0

To run, unzip and open the A2.sln file with Visual Studio.  Click the "run" button.
Output is demonstrated in the A2.mp4 file.

I started by modifying Drawable.h and Drawable.cpp.  I noticed that the files already
included a modelmatrix variable, but that it was set to be a mat4, which is not
needed for 2D graphics.  I modified this to a mat3 and added additional translation
and rotation matrices which will be combined into the modelmatrix in the draw call.
There may be a better way to do this, but since rotation must be applied first, but
translation is only available on instantiation (the user's click), I save the
translation at that time and access it later as needed.  Each Drawable also has a 
unique brightness and rotationAngle, so that the shapes can all have individual values
based on their creation time.  Because of this, I opted for increaseBrightness and
increaseRotation methods rather than direct setters.  These methods increase from 0
to 360 and 1 respectively, then set the rotation as a mat3 and brightness as a float.

I attempted the extra credit, so to minimize the number of files, I decided to make 
both the solid and multi-colored shapes use per-vertex coloring with a single vertex
shader.  This file was modified from A1 to include uniforms for the model matrix and
brightness.  The model matrix is applied directly to each vertex' position, while
brightness is a float from 0-1 representing a percentage of the full rgb value to apply.

For the Square and Triangle files, I added new private variables to hold the new
uniform locations, but the major change was to add a second constructor for each.  This
allows the specification of each vertex color for the extra credit.  The default
constructor uses the default red and blue values.  In order for this to work, I took
the rest of the code that used to be in the constructor and created a set() method.
There may be a better way to do this using default parameters, but my initial quick
tries were unsuccessful.  In fact, everything after initially setting the points and
colors arrays could probably be abstracted into yet another method, this time living
in Drawable, because that code from Square and Triangle is identical.  Perhaps next
time.  The model matrix is calculated by combining the rotation and translation matrices
in the correct order in the draw call, then passed along with the brightness to the shader.

Finally, in main.cpp, I added global variables to keep track of whether animation is
occuring or not, and the interval for calling the timer function.  Animation is toggled
in the keyboard callback by pressing spacebar.  The callback iterates through the list
of Drawables and increases rotation and brightenss of each.  Resetting from the max
value to zero is handled by each individual shape.  I also needed to add a mouse
callback to do the majority of the work here.  I looked up how to get the viewport
size, then applied the formula given in the notes to find the actual click position I
want.  Then I create my shapes based on which mouse button was clicked, and whether or
not shift was held.

This assignment was also pretty easy.  Besides some things I experimented with regarding
default constructors and parameters which didn't pan out, everything was very formulaic.
