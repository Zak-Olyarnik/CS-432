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

private:
	// building plane
	void buildGeometry();
	void makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4);

	// geometry
	static const GLuint numVertices = 121;
	static const GLuint numIndices = 600;
	vec4 vertices[numVertices];
	vec3 normals[numVertices];
	GLubyte indices[numIndices];
};
#endif
