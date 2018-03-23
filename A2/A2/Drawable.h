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
	GLuint VAO;
	GLuint VBO;
	mat3 translation;
	mat3 rotation;
	mat3 modelmatrix;
	GLfloat brightness = 1.0f;
	GLuint rotationAngle = 0;
	
public:
	virtual void draw(Camera, vector<Light>)=0;
	void setModelMatrix(mat3 mm) { modelmatrix = mm; }
	void setTranslation(GLfloat x, GLfloat y) { translation = mat3(vec3(1, 0, x), vec3(0, 1, y), vec3(0, 0, 1)); }
	void increaseRotation();
	void increaseBrightness();
	static unsigned char* ppmRead(char* filename, int* width, int* height);
	virtual ~Drawable()=0;
};

#endif
