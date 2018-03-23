//
// CS-432 A3
// Displays a multi-colored, rotating octagonal prism.
// Camera view can be controlled using keyboard input.
//

#include "Angel.h"		// includes gl.h, glut.h and other stuff...
#include "Camera.h"		// for camera objects
#include "Light.h"		// for lights (for use in future assignments)
//#include "Cube.h"		// cube object
#include "OctagonalPrism.h"		// drawn shape

// forward declarations
void init(void);
void display(void);
void close(void);
void resize(int width, int height);
void keyboard(unsigned char, int, int);
void specialKeys(int, int, int);
void rotationCallback(int value);

// objects
//Cube* mbox;
OctagonalPrism* shape;
Camera cam;
vector<Light> lights;
vector<Drawable*>drawables;

// other values
GLuint windowID=0;
GLuint timerInterval = 10;
bool animating = false;

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

	windowID = glutCreateWindow("CS 432 A3");

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
	glutTimerFunc(timerInterval, rotationCallback, 0);

	//start the main event listening loop
	glutMainLoop();
	return 0;
}

//----------------------------------------------------------------------------
// Initialization
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	//mbox = new Cube();
	//mbox->setModelMatrix(RotateX(45)*RotateY(10));
	shape = new OctagonalPrism();

	//drawables.push_back(mbox);
	drawables.push_back(shape);
}

//----------------------------------------------------------------------------
// Display callback
void display(void)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for (unsigned int i = 0; i < drawables.size(); i++)
		drawables[i]->draw(cam, lights);
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
	switch (key)
	{
		case 32:	// spacebar
			animating = !animating;
			break;
		case 'X':
			cam.pitch(1.0f);
			break;
		case 'x':
			cam.pitch(-1.0f);
			break;
		case 'Z':
			cam.roll(1.0f);
			break;
		case 'z':
			cam.roll(-1.0f);
			break;
		case 'C':
			cam.yaw(1.0f);
			break;
		case 'c':
			cam.yaw(-1.0f);
			break;
		case 'p': case 'P':
			cam.switchProjection();
			break;
		case 033:  // Escape key
		case 'q': case 'Q':
			close();
			break;
	}
}

//----------------------------------------------------------------------------
// Special keys callback
void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		cam.zoom(1.0f);
		break;
	case GLUT_KEY_DOWN:
		cam.zoom(-1.0f);
		break;
	case GLUT_KEY_LEFT:
		cam.strafe(-1.0f);
		break;
	case GLUT_KEY_RIGHT:
		cam.strafe(1.0f);
		break;
	}

	//glutPostRedisplay();
}

//----------------------------------------------------------------------------
// Rotation callback
void rotationCallback(int value)
{
	if (animating)
	{
		for (unsigned int i = 0; i < drawables.size(); i++)
		{
			drawables[i]->increaseRotation();
		}
	}

	glutTimerFunc(timerInterval, rotationCallback, value);	// reset timer
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