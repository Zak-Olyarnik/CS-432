#ifndef __CUBE_H__
#define __CUBE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Cube: public Drawable{


public:
	Cube();
	~Cube();
	void draw(Camera, vector<Light>);

private:
	void buildCube();
	void makeQuad(int, int, int, int);
	void makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4);
	GLuint vPosition;
	GLuint vColor;
	GLuint vMVPMatrix;
	GLuint vFrameMVPMatrix;

	unsigned int index;

	vec4 vertices[8] = { 
		vec4(-0.5,-0.5,0.5,1),
		vec4(-0.5,0.5,0.5,1),
		vec4(0.5,0.5,0.5,1),
		vec4(0.5,-0.5,0.5,1),
		vec4(-0.5,-0.5,-0.5,1),
		vec4(-0.5,0.5,-0.5,1),
		vec4(0.5,0.5,-0.5,1),
		vec4(0.5,-0.5,-0.5,1) };

	vec4 potentialColors[8] = { vec4(0,0,0,1), vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1),
		vec4(1,1,0,1), vec4(1,0,1,1),vec4(0,1,1,1),vec4(0.5f,0.2f,0.4f,1) };

	GLubyte indices[6 * 2 * 3]; //= {	};
	GLubyte frameIndices[24] = {
		0,1,
		1,2,
		2,3,
		3,0,
		1,5,
		5,6,
		6,2,
		6,7,
		7,4,
		4,5,
		7,3,
		4,0
	};

	vec4 points[6 * 2 * 3]; //6 faces, 2 triangles/face, 3 vertices per triangle
	vec4 colors[6 * 2 * 3];
};
#endif
