#ifndef __GROUND_H__
#define __GROUND_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Ground: public Drawable
{

public:
	Ground();
	~Ground();
	void draw(Camera*, vector<Light>);

private:
	// building cube
	void buildGeometry();
	void makeQuad(int ind1, int ind2, int ind3, int ind4);

	// geometry
	float halfWidth = 8, halfHeight = 5, halfLength = 250;
	static const GLuint numVertices = 8;
	vec4 uniqueVertices[numVertices] = {
		vec4(-halfWidth,-halfHeight,-halfLength,1),
		vec4(-halfWidth,-halfHeight,halfLength,1),
		vec4(halfWidth,-halfHeight,halfLength,1),
		vec4(halfWidth,-halfHeight,-halfLength,1),
		vec4(-halfWidth,halfHeight,-halfLength,1),
		vec4(-halfWidth,halfHeight,halfLength,1),
		vec4(halfWidth,halfHeight,halfLength,1),
		vec4(halfWidth,halfHeight,-halfLength,1)
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
	vec3 normals[6 * 2 * 3];
	vec2 textures[6 * 2 * 3] = {
		vec2(0,1),	// left face
		vec2(37.5f,1),
		vec2(37.5f,0.25f),
		vec2(37.5f,0.25f),
		vec2(0,0.25f),
		vec2(0,1),
		vec2(0,1),	// back face
		vec2(1.2f,1),
		vec2(1.2f,0.25f),
		vec2(1.2f,0.25f),
		vec2(0,0.25f),
		vec2(0,1),
		vec2(0,1),	// right face
		vec2(37.5f,1),
		vec2(37.5f,0.25f),
		vec2(37.5f,0.25f),
		vec2(0,0.25f),
		vec2(0,1),
		vec2(0,1),	// front face
		vec2(1.2f,1),
		vec2(1.2f,0.25f),
		vec2(1.2f,0.25f),
		vec2(0,0.25f),
		vec2(0,1),
		vec2(0,0),	// bottom face
		vec2(0,0.25f),
		vec2(10,0.25f),
		vec2(10,0.25f),
		vec2(10,0),
		vec2(0,0),
		vec2(0,0),	// top face
		vec2(0,0.25f),
		vec2(10,0.25f),
		vec2(10,0.25f),
		vec2(10,0),
		vec2(0,0)
	};
};
#endif
