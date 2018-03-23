#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Circle: public Drawable{

public:	
	Circle();
	~Circle();
	void draw(Camera, vector<Light>);
	
private:
	static const int step = 5;					// number of degrees per step
	static const int NumVertices = 360 / step;	// number of unique vertices
	GLuint vPosition;
	GLuint vColor;
};
#endif
