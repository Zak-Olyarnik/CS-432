#include "Plane.h"

Plane::Plane()
{
	// set shape defaults
	buildGeometry();
	translation = Translate(0, 0, 0);
	setModelMatrix(translation);

	// set up buffers
	glGenVertexArrays(1, &VAO);		// get a vertex array object
	glGenBuffers(1, &VBO);			// get a vertex buffer for positions
	glGenBuffers(1, &VIO);			// get a vertex buffer for indices
	glBindVertexArray(VAO);				// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// bind the VBO to the current VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals) + sizeof(textures), NULL, GL_STATIC_DRAW);	// allocate space on the VBO
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);				// put vertices on VBO
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);	// put normals on VBO
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), sizeof(textures), textures);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);		// bind the VIO to the current VAO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	// put indices on the VIO

	//-----------------------------------------------------------------------
	// load shader program
	assert((program = InitShader("vshaderTM_v150.glsl", "fshaderTM_v150.glsl")) != -1);
	glUseProgram(program);

	// get the location of the vPosition attribute in the shader
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// get the location of the vNormal attribute in the shader
	assert((vNormal = glGetAttribLocation(program, "vNormal")) != -1);
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	// get the location of the vTexture attribute in the shader
	assert((vTexture = glGetAttribLocation(program, "vTexture")) != -1);
	glEnableVertexAttribArray(vTexture);
	glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices) + sizeof(normals)));

	// get the texture data for the plane
	glGenTextures(1, &textureIDs[0]);
	int imgsize = 512;
	GLubyte *data = ppmRead((char*)"grass_texture.ppm", &imgsize, &imgsize);
	glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //move the data onto the GPU
	delete[] data;  // don't need this data now that its on the GPU

	// set the texturing parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
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

Plane::~Plane() {
	glDeleteTextures(1, &textureIDs[0]);
}

void Plane::buildGeometry()
{
	// define vertices
	index = 0;
	float texX = 0;
	for (int i = -10; i <= 10; i += 2)
	{
		float texY = 0;
		for (int j = -10; j <= 10; j += 2)
		{
			vertices[index] = vec4((float)i, 0, (float)j, 1.0f);
			textures[index] = vec2(texX, texY);
			index++;
			texY += 0.5f;
		}
		texX += 0.5f;
	}

	// indices
	index = 0;
	for (int i = 0; i <= 108; i++)
	{
		if (i == 10 || i == 21 || i == 32 || i == 43 || i == 54 || i == 65 || i == 76 || i == 87 || i == 98)
			continue;
		makeIndexedQuad(i + 1, i, i + 11, i + 12);
	}

	// normals
	for (int i = 0; i < numVertices; i++)
	{
		normals[i] = vec3(0, 1, 0);
	}
}

void Plane::makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4)
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

void Plane::draw(Camera cam, vector<Light> lights)
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

	// textures
	glUniform1i(vTexture, 0);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureIDs[0]);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
}
