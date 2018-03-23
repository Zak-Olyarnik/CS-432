#ifndef __CUBE_H__
#define __CUBE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Cube: public Drawable
{

public:
	Cube();
	~Cube();
	void draw(Camera, vector<Light>);

private:
	// building cube
	void buildGeometry();
	void makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4);

	// geometry
	static const GLuint numVertices = 98;
	static const GLuint numIndices = 576;	// 6 faces * 16 quads per face * 2 triangles per quad * 3 vertices per triangle
	vec4 vertices[numVertices] = {
		// bottom layer
		vec4(-0.5,-0.5,-0.5,1),
		vec4(-0.5,-0.5,-0.25,1),
		vec4(-0.5,-0.5,0,1),
		vec4(-0.5,-0.5,0.25,1),
		vec4(-0.5,-0.5,0.5,1),
		vec4(-0.25,-0.5,0.5,1),
		vec4(0,-0.5,0.5,1),
		vec4(0.25,-0.5,0.5,1),
		vec4(0.5,-0.5,0.5,1),
		vec4(0.5,-0.5,0.25,1),
		vec4(0.5,-0.5,0,1),
		vec4(0.5,-0.5,-0.25,1),
		vec4(0.5,-0.5,-0.5,1),
		vec4(0.25,-0.5,-0.5,1),
		vec4(0,-0.5,-0.5,1),
		vec4(-0.25,-0.5,-0.5,1),
		// 2nd layer
		vec4(-0.5,-0.25,-0.5,1),
		vec4(-0.5,-0.25,-0.25,1),
		vec4(-0.5,-0.25,0,1),
		vec4(-0.5,-0.25,0.25,1),
		vec4(-0.5,-0.25,0.5,1),
		vec4(-0.25,-0.25,0.5,1),
		vec4(0,-0.25,0.5,1),
		vec4(0.25,-0.25,0.5,1),
		vec4(0.5,-0.25,0.5,1),
		vec4(0.5,-0.25,0.25,1),
		vec4(0.5,-0.25,0,1),
		vec4(0.5,-0.25,-0.25,1),
		vec4(0.5,-0.25,-0.5,1),
		vec4(0.25,-0.25,-0.5,1),
		vec4(0,-0.25,-0.5,1),
		vec4(-0.25,-0.25,-0.5,1),
		// middle layer
		vec4(-0.5,0,-0.5,1),
		vec4(-0.5,0,-0.25,1),
		vec4(-0.5,0,0,1),
		vec4(-0.5,0,0.25,1),
		vec4(-0.5,0,0.5,1),
		vec4(-0.25,0,0.5,1),
		vec4(0,0,0.5,1),
		vec4(0.25,0,0.5,1),
		vec4(0.5,0,0.5,1),
		vec4(0.5,0,0.25,1),
		vec4(0.5,0,0,1),
		vec4(0.5,0,-0.25,1),
		vec4(0.5,0,-0.5,1),
		vec4(0.25,0,-0.5,1),
		vec4(0,0,-0.5,1),
		vec4(-0.25,0,-0.5,1),
		// 4th layer
		vec4(-0.5,0.25,-0.5,1),
		vec4(-0.5,0.25,-0.25,1),
		vec4(-0.5,0.25,0,1),
		vec4(-0.5,0.25,0.25,1),
		vec4(-0.5,0.25,0.5,1),
		vec4(-0.25,0.25,0.5,1),
		vec4(0,0.25,0.5,1),
		vec4(0.25,0.25,0.5,1),
		vec4(0.5,0.25,0.5,1),
		vec4(0.5,0.25,0.25,1),
		vec4(0.5,0.25,0,1),
		vec4(0.5,0.25,-0.25,1),
		vec4(0.5,0.25,-0.5,1),
		vec4(0.25,0.25,-0.5,1),
		vec4(0,0.25,-0.5,1),
		vec4(-0.25,0.25,-0.5,1),
		// top layer
		vec4(-0.5,0.5,-0.5,1),
		vec4(-0.5,0.5,-0.25,1),
		vec4(-0.5,0.5,0,1),
		vec4(-0.5,0.5,0.25,1),
		vec4(-0.5,0.5,0.5,1),
		vec4(-0.25,0.5,0.5,1),
		vec4(0,0.5,0.5,1),
		vec4(0.25,0.5,0.5,1),
		vec4(0.5,0.5,0.5,1),
		vec4(0.5,0.5,0.25,1),
		vec4(0.5,0.5,0,1),
		vec4(0.5,0.5,-0.25,1),
		vec4(0.5,0.5,-0.5,1),
		vec4(0.25,0.5,-0.5,1),
		vec4(0,0.5,-0.5,1),
		vec4(-0.25,0.5,-0.5,1),
		// bottom face
		vec4(-0.25,-0.5,-0.25,1),
		vec4(-0.25,-0.5,0,1),
		vec4(-0.25,-0.5,0.25,1),
		vec4(0,-0.5,-0.25,1),
		vec4(0,-0.5,0,1),
		vec4(0,-0.5,0.25,1),
		vec4(0.25,-0.5,-0.25,1),
		vec4(0.25,-0.5,0,1),
		vec4(0.25,-0.5,0.25,1),
		// top face
		vec4(-0.25,0.5,-0.25,1),
		vec4(-0.25,0.5,0,1),
		vec4(-0.25,0.5,0.25,1),
		vec4(0,0.5,-0.25,1),
		vec4(0,0.5,0,1),
		vec4(0,0.5,0.25,1),
		vec4(0.25,0.5,-0.25,1),
		vec4(0.25,0.5,0,1),
		vec4(0.25,0.5,0.25,1)
	};
	vec3 normals[numVertices] = {
		// bottom layer
		normalize(vec3(-1,-1,-1)),
		normalize(vec3(-1,-1,0)),
		normalize(vec3(-1,-1,0)),
		normalize(vec3(-1,-1,0)),
		normalize(vec3(-1,-1,1)),
		normalize(vec3(0,-1,1)),
		normalize(vec3(0,-1,1)),
		normalize(vec3(0,-1,1)),
		normalize(vec3(1,-1,1)),
		normalize(vec3(1,-1,0)),
		normalize(vec3(1,-1,0)),
		normalize(vec3(1,-1,0)),
		normalize(vec3(1,-1,-1)),
		normalize(vec3(0,-1,-1)),
		normalize(vec3(0,-1,-1)),
		normalize(vec3(0,-1,-1)),
		// 2nd layer
		normalize(vec3(-1,0,-1)),
		vec3(-1,0,0),
		vec3(-1,0,0),
		vec3(-1,0,0),
		normalize(vec3(-1,0,1)),
		vec3(0,0,1),
		vec3(0,0,1),
		vec3(0,0,1),
		normalize(vec3(1,0,1)),
		vec3(1,0,0),
		vec3(1,0,0),
		vec3(1,0,0),
		normalize(vec3(1,0,-1)),
		vec3(0,0,-1),
		vec3(0,0,-1),
		vec3(0,0,-1),
		// middle layer
		normalize(vec3(-1,0,-1)),
		vec3(-1,0,0),
		vec3(-1,0,0),
		vec3(-1,0,0),
		normalize(vec3(-1,0,1)),
		vec3(0,0,1),
		vec3(0,0,1),
		vec3(0,0,1),
		normalize(vec3(1,0,1)),
		vec3(1,0,0),
		vec3(1,0,0),
		vec3(1,0,0),
		normalize(vec3(1,0,-1)),
		vec3(0,0,-1),
		vec3(0,0,-1),
		vec3(0,0,-1),
		// 4th layer
		normalize(vec3(-1,0,-1)),
		vec3(-1,0,0),
		vec3(-1,0,0),
		vec3(-1,0,0),
		normalize(vec3(-1,0,1)),
		vec3(0,0,1),
		vec3(0,0,1),
		vec3(0,0,1),
		normalize(vec3(1,0,1)),
		vec3(1,0,0),
		vec3(1,0,0),
		vec3(1,0,0),
		normalize(vec3(1,0,-1)),
		vec3(0,0,-1),
		vec3(0,0,-1),
		vec3(0,0,-1),
		// top layer
		normalize(vec3(-1,1,-1)),
		normalize(vec3(-1,1,0)),
		normalize(vec3(-1,1,0)),
		normalize(vec3(-1,1,0)),
		normalize(vec3(-1,1,1)),
		normalize(vec3(0,1,1)),
		normalize(vec3(0,1,1)),
		normalize(vec3(0,1,1)),
		normalize(vec3(1,1,1)),
		normalize(vec3(1,1,0)),
		normalize(vec3(1,1,0)),
		normalize(vec3(1,1,0)),
		normalize(vec3(1,1,-1)),
		normalize(vec3(0,1,-1)),
		normalize(vec3(0,1,-1)),
		normalize(vec3(0,1,-1)),
		// bottom face
		vec3(0,-1,0),
		vec3(0,-1,0),
		vec3(0,-1,0),
		vec3(0,-1,0),
		vec3(0,-1,0),
		vec3(0,-1,0),
		vec3(0,-1,0),
		vec3(0,-1,0),
		vec3(0,-1,0),
		// top face
		vec3(0,1,0),
		vec3(0,1,0),
		vec3(0,1,0),
		vec3(0,1,0),
		vec3(0,1,0),
		vec3(0,1,0),
		vec3(0,1,0),
		vec3(0,1,0),
		vec3(0,1,0)
	};
	GLubyte indices[numIndices];
};
#endif
