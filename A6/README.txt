Zak Olyarnik
CS-432 A6

Windows 10
VS 2017
OpenGL Version: 4.5.0 - Build 22.20.16.4836
GLSL Version: 4.50 - Build 22.20.16.4836
Status: Using GLEW 2.1.0

To run, unzip and open the A6.sln file with Visual Studio.  Click the "run" button.
Output is demonstrated in the A6.mp4 file.


I textured the plane easily using the sample code, purposely going outside of the 0-1
texture coordinates to force repeating and achieve a higher resolution.  I simplified
the cube I've been using to no longer use indices for easier texture application.  The
shaders for these objects simply passed through the texture coordinates and applied the
texture color at the end by multiplying it by the result of the complex lighting as
calculated previously.

The skybox was created as a separate drawable from the normal cube because extra
references were needed, such as to the reference images and the map location in the
shader.  We also didn't need normals or texture locations explicitly defined, and the
draw call needed to be unique as well to adjust the camera view and the depth test.
Because of the custom way I had set up my camera, I had trouble when creating the model-
view matrix, but after adjusting that and the camera's near plane, the skybox behaves as
expected.

This assignment wasn't bad overall, but it took more time than I was expecting, between
all the "gotchas" and needing to start by redefining how I built my cube.  The skybox
is really neat and will be great to use for my final.


crate texture was provided in the sample code
grass texture from https://www.sportsfieldmanagementmagazine.com/columns/turf-health/know-your-turfgrass/
Batman logo from https://www.deviantart.com/art/Batman-Logo-264015635
skybox planes from https://opengameart.org/content/sky-box-sunny-day