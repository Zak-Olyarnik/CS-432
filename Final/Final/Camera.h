#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Angel.h"

class Camera {

public:
	Camera();
	Camera(vec4 eye, vec4 forward, vec4 up);
	~Camera();
	void reset(vec4 e, vec4 f, vec4 u);
	void debug();	// prints camera information to console
	
	// view
	virtual mat4 getViewMatrix();
	vec4 getPosition() { return eye; }
	vec4 getForward() { return forward; }
	vec4 getUp() { return up; }
	vec4 getRight() { return right; }
	bool get2D() { return is2D; }
	
	void setPosition(vec4 newEye) { eye = newEye; }
	virtual void setTarget(vec4 newTarget);
	void set2D();
	void setStationary() { canMove = false; }

	// movement
	void orbit(int dir);

	// projection
	mat4 getProjectionMatrix() { return projection_matrix; }
	void setPerspective();
	void setOrthographic();

protected:
	float speed = 0.1f;		// multiplier applied to all movement
	bool is2D = false;
	bool canMove = true;
	int xzOrbit = 90;
	
	// view
	mat4 view_matrix;
	vec4 eye;		// camera position
	vec4 forward;	// forward direction vector, a.k.a. "-n", "at"
	vec4 up;		// a.k.a. "v"
	vec4 right;		// a.k.a. "u"
	
	// projection
	mat4 projection_matrix;
};

#endif
