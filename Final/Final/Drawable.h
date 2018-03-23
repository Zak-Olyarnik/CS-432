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
	float rotation;
	unsigned int index = 0;		// persistent counter for assigning indices in array

	// shader variables	
	GLuint vPosition, vNormal, vTexture;
	GLuint vModelMatrix, vViewMatrix, vProjectionMatrix;	// matrices
	GLuint vDiffuse, vSpecular, vAmbient, vAlpha;	// materials
	GLuint vSunLightDir, vSunLightDiffuse, vSunLightSpecular, vSunLightAmbient;	// lighting
	GLuint vMap;	// cube map texturing

	// material properties
	vec4 diffuse, specular, ambient;
	GLfloat shine;

	// texture properties
	GLuint textureIDs;
	GLubyte *image;
	int imgSize;

public:
	virtual void draw(Camera*, vector<Light>)=0;
	virtual void reset() {}
	void setModelMatrix(mat4 mm) { modelmatrix = mm; }
	void setMaterial(vec4 a, vec4 d, vec4 s, float sh);
	void setTranslation(mat4 t) { translation = t; }
	mat4 getTranslation() { return translation; }
	static unsigned char* ppmRead(char* filename, int* width, int* height);
	virtual ~Drawable()=0;
};

#endif
