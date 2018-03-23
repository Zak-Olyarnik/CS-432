#include "Camera.h"


Camera::Camera()
{
	view_matrix = mat4(1.0f);
	setOrthographic();	// sets view_matrix to default
	eye = vec4(0, 0, 0, 0);			// this is technically translation of world around camera
	u = vec4(1.0f, 0, 0, 0);
	v = vec4(0, 1.0f, 0, 0);
	n = vec4(0, 0, 1.0f, 0);
	
	//at = vec4(0, 0, -2.0f, 0);
	//up = vec4(0, 1.0f, 0, 0);
	//camera_matrix = LookAt(eye, eye - n, v);

}


Camera::~Camera()
{

}

mat4 Camera::getViewMatrix()
{
	view_matrix = mat4(u.x, v.x, n.x, 0.0f, 
					   u.y, v.y, n.y, 0.0f,
					   u.z, v.z, n.z, 0.0f,
					   -eye.x, -eye.y, -eye.z, 1.0f);
	
	return view_matrix;
	
	//return LookAt(eye, at, up);
}


// correct combinations of sine and cosine for the vector axes came from http://www.songho.ca/opengl/gl_anglestoaxes.html
void Camera::pitch(GLfloat theta)
{
	theta = -theta * DegreesToRadians;
	v = v * cos(theta) - n * sin(theta);
	n = v * sin(theta) + n * cos(theta);
}

void Camera::roll(GLfloat theta)
{
	theta = -theta * DegreesToRadians;
	u = u * cos(theta) - v * sin(theta);
	v = u * sin(theta) + v * cos(theta);
}

void Camera::yaw(GLfloat theta)
{
	theta = -theta * DegreesToRadians;
	u = u * cos(theta) + n * sin(theta);
	n = -u * sin(theta) + n * cos(theta);
}

void Camera::zoom(GLfloat dist)
{
	/*
		from lecture slides:
		at = eye - n
		so n = eye - at

		we want "forward" to be -n	
	*/

	//vec4 n = eye - at;
	//eye += 0.1f * dist * -n;
	//at += 0.1f * dist * -n;

	eye += dist * speed * -n;	// "forward" is -n
}

void Camera::strafe(GLfloat dist)
{
	/*
		from lecture slides:
		at = eye - n
		so n = eye - at

		u given as norm(cross(up, n))
		which makes sense because "sideways" should be perpendicular to both "up" and "forward"
	*/
	
	//vec4 n = eye - at;
	//vec4 u = normalize(cross(up, n));

	//eye += dist * 0.1f * u;
	//at += dist * 0.1f * u;

	eye += dist * speed * u;
}

void Camera::setOrthographic()
{
	// values hardcoded based on assignment parameters
	GLfloat leftBound = -1.0f;
	GLfloat bottomBound = -1.0f;
	GLfloat rightBound = 1.0f;
	GLfloat topBound = 1.0f;
	GLfloat nearBound = -10.0f;
	GLfloat farBound = 10.0f;
	projection_matrix = Ortho(leftBound, rightBound, bottomBound, topBound, nearBound, farBound);
	isOrtho = true;
}

void Camera::setPerspective()
{
	// values hardcoded based on assignment parameters
	GLfloat fovy = 65.0f;
	GLfloat aspect = 1.0f;
	GLfloat nearBound = 1.0f;
	GLfloat farBound = 100.0f;
	projection_matrix = Perspective(fovy, aspect, nearBound, farBound);
	isOrtho = false;
}

void Camera::switchProjection()
{
	if (isOrtho)
	{
		setPerspective();
	}
	else
	{
		setOrthographic();
	}
}