#include "SubdividedSphere.h"

SubdividedSphere::SubdividedSphere()
{
	// set shape defaults
	tetrahedron(4);
	translation = Translate(0, 1, -4.0f);
	setModelMatrix(translation);
	setMaterial(vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 0.8f, 0, 1), 100);

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
	assert((program = InitShader("vshader_sphere.glsl", "fshader_sphere.glsl")) != -1);
	glUseProgram(program);

	// get the location of the vPosition attribute in the shader
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// get the location of the vNormal attribute in the shader
	assert((vNormal = glGetAttribLocation(program, "vNormal")) != -1);
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	// set up cube map textures
	imgSize = 1000;
	glGenTextures(1, &textureIDs);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIDs);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// read in the texture source images
	image = ppmRead((char*)"blue.ppm", &imgSize, &imgSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, imgSize, imgSize, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, imgSize, imgSize, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, imgSize, imgSize, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, imgSize, imgSize, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, imgSize, imgSize, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	image = ppmRead((char*)"sonic_face.ppm", &imgSize, &imgSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, imgSize, imgSize, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
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
	assert((vMap = glGetUniformLocation(program, "cubeMap")) != -1);
}

SubdividedSphere::~SubdividedSphere() {
	glDeleteTextures(1, &textureIDs);
}

void SubdividedSphere::tetrahedron(int count)
{
	// initial 4 vertices
	vec4 v[4] =
	{
		vec4(0, 0, 1, 1),
		vec4(0, 2 * sqrt2 / 3, -1.0f / 3.0f, 1),
		vec4(-sqrt6 / 3.0f, -sqrt2 / 3.0f, -1.0f / 3.0f, 1.0f),
		vec4(sqrt6 / 3.0f, -sqrt2 / 3.0f, -1.0f / 3.0f, 1.0f)
	};

	// subdivide each of the 4 faces
	divideTriangle(v[0], v[1], v[2], count);
	divideTriangle(v[3], v[2], v[1], count);
	divideTriangle(v[0], v[3], v[1], count);
	divideTriangle(v[0], v[2], v[3], count);
}

void SubdividedSphere::divideTriangle(const vec4& a, const vec4& b, const vec4& c, int count)
{
	if (count > 0)
	{
		vec4 v1 = unit(a + b);
		v1.w = 1.0;
		vec4 v2 = unit(a + c);
		v2.w = 1.0;
		vec4 v3 = unit(b + c);
		v3.w = 1.0;

		divideTriangle(a, v1, v2, count - 1);
		divideTriangle(c, v2, v3, count - 1);
		divideTriangle(b, v3, v1, count - 1);
		divideTriangle(v1, v3, v2, count - 1);
	}
	else
	{
		triangle(a, b, c);
	}
}

void SubdividedSphere::triangle(const vec4& a, const vec4& b, const vec4& c)
{
	vec3 a0 = vec3(a.x, a.y, a.z);
	vec3 b0 = vec3(b.x, b.y, b.z);
	vec3 c0 = vec3(c.x, c.y, c.z);

	vertices[index] = a0;
	normals[index] = normalize(a0);
	index++;
	vertices[index] = b0;
	normals[index] = normalize(b0);
	index++;
	vertices[index] = c0;
	normals[index] = normalize(c0);
	index++;
}

vec4 SubdividedSphere::unit(const vec4& p)
{
	float len = p.x*p.x + p.y*p.y + p.z*p.z;
	vec4 t;
	if (len > DivideByZeroTolerance)
	{
		t = p / sqrt(len);
		t.w = 1.0f;
	}
	return t;
}

bool SubdividedSphere::testCollision(vec4 direction, vec4 origin)
{
	GLfloat smallestZDistance = FLT_MAX;
	int firstIndex = -1;
	for (int i = 0; i < numVertices; i = i + 3)
	{
		vec4 e = modelmatrix * vertices[i];
		vec4 f = modelmatrix * vertices[i + 1];
		vec4 g = modelmatrix * vertices[i + 2];
		vec3 n = cross((f - e), (g - e));
		
		if (dot(n, direction) == 0)	// tested ray and plane containing this triangle are parallel, no collision
		{ }
		else
		{
			GLfloat t = -((dot(n, origin) + dot(-e, n)) / dot(n, direction));
			if (t < 0)	// tested ray points away from plane, no collision
			{ }
			else	// tested ray hit triangle's plane, now test if actually inside triangle
			{
				vec4 intersectionPoint = origin + t * direction;
				vec4 edge = f - e;
				vec4 vc = intersectionPoint - e;
				vec3 cr = cross(edge, vc);
				GLfloat result = dot(n, cr);
				if (result > 0)
				{
					edge = g - f;
					vc = intersectionPoint - f;
					cr = cross(edge, vc);
					result = dot(n, cr);
					if (result > 0)
					{
						edge = e - g;
						vc = intersectionPoint - g;
						cr = cross(edge, vc);
						result = dot(n, cr);
						if (result > 0)		// tested ray is inside bounds of triangle!
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void SubdividedSphere::roll(float deg, bool move)
{ 
	rotation += deg;
	if(move)
		translation = translation * Translate(0, 0, -0.2f);
}

void SubdividedSphere::jump(int dir)
{
	dir = dir / abs(dir);
	translation = translation * Translate(0, dir*0.1f, 0);
}

void SubdividedSphere::changeLane(float dir)
{
	dir = dir / abs(dir);
	translation = translation * Translate(dir*0.33f, 0, 0);
}

void SubdividedSphere::ramp()
{
	translation = translation * Translate(0, 0.025f, 0);
}

void SubdividedSphere::loop()
{
	vec3 loopPos(0, 8, -492.0f);
	float radius = 6.008f;
	float w = 1.633f; //(deg / 10 ms)
	loopTheta += w;
	float z = (float) (loopPos.z - radius * cos(loopTheta * M_PI/180));
	float y = (float) (loopPos.y + radius * sin(loopTheta * M_PI/180));
	translation = Translate(getPosition().x - 0.048f, y, z);
}

void SubdividedSphere::draw(Camera* cam, vector<Light> lights)
{
	modelmatrix = translation*RotateX((GLfloat)-rotation);
	cam->setTarget(modelmatrix*vec4(0, 0, 0, 1));

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
	glUniform1i(vMap, 0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIDs);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}
