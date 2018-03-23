//
// CS-432 A2
// Displays squares and triangles at position of mouse-click.
// Toggle animation of shapes' rotation and brightness using space bar.
// If shift is held, shapes are drawn multi-colored with per-vertex colors
//

#include "Angel.h"		// includes gl.h, glut.h and other stuff...
#include "Camera.h"		// for camera objects (for use in future assignments)
#include "Light.h"		// for lights (for use in future assignments)
#include "Square.h"		// template for drawing square
#include "Triangle.h"	// template for drawing triangle

// forward declarations
void init(void);
void display(void);
void keyboard(unsigned char, int, int);
void mouse(GLint button, GLint state, GLint x, GLint y);
void resize(int width, int height);
void close(void);
void timerCallback(int value);

// objects
Square* square;
Triangle* triangle;
Camera cam;
vector<Light> lights;
vector<Drawable*>drawables;

// other values
GLuint windowID = 0;
GLuint timerInterval = 50;
bool animating = false;
//----------------------------------------------------------------------------


int main(int argc, char **argv)
{
	// initialize GULT
	glutInit(&argc, argv);  
#ifdef __APPLE__
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_SINGLE);
#else
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
#endif
	glutInitWindowSize(500, 500);

	windowID = glutCreateWindow("CS 432 A2");

	// print out info about our system.  Must be done after creating a usage of GL, via glut
	fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//initialize glew on non-apple systems
#ifndef __APPLE__
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
	
	init(); // initalize objects

	// set up the callback functions
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutWMCloseFunc(close);
	glutReshapeFunc(resize);
	glutTimerFunc(50, timerCallback, 0);

	// start the main event listening loop
	glutMainLoop();
	return 0;
}

// Initialization
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
// Display callback
void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );

	for (unsigned int i = 0; i < drawables.size(); i++)
		drawables[i]->draw(cam, lights);
	glutSwapBuffers();
}

//----------------------------------------------------------------------------
// Keyboard event callback
void keyboard(unsigned char key, int x, int y)
{
    switch(key)
	{
		case 32:	// spacebar
			animating = !animating;
			break;
		case 033:  // escape key
		case 'q': case 'Q':
			close();
			break;
    }
}

//----------------------------------------------------------------------------
// Mouse event callback
void mouse(GLint button, GLint state, GLint x, GLint y)
{
	// get current viewport size
	//https://www.opengl.org/discussion_boards/showthread.php/166800-how-to-get-screen-size-in-openGL
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	GLint w = m_viewport[2];
	GLint h = m_viewport[3];
	GLfloat xCam = (2.0f * x / w) - 1;
	GLfloat yCam = (2.0f * (h - y) / h) - 1;

	int mod = glutGetModifiers();	// check for shift keypress
	vec4 red = vec4(1.0, .0, 0.0, 1.0);
	vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
	vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);
	vec4 yellow = vec4(1.0, 1.0, 0.0, 1.0);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (mod == GLUT_ACTIVE_SHIFT)	// draw multi-colored square
		{
			square = new Square(red, green, blue, yellow);
		}
		else	// draw default (red) square
		{
			square = new Square();
		}
		square->setTranslation(xCam, yCam);
		drawables.push_back(square);
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (mod == GLUT_ACTIVE_SHIFT)	// draw multi-colored triangle
		{
			triangle = new Triangle(red, green, blue);
		}
		else	// draw default (blue) triangle
		{
			triangle = new Triangle();
		}
		triangle->setTranslation(xCam, yCam);
		drawables.push_back(triangle);
	}
}

//----------------------------------------------------------------------------
// Timer callback
void timerCallback(int value)
{
	if (animating)
	{
		for (unsigned int i = 0; i < drawables.size(); i++)
		{
			drawables[i]->increaseRotation();
			drawables[i]->increaseBrightness();
		}
	}

	glutTimerFunc(timerInterval, timerCallback, value);	// reset timer
	glutPostRedisplay();		// schedule redraw
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
// Window close callback
void close()
{
	for (unsigned int i = 0; i < drawables.size(); i++)
		delete(drawables[i]);
	if(windowID>0)
		glutDestroyWindow(windowID);
    exit(EXIT_SUCCESS);
}
