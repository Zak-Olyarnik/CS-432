#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__
#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include <cassert>
#include <vector>

using namespace std;

class Drawable {
protected:
	GLuint program;
	GLuint frameProgram;
	GLuint VAO;
	GLuint frameVAO;
	GLuint VBO;
	GLuint VIO;
	mat4 modelmatrix;
	mat4 translation;
	GLuint rotationAngle = 0;

public:
	virtual void draw(Camera, vector<Light>)=0;
	virtual int testCollision(vec4 direction, vec4 origin)=0;
	void setModelMatrix(mat4 mm) { modelmatrix = mm; }
	static unsigned char* ppmRead(char* filename, int* width, int* height);
	virtual ~Drawable()=0;
};

#endif
