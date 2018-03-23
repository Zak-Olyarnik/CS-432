#ifndef __SKYPLANE_H__
#define __SKYPLANE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include "SkyboxCube.h"
#include <string>
#include <vector>

using namespace std;

class SkyPlane : public Drawable {

public:
	SkyPlane();
	~SkyPlane();
	void draw(Camera*, vector<Light>);

private:
	// building plane
	void buildGeometry();
	void makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4);

	// geometry
	static const GLuint numVertices = 4;
	static const GLuint numIndices = 6;
	vec4 vertices[numVertices] =
	{
		vec4(-0.5f, 0, -0.5f, 1),
		vec4(-0.5f, 0, 0.5f, 1),
		vec4(0.5f, 0, 0.5f, 1),
		vec4(0.5f, 0, -0.5f, 1)
	};
	vec2 textures[numVertices] =
	{
		vec2(0,0),
		vec2(0,1),
		vec2(1,1),
		vec2(1,0)
	};
	GLubyte indices[numIndices];

	SkyboxCube* skybox;
};
#endif
