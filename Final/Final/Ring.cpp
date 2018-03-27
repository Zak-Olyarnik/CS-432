#include "Ring.h"
#include <time.h> 

Ring::Ring()
{ }

Ring::Ring(SubdividedSphere* s)
{
	// set shape defaults
	player = s;
	buildGeometry();
	translation = Translate(0, 4, -4.0f);
	setModelMatrix(translation);
	setPositions();
	setMaterial(vec4(1, 1, 0, 1), vec4(0.75f, 0.6f, 0.2f, 1), vec4(0.5f, 0.5f, 0.5f, 1), 50);

	// set up buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);				// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// associate the VBO with the active VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), NULL, GL_STATIC_DRAW);	// allocate space on the VBO
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);				// put vertices on VBO
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);	// put normals on VBO

	//-----------------------------------------------------------------------
	// load shader program
	assert((program = InitShader("Shaders/vshader_ring.glsl", "Shaders/fshader_ring.glsl")) != -1);
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
	assert((vSunLightDir = glGetUniformLocation(program, "sunLightDirection")) != -1);
	assert((vSunLightDiffuse = glGetUniformLocation(program, "sunLightDiffuse")) != -1);
	assert((vSunLightSpecular = glGetUniformLocation(program, "sunLightSpecular")) != -1);
	assert((vSunLightAmbient = glGetUniformLocation(program, "sunLightAmbient")) != -1);
}

Ring::~Ring()
{ }

void Ring::buildGeometry()
{
	index = 0;

	// torus formula from http://paulbourke.net/geometry/torus/
	for (int theta = 0; theta < 360; theta = theta + levelOfDetail)
	{
		for (int phi = 0; phi < 360; phi = phi + levelOfDetail)
		{
			float x = (float) (cos(theta * M_PI / 180) * (r0 + r1 * cos(phi * M_PI / 180)));
			float y = (float) (sin(theta * M_PI / 180) * (r0 + r1 * cos(phi * M_PI / 180)));
			float z = (float) (r1 * sin(phi * M_PI / 180));
			vec4 p1 = vec4(x, y, z, 1);

			x = (float) (cos((theta + levelOfDetail) * M_PI / 180) * (r0 + r1 * cos(phi * M_PI / 180)));
			y = (float) (sin((theta + levelOfDetail) * M_PI / 180) * (r0 + r1 * cos(phi * M_PI / 180)));
			z = (float) (r1 * sin(phi * M_PI / 180));
			vec4 p2 = vec4(x, y, z, 1);

			x = (float) (cos((theta + levelOfDetail) * M_PI / 180) * (r0 + r1 * cos((phi + levelOfDetail) * M_PI / 180)));
			y = (float) (sin((theta + levelOfDetail) * M_PI / 180) * (r0 + r1 * cos((phi + levelOfDetail) * M_PI / 180)));
			z = (float) (r1 * sin((phi + levelOfDetail) * M_PI / 180));
			vec4 p3 = vec4(x, y, z, 1);

			x = (float) (cos(theta * M_PI / 180) * (r0 + r1 * cos((phi + levelOfDetail) * M_PI / 180)));
			y = (float) (sin(theta * M_PI / 180) * (r0 + r1 * cos((phi + levelOfDetail) * M_PI / 180)));
			z = (float) (r1 * sin((phi + levelOfDetail) * M_PI / 180));
			vec4 p4 = vec4(x, y, z, 1);
			
			makeQuad(p1, p2, p3, p4);
		}
	}
}

void Ring::makeQuad(vec4 p1, vec4 p2, vec4 p3, vec4 p4)
{
	// convert to vec3 (just to be safe)
	vec3 a = vec3(p1.x, p1.y, p1.z);
	vec3 b = vec3(p2.x, p2.y, p2.z);
	vec3 c = vec3(p3.x, p3.y, p3.z);
	vec3 d = vec3(p4.x, p4.y, p4.z);

	vec3 N = normalize(cross(b - a, c - a));
	vertices[index] = p1;
	normals[index] = N;
	index++;
	vertices[index] = p2;
	normals[index] = N;
	index++;
	vertices[index] = p3;
	normals[index] = N;
	index++;

	N = normalize(cross(d - c, a - c));
	vertices[index] = p3;
	normals[index] = N;
	index++;
	vertices[index] = p4;
	normals[index] = N;
	index++;
	vertices[index] = p1;
	normals[index] = N;
	index++;
}

void Ring::setPositions()
{
	srand(time(NULL));
	float x, y, z = 22;	// these "magic numbers" are dependent on the size of the course, for even spacing
	for (int i = 0; i < numInstances; i=i+3)
	{
		int lane = rand() % 6;	// 6 possible positions: on the ground or in the air in 3 lanes
		switch (lane)
		{
			case 0:
				x = -5.28f;
				y = 1;
				break;
			case 1:
				x = -5.28f;
				y = 3.5f;
				break;
			case 2:
				x = 0;
				y = 1;
				break;
			case 3:
				x = 0;
				y = 3.5f;
				break;
			case 4:
				x = 5.28f;
				y = 1;
				break;
			case 5:
				x = 5.28f;
				y = 3.5f;
				break;
		}
		mat4 position = Translate(x, y, -z);
		positions[i] = position;
		position = Translate(x, y, -(z + 3));
		positions[i+1] = position;
		position = Translate(x, y, -(z + 6));
		positions[i+2] = position;
		z = z + 50;
	}
}

void Ring::spin(GLfloat deg)
{
	rotation += deg;
}

bool Ring::testCollison(int instance)
{
	vec4 ringPos = positions[instance] * vec4(0, 0, 0, 1);
	vec4 playerPos = player->getPosition();
	
	// bounding sphere implementation: check if the distance between midpoints is less than
		// the sum of the radii
	auto distance = sqrt((playerPos.x - ringPos.x) * (playerPos.x - ringPos.x) +
						 (playerPos.y - ringPos.y) * (playerPos.y - ringPos.y) +
						 (playerPos.z - ringPos.z) * (playerPos.z - ringPos.z));
	
	return distance < (1 + outerRadius);
}

void Ring::reset()
{
	setPositions();
	for (int i = 0; i < numInstances; i++)
	{
		collected[i] = false;
	}
}

void Ring::draw(Camera* cam, vector<Light> lights)
{
	// bind VAO and shader program
	glBindVertexArray(VAO);
	glUseProgram(program);

	// matrices
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


	// rings are pseudo-instanced.  Each shares a VBO and all vertex/texture data, but unique
		// positions and "collected" attributes.  True instancing could not be used because of
		// the need to not render once a ring has been collected
	for (int i = 0; i < numInstances; i++)
	{
		if (collected[i])
			continue;	// do not render (don't even bother checking for collision) if ring has already been collected
		if (testCollison(i))
		{
			collected[i] = true;
			continue;
		}
		modelmatrix = positions[i] * RotateY((GLfloat)rotation);
		glUniformMatrix4fv(vModelMatrix, 1, GL_TRUE, modelmatrix);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}
