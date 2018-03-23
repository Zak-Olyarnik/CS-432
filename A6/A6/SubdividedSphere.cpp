#include "SubdividedSphere.h"

SubdividedSphere::SubdividedSphere()
{
	// set shape defaults
	tetrahedron(4);
	translation = Translate(0, 1, -4.0f);
	setModelMatrix(translation);

	// set up buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);				// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// associate the VBO with the active VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);

	//-----------------------------------------------------------------------
	// load shader program
	assert((program = InitShader("vshader00_v150.glsl", "fshader00_v150.glsl")) != -1);
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

SubdividedSphere::~SubdividedSphere() {

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
	normals[index] = normalize(vec3(vertices[index].x, vertices[index].y, vertices[index].z));
	index++;
	vertices[index] = b0;
	normals[index] = normalize(vec3(vertices[index].x, vertices[index].y, vertices[index].z));
	index++;
	vertices[index] = c0;
	normals[index] = normalize(vec3(vertices[index].x, vertices[index].y, vertices[index].z));
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

void SubdividedSphere::draw(Camera cam, vector<Light> lights)
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

	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}
