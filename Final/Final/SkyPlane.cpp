#include "SkyPlane.h"

SkyPlane::SkyPlane()
{
	// set shape defaults
	buildGeometry();
	translation = Translate(-10, 0, 0);
	scale = Scale(18, 18, 18);
	setModelMatrix(translation*scale);

	// set up buffers
	glGenVertexArrays(1, &VAO);		// get a vertex array object
	glGenBuffers(1, &VBO);			// get a vertex buffer for positions
	glGenBuffers(1, &VIO);			// get a vertex buffer for indices
	glBindVertexArray(VAO);				// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// bind the VBO to the current VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(textures), NULL, GL_STATIC_DRAW);	// allocate space on the VBO
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);				// put vertices on VBO
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(textures), textures);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);		// bind the VIO to the current VAO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	// put indices on the VIO

	//-----------------------------------------------------------------------
	// load shader program
	assert((program = InitShader("Shaders/vshader_skyplane.glsl", "Shaders/fshader_skyplane.glsl")) != -1);
	glUseProgram(program);

	// get the location of the vPosition attribute in the shader
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// get the location of the vTexture attribute in the shader
	assert((vTexture = glGetAttribLocation(program, "vTexture")) != -1);
	glEnableVertexAttribArray(vTexture);
	glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	// set the texturing parameters
	imgSize = 512;
	glGenTextures(1, &textureIDs);
	glBindTexture(GL_TEXTURE_2D, textureIDs);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// read in the texture source images
	image = ppmRead((char*)"Textures/skybox_side.ppm", &imgSize, &imgSize);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgSize, imgSize, 0, GL_RGB, GL_UNSIGNED_BYTE, image);  //move the data onto the GPU
	delete[] image;

	// get the locations of uniforms in the shader
	assert((vModelMatrix = glGetUniformLocation(program, "model_matrix")) != -1);
	assert((vViewMatrix = glGetUniformLocation(program, "view_matrix")) != -1);
	assert((vProjectionMatrix = glGetUniformLocation(program, "proj_matrix")) != -1);
}


SkyPlane::~SkyPlane() {
	glDeleteTextures(1, &textureIDs);
}

void SkyPlane::buildGeometry()
{
	makeIndexedQuad(0, 1, 2, 3);
}

void SkyPlane::makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4)
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

void SkyPlane::draw(Camera* cam, vector<Light> lights)
{
	if(!cam->get2D())
		return;
	
	translation = Translate(-10, 0, (cam->getPosition().z));
	modelmatrix = translation * RotateZ(90) * RotateY(-90) * scale;

	// bind VAO and shader program
	glBindVertexArray(VAO);
	glUseProgram(program);

	// matrices
	glUniformMatrix4fv(vModelMatrix, 1, GL_TRUE, modelmatrix);
	glUniformMatrix4fv(vViewMatrix, 1, GL_TRUE, cam->getViewMatrix());
	glUniformMatrix4fv(vProjectionMatrix, 1, GL_TRUE, cam->getProjectionMatrix());

	// textures
	glUniform1i(vTexture, 0);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureIDs);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
}
