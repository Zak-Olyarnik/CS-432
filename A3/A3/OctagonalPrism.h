#ifndef __OCTAGONALPRISM_H__
#define __OCTAGONALPRISM_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class OctagonalPrism : public Drawable {


public:
	OctagonalPrism();
	~OctagonalPrism();
	void draw(Camera, vector<Light>);

private:
	void makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4);
	void buildGeometry();
	GLuint vPosition;
	GLuint vColor;
	GLuint vMVPMatrix;
	GLuint vFrameMVPMatrix;
	static const GLuint numVertices = 16;
	static const GLuint numIndices = 48;

	unsigned int index;

	vec4 vertices[numVertices + 2];	// this includes the inner vertices of the front and back faces

	vec4 potentialColors[numVertices + 2] =
	{
		vec4(1,0,0,1),
		vec4(0,1,0,1),
		vec4(0,0,1,1),
		vec4(1,1,0,1),
		vec4(1,0,1,1),
		vec4(0,1,1,1),
		vec4(1,0,0,1),
		vec4(0,1,0,1),
		vec4(0,0,1,1),
		vec4(1,1,0,1),
		vec4(1,0,1,1),
		vec4(0,1,1,1),
		vec4(1,0,0,1),
		vec4(0,1,0,1),
		vec4(0,0,1,1),
		vec4(1,1,0,1),
		vec4(0,0,0,1),
		vec4(0,0,0,1)
	};

	GLubyte indices[numIndices + numIndices];		// indices for side faces + indices for front and back faces
	GLubyte frameIndices[numIndices] =		// defined manually rather than try and derive a pattern
	{
		0,1,
		1,9,
		9,8,
		8,0,
		1,2,
		2,10,
		10,9,
		2,3,
		3,11,
		11,10,
		3,4,
		4,12,
		12,11,
		4,5,
		5,13,
		13,12,
		5,6,
		6,14,
		14,13,
		6,7,
		7,15,
		15,14,
		7,0,
		8,15
	};
};
#endif
