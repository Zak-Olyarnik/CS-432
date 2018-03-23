#include "SkyboxCube.h"

SkyboxCube::SkyboxCube()
{
	// set shape defaults
	buildGeometry();
	scale = Scale(1, 1, 1);
	translation = Translate(0, 0, 0);
	setModelMatrix(translation*scale);

	// set up buffers
	glGenVertexArrays(1, &VAO);		// get a vertex array object
	glGenBuffers(1, &VBO);			// get a vertex buffer for positions
	glBindVertexArray(VAO);				// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// bind the VBO to the current VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW);	// allocate space on the VBO
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);				// put vertices on VBO
	
	//-----------------------------------------------------------------------
	// load shader program
	assert((program = InitShader("vshader_skybox.glsl", "fshader_skybox.glsl")) != -1);
	glUseProgram(program);

	// get the location of the vPosition attribute in the shader
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// set the texturing parameters
	glGenTextures(1, textureIDs);
	int imgsize = 512;
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIDs[0]);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// read in the texture source images
	skyTop = ppmRead((char*)"skybox_Top.ppm", &imgsize, &imgsize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyTop);
	delete[] skyTop;
	skyBottom = ppmRead((char*)"skybox_Bottom.ppm", &imgsize, &imgsize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyBottom);
	delete[] skyBottom;
	skyRight = ppmRead((char*)"skybox_Right.ppm", &imgsize, &imgsize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyRight);
	delete[] skyRight;
	skyLeft = ppmRead((char*)"skybox_Left.ppm", &imgsize, &imgsize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyLeft);
	delete[] skyLeft;
	skyFront = ppmRead((char*)"skybox_Front.ppm", &imgsize, &imgsize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyFront);
	delete[] skyFront;
	skyBack = ppmRead((char*)"skybox_Back.ppm", &imgsize, &imgsize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyBack);
	delete[] skyBack;

	// get the locations of uniforms in the shader
	assert((vModelViewMatrix = glGetUniformLocation(program, "modelview_matrix")) != -1);
	assert((vProjectionMatrix = glGetUniformLocation(program, "proj_matrix")) != -1);
	assert((vMap = glGetUniformLocation(program, "cubeMap")) != -1);
}

SkyboxCube::~SkyboxCube(){
	glDeleteTextures(2, textureIDs);
}

void SkyboxCube::buildGeometry()
{
	index = 0;

	makeQuad(0, 1, 5, 4);
	makeQuad(1, 2, 6, 5);
	makeQuad(2, 3, 7, 6);
	makeQuad(3, 0, 4, 7);
	makeQuad(0, 3, 2, 1);
	makeQuad(5, 6, 7, 4);
}


void SkyboxCube::makeQuad(int ind1, int ind2, int ind3, int ind4)
{
	vertices[index] = uniqueVertices[ind1];
	index++;
	vertices[index] = uniqueVertices[ind2];
	index++;
	vertices[index] = uniqueVertices[ind3];
	index++;
	vertices[index] = uniqueVertices[ind3];
	index++;
	vertices[index] = uniqueVertices[ind4];
	index++;
	vertices[index] = uniqueVertices[ind1];
	index++;
}

void SkyboxCube::draw(Camera cam, vector<Light> lights)
{
	// bind VAO and shader program
	glBindVertexArray(VAO);
	glUseProgram(program);

	// matrices
	vec4 skyboxEye = vec4(0, 0, 0, 1);
	mat4 model_view = LookAt(skyboxEye, skyboxEye + cam.getForward(), cam.getUp());
	glUniformMatrix4fv(vModelViewMatrix, 1, GL_TRUE, model_view);
	glUniformMatrix4fv(vProjectionMatrix, 1, GL_TRUE, cam.getProjectionMatrix());

	// textures
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIDs[0]);
	glUniform1i(vMap, 0);

	// special skybox settings
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}
