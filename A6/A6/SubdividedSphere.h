#ifndef __SUBDIVIDEDSPHERE_H__
#define __SUBDIVIDEDSPHERE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class SubdividedSphere : public Drawable {

public:
	SubdividedSphere();
	~SubdividedSphere();
	void draw(Camera, vector<Light>);

private:
	// building sphere
	void tetrahedron(int);
	void divideTriangle(const vec4&, const vec4&, const vec4&, int);
	void triangle(const vec4&, const vec4&, const vec4&);
	vec4 unit(const vec4&);

	// geometry
	static const GLuint numVertices = 3072;	// (4 triangular faces per tetrahedron)^(numDivisions+1)*3 vertices per triangle 
	GLfloat sqrt2 = (float)sqrt(2.0);
	GLfloat sqrt6 = (float)sqrt(6.0);
	vec4 vertices[numVertices];
	vec3 normals[numVertices];
};
#endif
