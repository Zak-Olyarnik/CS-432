#ifndef __PLANE_H__
#define __PLANE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Plane : public Drawable {


public:
	Plane();
	~Plane();
	void draw(Camera, vector<Light>);
	int testCollision(vec4 direction, vec4 origin);

private:
	void makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4);
	void buildGeometry();
	GLuint vPosition;
	GLuint vColor;
	GLuint vMVPMatrix;
	GLuint vFrameMVPMatrix;
	static const GLuint numVertices = 4;
	static const GLuint numIndices = 6;
	vec4 vertices[numVertices];		// storage for defining vertices
	GLubyte indices[numIndices];	// storage for defining indices
	unsigned int index;		// persistent counter for assigning indices in array
};
#endif
