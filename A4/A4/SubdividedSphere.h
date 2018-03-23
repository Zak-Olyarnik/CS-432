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
	int testCollision(vec4 direction, vec4 origin);
	void recolorTriangle(int firstIndex);

private:
	void tetrahedron(int);
	void divideTriangle(const vec4&, const vec4&, const vec4&, int);
	void triangle(const vec4&, const vec4&, const vec4&);
	vec4 unit(const vec4&);
	
	GLuint vPosition;
	GLuint vColor;
	GLuint vMVPMatrix;
	static const GLuint numVertices = 3072;	// (4 triangular faces per tetrahedron)^(numDivisions+1)*3 vertices per triangle 
	unsigned int index = 0;
	GLfloat sqrt2 = (float)sqrt(2.0);
	GLfloat sqrt6 = (float)sqrt(6.0);

	vec4 vertices[numVertices];
	vec4 colors[numVertices];
};
#endif
