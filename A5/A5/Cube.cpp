#include "Cube.h"

Cube::Cube()
{
	// set shape defaults
	buildGeometry();
	scale = Scale(1.5, 1.5, 1.5);
	translation = Translate(3, 1, -6);
	setModelMatrix(translation*scale);

	// set up buffers
	glGenVertexArrays(1, &VAO);		// get a vertex array object
	glGenBuffers(1, &VBO);			// get a vertex buffer for positions
	glGenBuffers(1, &VIO);			// get a vertex buffer for indices
	glBindVertexArray(VAO);				// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// bind the VBO to the current VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), NULL, GL_STATIC_DRAW);	// allocate space on the VBO
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);				// put vertices on VBO
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);	// put normals on VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);		// bind the VIO to the current VAO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	// put indices on the VIO

	//-----------------------------------------------------------------------
	// load shader program
	assert((program = InitShader("vshader01_v150.glsl", "fshader01_v150.glsl")) != -1);
	glUseProgram(program);

	// get the location of the vPosition attribute in the shader
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// get the location of the vNormal attribute in the shader
	assert((vNormal = glGetAttribLocation(program, "vNormal")) != -1);
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	// get the locations of uniforms in the shader
	assert((vModelMatrix = glGetUniformLocation(program, "model_matrix")) != -1);
	assert((vViewMatrix = glGetUniformLocation(program, "view_matrix")) != -1);
	assert((vProjectionMatrix = glGetUniformLocation(program, "proj_matrix")) != -1);
	assert((vDiffuse = glGetUniformLocation(program, "matDiffuse")) != -1);
	assert((vSpecular = glGetUniformLocation(program, "matSpecular")) != -1);
	assert((vAmbient = glGetUniformLocation(program, "matAmbient")) != -1);
	assert((vAlpha = glGetUniformLocation(program, "matAlpha")) != -1);
	assert((vFlashLightDiffuse = glGetUniformLocation(program, "flashLightDiffuse")) != -1);
	assert((vFlashLightSpecular = glGetUniformLocation(program, "flashLightSpecular")) != -1);
	assert((vFlashLightAmbient = glGetUniformLocation(program, "flashLightAmbient")) != -1);
	assert((vFlashLightEnabled = glGetUniformLocation(program, "flashLightEnabled")) != -1);
	assert((vFlashLightAngle = glGetUniformLocation(program, "flashLightAngle")) != -1);
	assert((vSunLightDir = glGetUniformLocation(program, "sunLightDirection")) != -1);
	assert((vSunLightDiffuse = glGetUniformLocation(program, "sunLightDiffuse")) != -1);
	assert((vSunLightSpecular = glGetUniformLocation(program, "sunLightSpecular")) != -1);
	assert((vSunLightAmbient = glGetUniformLocation(program, "sunLightAmbient")) != -1);
	assert((vSunLightEnabled = glGetUniformLocation(program, "sunLightEnabled")) != -1);
}

Cube::~Cube(){

}

void Cube::buildGeometry()
{
	index = 0;

	// side faces
	for (int i = 0; i <= 63; i++)
	{
		if (i == 15 || i == 31 || i == 47 || i == 63)
		{
			makeIndexedQuad(i, i-15, i + 1, i + 16);
		}
		else
		{
			makeIndexedQuad(i, i + 1, i + 17, i + 16);
		}
	}

	// bottom face
	makeIndexedQuad(1, 0, 15, 80);
	makeIndexedQuad(2, 1, 80, 81);
	makeIndexedQuad(3, 2, 81, 82);
	makeIndexedQuad(4, 3, 82, 5);
	makeIndexedQuad(80, 15, 14, 83);
	makeIndexedQuad(81, 80, 83, 84);
	makeIndexedQuad(82, 81, 84, 85);
	makeIndexedQuad(5, 82, 85, 6);
	makeIndexedQuad(83, 14, 13, 86);
	makeIndexedQuad(84, 83, 86, 87);
	makeIndexedQuad(85, 84, 87, 88);
	makeIndexedQuad(6, 85, 88, 7);
	makeIndexedQuad(86, 13, 12, 11);
	makeIndexedQuad(87, 86, 11, 10);
	makeIndexedQuad(88, 87, 10, 9);
	makeIndexedQuad(7, 88, 9, 8);
	
	// top face
	makeIndexedQuad(89, 79, 64, 65);
	makeIndexedQuad(90, 89, 65, 66);
	makeIndexedQuad(91, 90, 66, 67);
	makeIndexedQuad(69, 91, 67, 68);
	makeIndexedQuad(92, 78, 79, 89);
	makeIndexedQuad(93, 92, 89, 90);
	makeIndexedQuad(94, 93, 90, 91);
	makeIndexedQuad(70, 94, 91, 69);
	makeIndexedQuad(95, 77, 78, 92);
	makeIndexedQuad(96, 95, 92, 93);
	makeIndexedQuad(97, 96, 93, 94);
	makeIndexedQuad(71, 97, 94, 70);
	makeIndexedQuad(75, 76, 77, 95);
	makeIndexedQuad(74, 75, 95, 96);
	makeIndexedQuad(73, 74, 96, 97);
	makeIndexedQuad(72, 73, 97, 71);
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

void Cube::draw(Camera cam, vector<Light> lights)
{
	// bind VAO and shader program
	glBindVertexArray(VAO);
	glUseProgram(program);

	// matrices
	glUniformMatrix4fv(vModelMatrix, 1, GL_TRUE, modelmatrix);
	glUniformMatrix4fv(vViewMatrix, 1, GL_TRUE, cam.getViewMatrix());
	glUniformMatrix4fv(vProjectionMatrix, 1, GL_TRUE, cam.getProjectionMatrix());

	// materials
	glUniform4fv(vDiffuse, 1, diffuse);
	glUniform4fv(vSpecular, 1, specular);
	glUniform4fv(vAmbient, 1, ambient);
	glUniform1f(vAlpha, shine);

	// flashlight
	glUniform4fv(vFlashLightAmbient, 1, lights[0].getAmbient());
	glUniform4fv(vFlashLightDiffuse, 1, lights[0].getDiffuse());
	glUniform4fv(vFlashLightSpecular, 1, lights[0].getSpecular());
	glUniform1i(vFlashLightEnabled, lights[0].getEnabled());
	glUniform1f(vFlashLightAngle, lights[0].getFlashLightAngle());

	// sun
	glUniform4fv(vSunLightDir, 1, lights[1].getDirection());
	glUniform4fv(vSunLightAmbient, 1, lights[1].getAmbient());
	glUniform4fv(vSunLightDiffuse, 1, lights[1].getDiffuse());
	glUniform4fv(vSunLightSpecular, 1, lights[1].getSpecular());
	glUniform1i(vSunLightEnabled, lights[1].getEnabled());

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
}
