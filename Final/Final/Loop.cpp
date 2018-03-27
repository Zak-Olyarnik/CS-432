#include "Loop.h"

Loop::Loop()
{
	// set shape defaults
	buildGeometry();
	translation = Translate(0, 8, -492.0f);
	scale = Scale(30, 16, 16);
	setModelMatrix(translation*scale);
	setMaterial(vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 0.8f, 0, 1), 20);

	// set up buffers
	glGenVertexArrays(1, &VAO);		// get a vertex array object
	glGenBuffers(1, &VBO);			// get a vertex buffer for positions
	glBindVertexArray(VAO);				// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// bind the VBO to the current VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals) + sizeof(textures), NULL, GL_STATIC_DRAW);	// allocate space on the VBO
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);				// put vertices on VBO
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);	// put normals on VBO
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), sizeof(textures), textures);
	
	//-----------------------------------------------------------------------
	// load shader program
	assert((program = InitShader("Shaders/vshader_ground.glsl", "Shaders/fshader_ground.glsl")) != -1);
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

	// set up textures
	imgSize = 512;
	glGenTextures(1, &textureIDs);
	glBindTexture(GL_TEXTURE_2D, textureIDs);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// read in the texture source image
	image = ppmRead((char*)"Textures/ground.ppm", &imgSize, &imgSize);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgSize, imgSize, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	delete[] image;

	// get the locations of uniforms in the shader
	assert((vModelMatrix = glGetUniformLocation(program, "model_matrix")) != -1);
	assert((vViewMatrix = glGetUniformLocation(program, "view_matrix")) != -1);
	assert((vProjectionMatrix = glGetUniformLocation(program, "proj_matrix")) != -1);
	assert((vDiffuse = glGetUniformLocation(program, "matDiffuse")) != -1);
	assert((vSpecular = glGetUniformLocation(program, "matSpecular")) != -1);
	assert((vAmbient = glGetUniformLocation(program, "matAmbient")) != -1);
	assert((vAlpha = glGetUniformLocation(program, "matAlpha")) != -1);
	assert((vSunLightDir = glGetUniformLocation(program, "sunLightDirection")) != -1);
	assert((vSunLightDiffuse = glGetUniformLocation(program, "sunLightDiffuse")) != -1);
	assert((vSunLightSpecular = glGetUniformLocation(program, "sunLightSpecular")) != -1);
	assert((vSunLightAmbient = glGetUniformLocation(program, "sunLightAmbient")) != -1);
}

Loop::~Loop(){
	glDeleteTextures(1, &textureIDs);
}

void Loop::buildGeometry()
{
	index = 0;

	for (int i = 0; i < numVertices; i=i+3)
	{
		vec3 a = vec3(vertices[i].x, vertices[i].y, vertices[i].z);
		vec3 b = vec3(vertices[i+1].x, vertices[i+1].y, vertices[i+1].z);
		vec3 c = vec3(vertices[i+2].x, vertices[i+2].y, vertices[i+2].z);

		vec3 N = normalize(cross(b - a, c - a));
		normals[i] = N;
		normals[i+1] = N;
		normals[i+2] = N;
	}
}

void Loop::draw(Camera* cam, vector<Light> lights)
{
	// bind VAO and shader program
	glBindVertexArray(VAO);
	glUseProgram(program);

	// matrices
	glUniformMatrix4fv(vModelMatrix, 1, GL_TRUE, modelmatrix);
	glUniformMatrix4fv(vViewMatrix, 1, GL_TRUE, cam->getViewMatrix());
	glUniformMatrix4fv(vProjectionMatrix, 1, GL_TRUE, cam->getProjectionMatrix());

	// materials
	glUniform4fv(vDiffuse, 1, diffuse);
	glUniform4fv(vSpecular, 1, specular);
	glUniform4fv(vAmbient, 1, ambient);
	glUniform1f(vAlpha, shine);

	// sun
	glUniform4fv(vSunLightDir, 1, (cam->getViewMatrix() * lights[0].getDirection()));
	glUniform4fv(vSunLightAmbient, 1, lights[0].getAmbient());
	glUniform4fv(vSunLightDiffuse, 1, lights[0].getDiffuse());
	glUniform4fv(vSunLightSpecular, 1, lights[0].getSpecular());

	// textures
	glUniform1i(vTexture, 0);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureIDs);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}
