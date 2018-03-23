#include "Square.h"

Square::Square()
{
	// get a vertex buffer and a vertex array object
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);				// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// associate the VBO with the active VAO

	// define vertices
	vec2 points[NumVertices] = {
		vec2(0.25, 0.25),
		vec2(0.75, 0.25),
		vec2(0.75, 0.75),
		vec2(0.25, 0.75)
	};
	
	// put the data on the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	
	// load shader program
	assert((program = InitShader("vshader_solidColor.glsl", "fshader_solidColor.glsl"))!=-1);
	glUseProgram(program);  // make it the active one

	// get the location of the vPosition attribute
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);  // enable it
	// associate the area in the active VBO with this attribute and tell it how data to pull out for each vertex from the VBO
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// get the location of the uniform color in the shader
	assert((vColor = glGetUniformLocation(program, "color"))!=-1);

}
Square::~Square() {
	// put additional things to destroy in here.  The Drawable class handles the VBO/VAO
}


void Square::draw(Camera cam, vector<Light> lights){
	// make this object's VAO active
		// this in turn makes its associated VBO active and
		// links the attributes to the shader
	glBindVertexArray(VAO); 
	glUseProgram(program);  // also switch to using this shader program

	// unfortunately, every time we draw we still need to reset all uniform variables
	vec4 red_opaque = vec4(1.0, 0.0, 0.0, 1.0);	// RGBA colors
	glUniform4fv(vColor, 1, red_opaque);

	// now that we're all set up, tell the GPU to render NumVertice vertexes as a triangle fan
		// starting at offset 0 in the VBO
	glDrawArrays(GL_TRIANGLE_FAN, 0, NumVertices);

}
