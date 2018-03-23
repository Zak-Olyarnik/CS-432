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
	void toggleTexture();

private:
	// building cube
	void buildGeometry();
	void makeQuad(int ind1, int ind2, int ind3, int ind4);

	// geometry
	static const GLuint numVertices = 8;
	vec4 uniqueVertices[numVertices] = {
		vec4(-0.5,-0.5,-0.5,1),
		vec4(-0.5,-0.5,0.5,1),
		vec4(0.5,-0.5,0.5,1),
		vec4(0.5,-0.5,-0.5,1),
		vec4(-0.5,0.5,-0.5,1),
		vec4(-0.5,0.5,0.5,1),
		vec4(0.5,0.5,0.5,1),
		vec4(0.5,0.5,-0.5,1)
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
	vec2 textures[6 * 2 * 3];
	int textureToUse = 0;
};
#endif
