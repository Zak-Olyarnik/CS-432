//
// CS-432 Final
// Sonic the Hedgehog!
// Control Sonic's lane with the arrow keys.  Jump by clicking on him.
// Control the camera (orbiting around Sonic) with a/s, or switch to 2D using space.
// Run over rings to collect them.
// Includes texturing and skybox, "faked" instancing for rings and both click and 
//		bounding sphere collision detection.
//

#include "Angel.h"		// includes gl.h, glut.h and other stuff...
#include "Camera.h"		// for camera objects
#include "Light.h"		// for lights (for use in future assignments)
#include "SubdividedSphere.h"	// sphere object
#include "Ground.h"				// ground object
#include "Loop.h"				// loop object
#include "Ring.h"				// ring objects
#include "SkyboxCube.h"			// skybox
#include "SkyPlane.h"			// skyplane, for 2D

// forward declarations
void init(void);
void display(void);
void close(void);
void resize(int width, int height);
void keyboard(unsigned char, int, int);
void specialKeys(int, int, int);
void mouse(GLint button, GLint state, GLint x, GLint y);
void timerCallback(int value);

// objects
SubdividedSphere* sphere;
Ground* ground;
Loop* loop;
Ring* ring;
SkyboxCube* skybox;
SkyPlane* skyplane;
Camera playerCam;
Camera sideCam;
Camera loopCam;
Camera* activeCam;
vector<Light> lights;
vector<Drawable*>drawables;

// animation timers
GLuint timerInterval = 10;				// time (ms) between callbacks
GLint timer = 0;						// total time elapsed, used for reset
GLint resetTime = (int)(27.5 * 1000);	// total time length of animation
GLint camSwitchTime = 23 * 1000;		// time to auto-switch to stationary cam
GLint rampTime = 24 * 1000;				// time when sphere reaches the ramp
GLint loopTime = (int)(24.4 * 1000);	// time when sphere reaches the loop
GLint offLoopTime = (int)(26.6 * 1000);	// time when sphere exits the loop
bool onRamp = false;		// flag for being on the ramp (to control special movement)
bool onLoop = false;		// flag for being on the loop (to control special movement)

// movement
GLfloat rollSpeed = 7.0f;	// sphere roll speed
GLuint jumpTimer = 0;		// timer allowing jump
GLuint maxJump = 35;		// "height" of jump

// lane switching
GLuint currentLane = 0;				// controls x-position
GLfloat changeLane = 0;				// timer allowing lane change
GLfloat changeLaneSpeed = 0.0625f;	// speed of lane change

// other values
GLuint windowID=0;
GLfloat spinSpeed = 1.5f;	// ring spin speed


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

	windowID = glutCreateWindow("CS-432 Final");

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
	skyplane = new SkyPlane();
	drawables.push_back(skyplane);

	// set up other geometry
	sphere = new SubdividedSphere();
	drawables.push_back(sphere);
	ground = new Ground();
	drawables.push_back(ground);
	loop = new Loop();
	drawables.push_back(loop);
	ring = new Ring(sphere);
	drawables.push_back(ring);

	// set up cameras
	vec4 eye(0, 4.0f, 0, 1.0f);
	vec4 forward(0, 0, -1.0f, 0);
	vec4 up(0, 1.0f, 0, 0);
	playerCam = Camera(eye, forward, up);	// orbiting camera
	
	eye = vec4(50.0f, 0, 0, 1.0f);
	forward = vec4(-1.0f, 0, 0, 0);
	up = vec4(0, 1.0f, 0, 0);
	sideCam = Camera(eye, forward, up);
	sideCam.set2D();						// 2D camera

	eye = vec4(16.773159f, 4.243996f, -497.122711f, 1.0f);
	forward = vec4(-0.912261f, 0.051681f, 0.406338f, 0);
	up = vec4(0.041521f, 0.998566f, -0.033788f, 0);
	loopCam = Camera(eye, forward, up);
	loopCam.setStationary();				// stationary camera, fixed on loop

	activeCam = &playerCam;

	// sun
	lights.push_back(Light(vec4(0, 0, 0, 0), vec4(-0.5, -1.5, -0.5, 0), vec4(.25f, .25f, .25f, 1), vec4(0.5f, 0.5f, 0.5f, 1), vec4(0.95f, 0.95f, 0.95f, 1)));
}

//----------------------------------------------------------------------------
// Display callback
void display(void)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for (unsigned int i = 0; i < drawables.size(); i++)
		drawables[i]->draw(activeCam, lights);
	
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
			if (activeCam == &playerCam)
				activeCam = &sideCam;
			else if (activeCam == &sideCam)
				activeCam = &playerCam;
			break;
		case 033:  // escape key
		case 'q': case 'Q':
			// close program
			close();
			break;
		case 'x':
			activeCam->orbit(-1);
			break;
		case 'z':
			activeCam->orbit(1);
			break;
		case 'd':
			activeCam->debug();
			break;
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
// Special keys callback
void specialKeys(int key, int x, int y)
{
	if (activeCam == &loopCam)
		return;

	switch (key)	// controls left and right movement of sphere, sensitive to relative positions of sphere and camera
	{
		case GLUT_KEY_LEFT:
			if ((currentLane == 0 || currentLane == 1) && activeCam->getPosition().z > sphere->getPosition().z)
				changeLane = -1;
			else if ((currentLane == 0 || currentLane == -1) && activeCam->getPosition().z < sphere->getPosition().z)
				changeLane = 1;
			break;
		case GLUT_KEY_RIGHT:
			if ((currentLane == 0 || currentLane == -1) && activeCam->getPosition().z > sphere->getPosition().z)
				changeLane = 1;
			else if ((currentLane == 0 || currentLane == 1) && activeCam->getPosition().z < sphere->getPosition().z)
				changeLane = -1;
			break;
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
// Mouse event callback
void mouse(GLint button, GLint state, GLint x, GLint y)
{
	if (activeCam == &loopCam)
		return;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		bool clicked = false;

		// get current viewport size
		//https://www.opengl.org/discussion_boards/showthread.php/166800-how-to-get-screen-size-in-openGL
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);
		GLint w = m_viewport[2];
		GLint h = m_viewport[3];
		GLfloat xCam = (2.0f * x / w) - 1;
		GLfloat yCam = (2.0f * (h - y) / h) - 1;
		
		vec4 nearPlanePoint = vec4(xCam, yCam, -1, 1);									// clicked point on the camera's near-plane
		vec4 camCoordsPoint = inverse(activeCam->getProjectionMatrix())*nearPlanePoint;	// undo projection for camera coordinates
		camCoordsPoint = camCoordsPoint / camCoordsPoint.w;								// make sure w is 1
		vec4 worldCoordsPoint = inverse(activeCam->getViewMatrix())*camCoordsPoint;		// undo view for world coordinates
		worldCoordsPoint = worldCoordsPoint / worldCoordsPoint.w;						// make sure w is 1

		if (activeCam == &playerCam)
		{
			vec4 ray = worldCoordsPoint - activeCam->getPosition();						// define ray from camera to clicked point
			clicked = sphere->testCollision(normalize(ray), activeCam->getPosition());	// test the sphere object to see if any of its composing triangles were clicked
		}
		if (activeCam == &sideCam)
		{
			// https://www.reddit.com/r/gamedev/comments/1lpgju/is_raycasting_possible_with_an_orthographic/
			clicked = sphere->testCollision(vec4(-1.0f, 0, 0, 0), worldCoordsPoint);	// for ortho, ray direction is always the same and origin is the click location
		}

		if (clicked)
		{
			if (jumpTimer == 0)
			{
				jumpTimer = 1;
			}
		}

		glutPostRedisplay();
	}
}

//----------------------------------------------------------------------------
// Timer callback
void timerCallback(int value)
{
	timer += timerInterval;
	if (timer % resetTime == 0)		// reset at end of course
	{
		activeCam = &playerCam;
		vec4 eye(0, 4.0f, 0, 1.0f);
		vec4 forward(0, 0, -1.0f, 0);
		vec4 up(0, 1.0f, 0, 0);
		activeCam->reset(eye, forward, up);
		currentLane = 0;
		for (unsigned int i = 0; i < drawables.size(); i++)
			drawables[i]->reset();
	}

	if (timer % resetTime == camSwitchTime)	// auto-switch camera to view loop
	{
		mat4 newt;
		newt[0][3] = 5.28f;
		newt[1][3] = sphere->getTranslation()[1][3];
		newt[2][3] = sphere->getTranslation()[2][3];
		sphere->setTranslation(newt);
		activeCam = &loopCam;
	}

	if (timer % resetTime == rampTime)	// sphere has reached ramp
	{
		onRamp = true;
	}

	if (timer % resetTime == loopTime)	// sphere has reached loop
	{
		onRamp = false;
		onLoop = true;
	}

	if (timer % resetTime == offLoopTime)	// sphere has exited loop
	{
		onLoop = false;
	}

	if (onRamp)
	{
		sphere->ramp();		// manually adjust movement to make sphere go up ramp
		sphere->roll(rollSpeed, true);	// constant rolling and movement of sphere
	}
	else if (onLoop)
	{
		sphere->roll(rollSpeed, false);	// constant rotation of sphere (no movement)
		sphere->loop();	// manually adjust movement to make sphere go around loop
	}
	else
	{
		sphere->roll(rollSpeed, true);	// constant rolling and movement of sphere
	}

	
	if (jumpTimer != 0)			// jump animation
	{
		if (jumpTimer == maxJump)
		{
			jumpTimer *= -1;
			jumpTimer++;
		}
		sphere->jump(jumpTimer);
		jumpTimer++;
	}

	if (changeLane != 0)	// lane change animation
	{
		sphere->changeLane(changeLane);
		if ((changeLane - (changeLaneSpeed * (changeLane / abs(changeLane)))) == 0)
		{
			currentLane += (GLuint)(changeLane / abs(changeLane));
		}
		changeLane -= changeLaneSpeed * (changeLane/abs(changeLane));
	}

	ring->spin(spinSpeed);		// constant spinning of rings

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