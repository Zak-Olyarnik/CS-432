#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Angel.h"

class Camera {

public:
	Camera();
	~Camera();
	// view
	mat4 getViewMatrix();
	void pitch(GLfloat theta);
	void roll(GLfloat theta);
	void yaw(GLfloat theta);
	void zoom(GLfloat dist);
	void strafe(GLfloat dist);
	// projection
	mat4 getProjectionMatrix() { return projection_matrix; }
	void setOrthographic();
	void setPerspective();
	void switchProjection();

private:
	float speed = 0.1f;
	// view
	mat4 view_matrix;
	vec4 eye;
	vec4 u;
	vec4 v;	// this is "up"
	vec4 n;
	//vec4 at;
	//vec4 up;
	// projection
	mat4 projection_matrix;
	bool isOrtho;




};

#endif
