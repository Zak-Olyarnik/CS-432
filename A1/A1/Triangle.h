#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Triangle: public Drawable{


public:	
	Triangle();
	~Triangle();
	void draw(Camera, vector<Light>);
	
private:
	static const int NumVertices = 3;
	GLuint vPosition;
	GLuint vColor;
};
#endif
