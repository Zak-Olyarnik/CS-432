#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Square: public Drawable
{
private:
	static const int NumVertices = 4;
	GLuint vPosition;
	GLuint vColor;
	GLuint vModelMatrix;
	GLfloat fBrightness;

public:	
	Square();
	Square(vec4 color1, vec4 color2, vec4 color3, vec4 color4);
	~Square();
	void draw(Camera, vector<Light>);
	void set(vec4 color1, vec4 color2, vec4 color3, vec4 color4);
};
#endif
