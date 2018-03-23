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
	GLuint VAO;
	GLuint VBO;
	GLuint VIO;
	GLuint program;
	mat4 modelmatrix;
	mat4 translation;
	mat4 scale;
	unsigned int index = 0;		// persistent counter for assigning indices in array

	// shader variables	
	GLuint vPosition, vNormal;
	GLuint vModelMatrix, vViewMatrix, vProjectionMatrix;
	GLuint vDiffuse, vSpecular, vAmbient, vAlpha;
	GLuint vFlashLightDiffuse, vFlashLightSpecular, vFlashLightAmbient, vFlashLightEnabled, vFlashLightAngle;
	GLuint vSunLightDir, vSunLightDiffuse, vSunLightSpecular, vSunLightAmbient, vSunLightEnabled;

	// material properties
	vec4 diffuse, specular, ambient;
	GLfloat shine;

public:
	virtual void draw(Camera, vector<Light>)=0;
	void setModelMatrix(mat4 mm) { modelmatrix = mm; }
	void setMaterial(vec4 a, vec4 d, vec4 s, float sh);
	static unsigned char* ppmRead(char* filename, int* width, int* height);
	virtual ~Drawable()=0;
};

#endif
