#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Angel.h"

class Camera {

public:
	Camera();
	Camera(vec4 eye, vec4 forward, vec4 up);
	~Camera();
	void debug();	// prints camera information to console
	// view
	mat4 getViewMatrix();
	vec4 getPosition() { return eye; }
	void pitch(GLfloat theta);
	void roll(GLfloat theta);
	void yaw(GLfloat theta);
	void zoom(GLfloat dist);
	void strafe(GLfloat dist);
	void setPosition(vec4 newEye);
	// projection
	mat4 getProjectionMatrix() { return projection_matrix; }
	void setPerspective();

private:
	float speed = 0.1f;		// multiplier applied to all movement
	// view
	mat4 view_matrix;
	vec4 eye;		// camera position
	vec4 forward;	// forward direction vector, a.k.a. "n", "at"
	vec4 up;		// a.k.a. "v"
	vec4 right;		// a.k.a. "u"
	// projection
	mat4 projection_matrix;
};

#endif
