#include "Triangle.h"

Triangle::Triangle()
{
	vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);
	set(blue, blue, blue);
}

Triangle::Triangle(vec4 color1, vec4 color2, vec4 color3)
{
	set(color1, color2, color3);
}

void Triangle::set(vec4 color1, vec4 color2, vec4 color3)
{
	// get a vertex buffer and a vertex array object
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);				// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// associate the VBO with the active VAO

	// define vertices (homogenous coordinates)
		// this creates an equilateral triangle with side length 0.4
	vec3 points[NumVertices] =
	{
		vec3(-0.4f / 2, -0.4f*sqrtf(3) / 6, 1.0f),
		vec3(0.4f / 2, -0.4f*sqrtf(3) / 6, 1.0f),
		vec3(0, 0.4f*sqrtf(3)/3.0f, 1.0f)
	};
	
	// define per-vertex colors
	vec4 colors[NumVertices] =
	{
		color1, color2, color3
	};

	// set shape defaults
	setModelMatrix(mat3(1.0));
	brightness = 1.0f;
	rotationAngle = 0;

	// put the data on the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
	
	// load shader program
	assert((program = InitShader("vshader_multiColor.glsl", "fshader_multiColor.glsl"))!=-1);
	glUseProgram(program);  // make it the active one

	// get the location of the vPosition attribute
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);  // enable it
	// associate the area in the active VBO with this attribute and tell it how data to pull out for each vertex from the VBO
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// get the location of the vColor attribute in the shader
	assert((vColor = glGetAttribLocation(program, "vColor")) != -1);
	glEnableVertexAttribArray(vColor);  // enable it
	// associate the area in the active VBO with this attribute and tell it how data to pull out for each vertex from the VBO
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	// get the location of the model matrix in the shader
	assert((vModelMatrix = glGetUniformLocation(program, "model_matrix")) != -1);
	assert((fBrightness = glGetUniformLocation(program, "brightness")) != -1);
}

Triangle::~Triangle()
{
	// put additional things to destroy in here.  The Drawable class handles the VBO/VAO
}

void Triangle::draw(Camera cam, vector<Light> lights)
{
	// make this object's VAO active
		// this in turn makes its associated VBO active and
		// links the attributes to the shader
	glBindVertexArray(VAO); 
	glUseProgram(program);  // also switch to using this shader program

	// unfortunately, every time we draw we still need to reset all uniform variables
	modelmatrix = translation * rotation;
	glUniformMatrix3fv(vModelMatrix, 1, GL_TRUE, modelmatrix);
	glUniform1f(fBrightness, brightness);

	// now that we're all set up, tell the GPU to render NumVertice vertexes as a triangle fan
		// starting at offset 0 in the VBO
	glDrawArrays(GL_TRIANGLE_FAN, 0, NumVertices);
}
