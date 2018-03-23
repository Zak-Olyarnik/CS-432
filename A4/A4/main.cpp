//
// CS-432 A4
// Displays a multi-colored sphere drawn with recursive subdivision.
// Includes picking to turn sphere's triangles black on click.
// Includes the flying camera interface from A3.
//

#include "Angel.h"		// includes gl.h, glut.h and other stuff...
#include "Camera.h"		// for camera objects
#include "Light.h"		// for lights (for use in future assignments)
#include "Plane.h"				// ground plane object
#include "SubdividedSphere.h"	// sphere object

// forward declarations
void init(void);
void display(void);
void close(void);
void resize(int width, int height);
void keyboard(unsigned char, int, int);
void specialKeys(int, int, int);
void mouse(GLint button, GLint state, GLint x, GLint y);

// objects
SubdividedSphere* sphere;
Plane* groundPlane;
Camera flyingCam;
Camera stationaryCam;
Camera* activeCam;
vector<Light> lights;
vector<Drawable*>drawables;

// other values
GLuint windowID=0;

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

	windowID = glutCreateWindow("CS-432 A4");

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
	glutMouseFunc(mouse);

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

	// set up shapes
	sphere = new SubdividedSphere();
	groundPlane = new Plane();
	drawables.push_back(sphere);
	drawables.push_back(groundPlane);

	// set up cameras
	vec4 eye(0, 2.0f, 0, 1.0f);
	vec4 forward(0, 0, -1.0f, 0);
	vec4 up(0, 1.0f, 0, 0);
	flyingCam = Camera(eye, forward, up);
	
	eye = vec4(0, 10.0f, 0, 1.0f);
	forward = vec4(0, -1.0f, 0, 0);
	up = vec4(0, 0, -1.0f, 0);
	stationaryCam = Camera(eye, forward, up);

	activeCam = &flyingCam;
}

//----------------------------------------------------------------------------
// Display callback
void display(void)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for (unsigned int i = 0; i < drawables.size(); i++)
		drawables[i]->draw(*activeCam, lights);
	
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
			// toggle cameras
			if (activeCam == &flyingCam)
				activeCam = &stationaryCam;
			else
				activeCam = &flyingCam;
			break;
		case 033:  // escape key
		case 'q': case 'Q':
			// close program
			close();
			break;
		default:
			if (activeCam == &flyingCam)	// only flying cam can move
			{
				switch (key)
				{
					case 'X':
						activeCam->pitch(1.0f);
						break;
					case 'x':
						activeCam->pitch(-1.0f);
						break;
					case 'Z':
						activeCam->roll(1.0f);
						break;
					case 'z':
						activeCam->roll(-1.0f);
						break;
					case 'C':
						activeCam->yaw(1.0f);
						break;
					case 'c':
						activeCam->yaw(-1.0f);
						break;
					case 'd':
						activeCam->debug();
						break;
				}
			}
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
// Special keys callback
void specialKeys(int key, int x, int y)
{
	if (activeCam == &flyingCam)	// only flying cam can move
	{
		switch (key)
		{
			case GLUT_KEY_UP:
				activeCam->zoom(1.0f);
				break;
			case GLUT_KEY_DOWN:
				activeCam->zoom(-1.0f);
				break;
			case GLUT_KEY_LEFT:
				activeCam->strafe(-1.0f);
				break;
			case GLUT_KEY_RIGHT:
				activeCam->strafe(1.0f);
				break;
		}
		glutPostRedisplay();
	}
}

//----------------------------------------------------------------------------
// Mouse event callback
void mouse(GLint button, GLint state, GLint x, GLint y)
{
	if (activeCam == &flyingCam && (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN))	// only flying cam can change triangle colors
	{
		// get current viewport size
		//https://www.opengl.org/discussion_boards/showthread.php/166800-how-to-get-screen-size-in-openGL
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);
		GLint w = m_viewport[2];
		GLint h = m_viewport[3];
		GLfloat xCam = (2.0f * x / w) - 1;
		GLfloat yCam = (2.0f * (h - y) / h) - 1;

		// get a ray from the camera to the clicked point
		vec4 nearPlanePoint = vec4(xCam, yCam, -1, 1);									// clicked point on the camera's near-plane
		vec4 camCoordsPoint = inverse(activeCam->getProjectionMatrix())*nearPlanePoint;	// undo projection for camera coordinates
		camCoordsPoint = camCoordsPoint / camCoordsPoint.w;								// make sure w is 1
		vec4 worldCoordsPoint = inverse(activeCam->getViewMatrix())*camCoordsPoint;		// undo view for world coordinates
		worldCoordsPoint = worldCoordsPoint / worldCoordsPoint.w;						// make sure w is 1
		vec4 ray = worldCoordsPoint - activeCam->getPosition();							// define ray from camera to clicked point

		// test the sphere object to see if any of its composing triangles were clicked
		GLint firstIndexOfClosestHitTriangle = sphere->testCollision(normalize(ray), activeCam->getPosition());
		if (firstIndexOfClosestHitTriangle >= 0)	// if we find that a triangle was clicked, use its vertex locations on the VBO to change its corresponding colors
		{
			sphere->recolorTriangle(firstIndexOfClosestHitTriangle);
		}

		glutPostRedisplay();
	}
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