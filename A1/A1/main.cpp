//
// CS-432 A1
// Displays a red square, blue triangle, and multi-colored circle in 2D 
//

#include "Angel.h"		// includes gl.h, glut.h and other stuff...
#include "Camera.h"		// for camera objects (for use in future assignments)
#include "Light.h"		// for lights (for use in future assignments)
#include "Square.h"		// template for drawing square
#include "Triangle.h"	// template for drawing triangle
#include "Circle.h"		// template for drawing circle

// Forward declarations
void init(void);
void display(void);
void keyboard(unsigned char, int, int);
void resize(int width, int height);
void close(void);

// Objects
Square* redSquare;
Triangle* blueTriangle;
Circle* circle;
Camera cam;
vector<Light> lights;
vector<Drawable*>drawables;

GLuint windowID=0;
//----------------------------------------------------------------------------


int main(int argc, char **argv)
{
	// initialize GULT
	glutInit(&argc, argv);  
#ifdef __APPLE__
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_SINGLE);
#else
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
#endif
	glutInitWindowSize(500, 500);

	windowID = glutCreateWindow("CS 432 A1");

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
	glutWMCloseFunc(close);
	glutReshapeFunc(resize);

	// start the main event listening loop
	glutMainLoop();
	return 0;
}

// Initialization
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	redSquare = new Square();
	drawables.push_back(redSquare);
	blueTriangle = new Triangle();
	drawables.push_back(blueTriangle);
	circle = new Circle();
	drawables.push_back(circle);
}

//----------------------------------------------------------------------------
// Display callback
void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );

	for (unsigned int i = 0; i < drawables.size(); i++)
		drawables[i]->draw(cam, lights);
	glFlush();
}

//----------------------------------------------------------------------------
// Keyboard event callback
void keyboard(unsigned char key, int x, int y)
{
    switch(key)
	{
		case 033:  // Escape key
		case 'q': case 'Q':
			close();
			break;
    }
}

//----------------------------------------------------------------------------
// Window resize callback
void resize(int w, int h)
{
	if (w < h)
		h = w;
	if (h < w)
		w = h;
	//w = 500;	// fix window dimensions to those specified by assignment
	//h = 500;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);  // make the viewport the entire window
}

void close()
{
	for (unsigned int i = 0; i < drawables.size(); i++)
		delete(drawables[i]);
	if(windowID>0)
		glutDestroyWindow(windowID);
    exit(EXIT_SUCCESS);
}
