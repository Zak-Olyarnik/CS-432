#include "Camera.h"


Camera::Camera()
{ }

Camera::Camera(vec4 e, vec4 f, vec4 u)
{
	reset(e, f, u);
}


Camera::~Camera()
{ }

void Camera::reset(vec4 e, vec4 f, vec4 u)
{
	// view
	view_matrix = mat4(1.0f);

	eye = e;
	forward = normalize(f);
	up = normalize(u);
	view_matrix = LookAt(eye, eye + forward, up);

	right = cross(up, -forward);
	right.w = 0;	// cross() returns vec3, so manually set w to 0
	right = normalize(right);

	// projection
	setPerspective();

	// orbit
	xzOrbit = 90;
}

mat4 Camera::getViewMatrix()
{
	return LookAt(eye, eye + forward, up);
}

void Camera::setTarget(vec4 newTarget)
{
	if (is2D)
	{
		eye.z = newTarget.z;
	}
	else if (!canMove)
	{
		return;
	}
	else
	{
		newTarget.y = 2;
		eye.x = (float) (newTarget.x + 5 * cos(M_PI/180*xzOrbit));
		eye.z = (float) (newTarget.z + 5 * sin(M_PI/180*xzOrbit));
		forward = newTarget - eye;
	}
}

void Camera::set2D()
{
	is2D = true;
	canMove = false;
	setOrthographic();
}

void Camera::orbit(int dir)
{
	if (!canMove)
		return;

	xzOrbit += dir*5;
}

void Camera::setPerspective()
{
	GLfloat fovy = 65.0f;
	GLfloat aspect = 1.0f;
	GLfloat nearBound = 0.5f;
	GLfloat farBound = 100.0f;
	projection_matrix = Perspective(fovy, aspect, nearBound, farBound);
}

void Camera::setOrthographic()
{
	GLfloat leftBound = -9.0f;
	GLfloat bottomBound = -9.0f;
	GLfloat rightBound = 9.0f;
	GLfloat topBound = 9.0f;
	GLfloat nearBound = -100.0f;
	GLfloat farBound = 100.0f;
	projection_matrix = Ortho(leftBound, rightBound, bottomBound, topBound, nearBound, farBound);
}

void Camera::debug()
{
	fprintf(stdout, "eye: %f,%f,%f,%f,\n", eye.x, eye.y, eye.z, eye.w);
	fprintf(stdout, "forward: %f,%f,%f,%f,\n", forward.x, forward.y, forward.z, forward.w);
	fprintf(stdout, "up: %f,%f,%f,%f,\n", up.x, up.y, up.z, up.w);
	fprintf(stdout, "right: %f,%f,%f,%f,\n\n", right.x, right.y, right.z, right.w);
}