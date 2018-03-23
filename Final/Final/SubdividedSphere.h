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
	void draw(Camera*, vector<Light>);
	void reset() override { translation = Translate(0, 1, -4.0f); }
	bool testCollision(vec4 direction, vec4 origin);
	vec4 getPosition() { return modelmatrix * vec4(0, 0, 0, 1); }
	
	// movement
	void roll(float deg, bool move);
	void jump(int dir);
	void changeLane(float dir);
	void ramp();
	void loop();

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
	float loopTheta = 270.0f;
};
#endif
