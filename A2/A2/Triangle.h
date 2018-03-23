#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Triangle: public Drawable
{
private:
	static const int NumVertices = 3;
	GLuint vPosition;
	GLuint vColor;
	GLuint vModelMatrix;
	GLfloat fBrightness;

public:
	Triangle();
	Triangle(vec4 color1, vec4 color2, vec4 color3);
	~Triangle();
	void draw(Camera, vector<Light>);
	void set(vec4 color1, vec4 color2, vec4 color3);
};
#endif
