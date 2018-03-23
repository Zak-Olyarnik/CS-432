//
// CS-432 A6
// Adds textures
//

#include "Angel.h"		// includes gl.h, glut.h and other stuff...
#include "Camera.h"		// for camera objects
#include "Light.h"		// for lights
#include "Plane.h"				// ground plane object
#include "SubdividedSphere.h"	// sphere object
#include "Cube.h"				// cube object
#include "SkyboxCube.h"			// skybox

// forward declarations
void init(void);
void display(void);
void close(void);
void resize(int width, int height);
void keyboard(unsigned char, int, int);
void specialKeys(int, int, int);
void timerCallback(int value);

// objects
SkyboxCube* skybox;
SubdividedSphere* sphere;
Plane* groundPlane;
Cube* cube;
Camera flyingCam;
vector<Light> lights;
vector<Drawable*>drawables;

// other values
GLuint windowID=0;
GLuint timerInterval = 50;
GLuint rotationCounter = 0;

//----------------------------------------------------------------------------
// Main
int main(int argc, char **argv)
{
	// initialize GLUT
	glutInit(&argc, argv);
#ifdef __APPLE__
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_SINGLE);
#else
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
	glutInitWindowSize(512, 512);

	windowID = glutCreateWindow("CS-432 A6");

	// print out info about our system.  Must be done after creating a usage of GL, via glut
	fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// initialize glew on non-apple systems
#ifndef __APPLE__
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

	init(); //initalize objects

	// set up the callback functions
	glutDisplayFunc(display);
	glutWMCloseFunc(close);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutTimerFunc(timerInterval, timerCallback, 0);

	// start the main event listening loop
	glutMainLoop();
	return 0;
}

//----------------------------------------------------------------------------
// Initialization
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	// set up skybox, make sure this is rendered first
	skybox = new SkyboxCube();
	drawables.push_back(skybox);

	// set up shapes
	sphere = new SubdividedSphere();
	sphere->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
	cube = new Cube();
	cube->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
	groundPlane = new Plane();
	groundPlane->setMaterial(vec4(0, 1, 0, 1), vec4(0.1f, 0.35f, 0.1f, 1), vec4(0.45f, 0.55f, 0.45f, 1), 32);
	drawables.push_back(sphere);
	drawables.push_back(cube);
	drawables.push_back(groundPlane);

	// set up cameras
	vec4 eye(0, 2.0f, 0, 1.0f);
	vec4 forward(0, 0, -1.0f, 0);
	vec4 up(0, 1.0f, 0, 0);
	flyingCam = Camera(eye, forward, up);

	// flashlight
	lights.push_back(Light(vec4(0, 0, 0, 1), vec4(0, 0, -1, 0), vec4(1, 1, 1, 1), vec4(0.2f, 0.2f, 0.2f, 1), vec4(1, 1, 1, 1)));
	// sun
	lights.push_back(Light(vec4(0, 0, 0, 0), vec4(0, -1, 0, 1), vec4(.5, .5, .5, 1), vec4(0.75f, 0.75f, 0.75f, 1), vec4(1, 1, 1, 1)));
}

//----------------------------------------------------------------------------
// Display callback
void display(void)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for (unsigned int i = 0; i < drawables.size(); i++)
		drawables[i]->draw(flyingCam, lights);
	
	glutSwapBuffers();
}

//----------------------------------------------------------------------------
// Window resize callback
void resize(int w, int h)
{
	if (w < h)
		h = w;
	if (h < w)
		w = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);  // make the viewport the entire window
}

//----------------------------------------------------------------------------
// Keyboard callback
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 32:	// spacebar
			lights[0].toggleEnabled();
			break;
		case 033:  // escape key
		case 'q':
		case 'Q':
			// close program
			close();
			break;
		case 'X':
			flyingCam.pitch(1.0f);
			break;
		case 'x':
			flyingCam.pitch(-1.0f);
			break;
		case 'Z':
			flyingCam.roll(1.0f);
			break;
		case 'z':
			flyingCam.roll(-1.0f);
			break;
		case 'C':
			flyingCam.yaw(1.0f);
			break;
		case 'c':
			flyingCam.yaw(-1.0f);
			break;
		case 'd':
			flyingCam.debug();
			break;
		case 't':
		case 'T':
			// toggle cube texture
			cube->toggleTexture();
			break;
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
// Special keys callback
void specialKeys(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			flyingCam.zoom(1.0f);
			break;
		case GLUT_KEY_DOWN:
			flyingCam.zoom(-1.0f);
			break;
		case GLUT_KEY_LEFT:
			flyingCam.strafe(-1.0f);
			break;
		case GLUT_KEY_RIGHT:
			flyingCam.strafe(1.0f);
			break;
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
// Timer callback
void timerCallback(int value)
{
	// rotate sun
	lights[1].setDirection(RotateZ(1)*lights[1].getDirection());
	rotationCounter++;
	if (rotationCounter == 360)
	{
		rotationCounter = 0;
	}
	if (rotationCounter == 90 || rotationCounter == 270)
	{
		lights[1].toggleEnabled();
	}

	glutTimerFunc(timerInterval, timerCallback, value);	// reset timer
	glutPostRedisplay();		// schedule redraw
}

//----------------------------------------------------------------------------
// Window close callback
void close()
{
	for (unsigned int i = 0; i < drawables.size(); i++)
		delete(drawables[i]);

	if(windowID>0)
			glutDestroyWindow(windowID);

    exit(EXIT_SUCCESS);
}