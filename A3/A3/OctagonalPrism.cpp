#include "OctagonalPrism.h"

OctagonalPrism::OctagonalPrism()
{
	// set shape defaults
	buildGeometry();
	translation = Translate(0, 0, -2.0f);
	setModelMatrix(translation);
	rotationAngle = 0;

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
	assert((program = InitShader("vshader00_v150.glsl", "fshader00_v150.glsl")) != -1);
	glUseProgram(program);

	// get the location of the vPosition attribute in the shader
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// get the location of the vColor attribute in the shader
	assert((vColor = glGetAttribLocation(program, "vColor")) != -1);
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

OctagonalPrism::~OctagonalPrism() {

}

void OctagonalPrism::buildGeometry()
{
	// define vertices
	for (int i = 0; i < numVertices / 2; i++)
	{
		GLfloat x = 0.2f * (float)cos(i * 45.0f * M_PI / 180);
		GLfloat y = 0.2f * (float)sin(i * 45.0f * M_PI / 180);
		vertices[i] = vec4(x, y, 0.25f, 1.0f);
		vertices[i + 8] = vec4(x, y, -0.25f, 1.0f);
	}
	// center points of front and back faces
	vertices[numVertices] = vec4(0, 0, 0.25f, 1.0f);
	vertices[numVertices + 1] = vec4(0, 0, -0.25f, 1.0f);

	// indices
	index = 0;
	// side faces
	makeIndexedQuad(0, 1, 9, 8);
	makeIndexedQuad(1, 2, 10, 9);
	makeIndexedQuad(2, 3, 11, 10);
	makeIndexedQuad(3, 4, 12, 11);
	makeIndexedQuad(4, 5, 13, 12);
	makeIndexedQuad(5, 6, 14, 13);
	makeIndexedQuad(6, 7, 15, 14);
	makeIndexedQuad(7, 0, 8, 15);

	// front face
	for (int i = 0; i < numVertices / 2; i++)
	{
		indices[index] = i + 1;
		index++;
		indices[index] = i;
		index++;
		indices[index] = 16;
		index++;
	}
	indices[index - 3] = 0;		// must reassign the wrap-around point

	// back face
	for (int i = numVertices / 2; i < numVertices; i++)
	{
		indices[index] = i;
		index++;
		indices[index] = i + 1;
		index++;
		indices[index] = 17;
		index++;
	}
	indices[index - 2] = 8;		// must reassign the wrap-around point
}

void OctagonalPrism::makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4)
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

void OctagonalPrism::draw(Camera cam, vector<Light> lights)
{
	mat4 mvp_matrix = cam.getProjectionMatrix() * cam.getViewMatrix() * modelmatrix;

	// draw the cube body
	glBindVertexArray(VAO);
	glUseProgram(program);  // also switch to using this shader program
	glUniformMatrix4fv(vMVPMatrix, 1, GL_TRUE, mvp_matrix);
	glDrawElements(GL_TRIANGLES, numIndices + numIndices, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));

	// draw the cube frame
	glBindVertexArray(frameVAO);
	glUseProgram(frameProgram);  // also switch to using this shader program
	glUniformMatrix4fv(vFrameMVPMatrix, 1, GL_TRUE, mvp_matrix);
	glUniform4fv(vColor, 1, vec4(0, 0, 0, 1));
	glLineWidth(5.0);
	glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_BYTE, BUFFER_OFFSET(sizeof(indices)));
}
