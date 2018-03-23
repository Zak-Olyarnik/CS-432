#ifndef __SKYBOXCUBE_H__
#define __SKYBOXCUBE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class SkyboxCube: public Drawable
{

public:
	SkyboxCube();
	~SkyboxCube();
	void draw(Camera, vector<Light>);

private:
	// building cube
	void buildGeometry();
	void makeQuad(int ind1, int ind2, int ind3, int ind4);

	// geometry
	static const GLuint numVertices = 8;
	vec4 uniqueVertices[numVertices] = {
		vec4(-1,-1,-1,1),
		vec4(-1,-1,1,1),
		vec4(1,-1,1,1),
		vec4(1,-1,-1,1),
		vec4(-1,1,-1,1),
		vec4(-1,1,1,1),
		vec4(1,1,1,1),
		vec4(1,1,-1,1)
	};
	vec3 uniqueNormals[numVertices] = {
		normalize(vec3(-1,-1,-1)),
		normalize(vec3(-1,-1,1)),
		normalize(vec3(1,-1,1)),
		normalize(vec3(1,-1,-1)),
		normalize(vec3(-1,1,-1)),
		normalize(vec3(-1,1,1)),
		normalize(vec3(1,1,1)),
		normalize(vec3(1,1,-1))
	};
	vec4 vertices[6 * 2 * 3];

	GLubyte *skyTop, *skyBottom, *skyRight, *skyLeft, *skyFront, *skyBack;
	GLuint vModelViewMatrix, vMap;
};
#endif
