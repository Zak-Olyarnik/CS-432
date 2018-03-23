#include "Camera.h"


Camera::Camera()
{
	// view
	view_matrix = mat4(1.0f);

	eye = vec4(0, 0, 0, 1.0f);
	forward = vec4(0, 0, -1.0f, 0.0f);
	up = vec4(0, 1.0f, 0, 0);
	view_matrix = LookAt(eye, eye + forward, up);

	right = cross(up, -forward);
	right.w = 0;	// cross() returns vec3, so manually set w to 0
	right = normalize(right);

	// projection
	setPerspective();
}

Camera::Camera(vec4 e, vec4 f, vec4 u)
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
}


Camera::~Camera()
{

}

mat4 Camera::getViewMatrix()
{
	return LookAt(eye, eye + forward, up);
}

void Camera::debug()
{
	fprintf(stdout, "eye: %f,%f,%f,%f,\n", eye.x, eye.y, eye.z, eye.w);
	fprintf(stdout, "forward: %f,%f,%f,%f,\n", forward.x, forward.y, forward.z, forward.w);
	fprintf(stdout, "up: %f,%f,%f,%f,\n", up.x, up.y, up.z, up.w);
	fprintf(stdout, "right: %f,%f,%f,%f,\n\n", right.x, right.y, right.z, right.w);
}

void Camera::setPosition(vec4 newEye)
{ 
	eye = newEye;
}

// correct combinations of sine and cosine for the vector axes came from http://www.songho.ca/opengl/gl_anglestoaxes.html
void Camera::pitch(GLfloat theta)
{
	theta = -theta * DegreesToRadians;
	vec4 up0 = up * cos(theta) + forward * sin(theta);
	forward = -up * sin(theta) + forward * cos(theta);

	up = up0;
}

void Camera::roll(GLfloat theta)
{
	theta = -theta * DegreesToRadians;
	vec4 right0 = right * cos(theta) - up * sin(theta);
	up = right * sin(theta) + up * cos(theta);
	right = right0;
}

void Camera::yaw(GLfloat theta)
{
	theta = -theta * DegreesToRadians;
	vec4 right0 = right * cos(theta) - forward * sin(theta);
	forward = right * sin(theta) + forward * cos(theta);
	right = right0;
}

void Camera::zoom(GLfloat dist)
{
	eye += dist * speed * forward;
}

void Camera::strafe(GLfloat dist)
{
	eye += dist * speed * right;
}

void Camera::setPerspective()
{
	// values hardcoded based on assignment parameters
	GLfloat fovy = 65.0f;
	GLfloat aspect = 1.0f;
	GLfloat nearBound = 1.0f;
	GLfloat farBound = 100.0f;
	projection_matrix = Perspective(fovy, aspect, nearBound, farBound);
}