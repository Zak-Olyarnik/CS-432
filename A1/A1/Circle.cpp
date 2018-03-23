#include "Circle.h"
#include <ctime>

Circle::Circle()
{
	// get a vertex buffer and a vertex array object
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);				// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// associate the VBO with the active VAO

	// define vertices
	vec2 points[NumVertices + 2];	// +2 necessary to correctly draw the triangle fan; we must include both
	points[0] = vec2(0, 0);				// the circle's center (0,0) and repeat the first point at the end to close the shape
	for (int i = 1; i < NumVertices+2; i++)
	{
		GLfloat x = 0.2f * (float)cos(i * step * M_PI / 180);
		GLfloat y = 0.2f * (float)sin(i * step * M_PI / 180);
		points[i] = vec2(x, y);
	}

	// define colors
	vec4 colors[NumVertices + 2];
	srand(time(0));			// http://www.cplusplus.com/forum/beginner/5131/
	for (int i = 0; i < NumVertices + 2; i++)
	{
		// https://stackoverflow.com/questions/9878965/rand-between-0-and-1
		GLfloat r = ((float)rand() / (RAND_MAX));
		GLfloat g = ((float)rand() / (RAND_MAX));
		GLfloat b = ((float)rand() / (RAND_MAX));
		colors[i] = vec4(r, g, b, 1.0);
	}

	// put the data on the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
	
	// load shader program
	assert((program = InitShader("vshader_multiColor.glsl", "fshader_multiColor.glsl"))!=-1);
	glUseProgram(program);  // make it the active one

	// get the location of the vPosition attribute in the shader
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);  // enable it
	// associate the area in the active VBO with this attribute and tell it how data to pull out for each vertex from the VBO
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// get the location of the cPosition attribute in the shader
	assert((vColor = glGetAttribLocation(program, "vColor"))!=-1);
	glEnableVertexAttribArray(vColor);  // enable it
	// associate the area in the active VBO with this attribute and tell it how data to pull out for each vertex from the VBO
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

}
Circle::~Circle() {
	// put additional things to destroy in here.  The Drawable class handles the VBO/VAO
}


void Circle::draw(Camera cam, vector<Light> lights){
	// make this object's VAO active.  This in turn makes its associated VBO active and
		// links the attributes to the shader
	glBindVertexArray(VAO); 
	glUseProgram(program);  // also switch to using this shader program

	// now that we're all set up, tell the GPU to render NumVertice vertexes as a triangle fan
		// starting at offset 0 in the VBO
	glDrawArrays(GL_TRIANGLE_FAN, 0, NumVertices + 2);	// don't forget the +2 here as well!

}
