#include "Cube.h"

Cube::Cube()
{
	// set shape defaults
	translation = Translate(0, 0, 0);	// mat.h Translate has been modified to return
	setModelMatrix(translation);				// the correct translation matrix
	rotationAngle = 0;

	buildCube();

	// get buffers for the attributes and indices
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VIO);

	// put the vertex and color data on the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(potentialColors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(potentialColors), potentialColors);

	// put the index data on the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) + sizeof(frameIndices), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), sizeof(frameIndices), frameIndices);

	// get a vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);				// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// associate the VBO with the active VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);

	// load shader program
	assert((program = InitShader("vshader00_v150.glsl", "fshader00_v150.glsl"))!=-1);
	glUseProgram(program);

	// get the location of the vPosition attribute in the shader
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// get the location of the vColor attribute in the shader
	assert((vColor = glGetAttribLocation(program, "vColor"))!=-1);
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	// get the location of the model-view-projection matrix in the shader
	assert((vMVPMatrix = glGetUniformLocation(program, "mvp_matrix")) != -1);


	// repeat with a new VAO for the frame
	glGenVertexArrays(1, &frameVAO);
	glBindVertexArray(frameVAO);		// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// associate the VBO with the active VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);

	// load shader program
	assert((frameProgram = InitShader("vshader01_v150.glsl", "fshader00_v150.glsl")) != -1);
	glUseProgram(frameProgram);

	// get the location of the vPosition attribute in the shader
	assert((vPosition = glGetAttribLocation(frameProgram, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// get the location of the vColor and mvp_matrix attributes in the shader
	assert((vColor = glGetUniformLocation(frameProgram, "vColor")) != -1);
	assert((vFrameMVPMatrix = glGetUniformLocation(frameProgram, "mvp_matrix")) != -1);
}

Cube::~Cube(){

}

//----------------------------------------------------------------------------
void Cube::makeQuad(int ind1, int ind2, int ind3, int ind4) {
	//triangle #1
	points[index] = vertices[ind1];
	colors[index] = potentialColors[index%8];
	index++;

	points[index] = vertices[ind2];
	colors[index] = colors[index - 1];
	index++;

	points[index] = vertices[ind3];
	colors[index] = colors[index - 1];
	index++;

	//Triangle #2
	points[index] = vertices[ind3];
	colors[index] = colors[index - 1];
	index++;

	points[index] = vertices[ind4];
	colors[index] = colors[index - 1];
	index++;

	points[index] = vertices[ind1];
	colors[index] = colors[index - 1];
	index++;
}

void Cube::makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4)
{
	indices[index] = ind1;
	index++;
	indices[index] = ind2;
	index++;
	indices[index] = ind3;
	index++;
	indices[index] = ind3;
	index++;
	indices[index] = ind4;
	index++;
	indices[index] = ind1;
	index++;
}

void Cube::buildCube() {
	index = 0;

	makeIndexedQuad(1, 0, 3, 2);  //front
	makeIndexedQuad(2, 3, 7, 6);  //right
	makeIndexedQuad(3, 0, 4, 7);  //bottom
	makeIndexedQuad(6, 5, 1, 2);  //top
	makeIndexedQuad(4, 5, 6, 7);  //back
	makeIndexedQuad(5, 4, 0, 1);  //left
}

void Cube::draw(Camera cam, vector<Light> lights)
{
	mat4 mvp_matrix = cam.getViewMatrix() * modelmatrix;

	// draw the cube body
	glBindVertexArray(VAO);
	glUseProgram(program);  // also switch to using this shader program
	glUniformMatrix4fv(vMVPMatrix, 1, GL_TRUE, mvp_matrix);
	glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));

	// draw the cube frame
	glBindVertexArray(frameVAO);
	glUseProgram(frameProgram);  // also switch to using this shader program
	glUniformMatrix4fv(vFrameMVPMatrix, 1, GL_TRUE, mvp_matrix);
	glUniform4fv(vColor, 1, vec4(0, 0, 0, 1));
	glLineWidth(5.0);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, BUFFER_OFFSET(sizeof(indices)));
}
