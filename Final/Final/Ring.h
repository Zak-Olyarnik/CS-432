#ifndef __RING_H__
#define __RING_H__

#include "Angel.h"
#include "Drawable.h"
#include "SubdividedSphere.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Ring : public Drawable {

public:
	Ring();
	Ring(SubdividedSphere* s);
	~Ring();
	void draw(Camera*, vector<Light>);
	void reset() override;

	// movement
	void spin(GLfloat deg);

private:
	// building sphere
	void buildGeometry();
	void makeQuad(vec4 p1, vec4 p2, vec4 p3, vec4 p4);
	void setPositions();
	bool testCollison(int instance);

	// geometry
	GLfloat outerRadius = 0.75f;
	GLfloat innerRadius = 0.45f;
	GLfloat r1 = (outerRadius - innerRadius) / 2.0f;
	GLfloat r0 = innerRadius + r1;
	static const GLint levelOfDetail = 2;
	static const GLuint numVertices = (360 / levelOfDetail) * (360 / levelOfDetail) * 6;	// (72 increments of theta * 72 increments of phi) faces * 2 triangles per face * 3 vertices per triangle
	vec4 vertices[numVertices];
	vec3 normals[numVertices];

	static const int numInstances = 27;
	mat4 positions[numInstances];
	bool collected[numInstances] = { false };
	SubdividedSphere* player;
};
#endif
