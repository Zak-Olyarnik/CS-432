#include "Plane.h"

Plane::Plane()
{
	// set shape defaults
	buildGeometry();
	translation = Translate(0, 0, 0);
	setModelMatrix(translation);

	// get buffers for the attributes and indices
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VIO);

	// put the vertex data on the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// put the index data on the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// get a vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);						// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);			// associate the VBO with the active VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);	// associate the VIO with the active VBO

	// load shader program
	assert((program = InitShader("vshader_solidColor.glsl", "fshader.glsl")) != -1);
	glUseProgram(program);

	// get the location of the vPosition attribute in the shader
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// get the locations of the uniforms in the shader
	assert((vMVPMatrix = glGetUniformLocation(program, "mvp_matrix")) != -1);
	assert((vColor = glGetUniformLocation(program, "vColor")) != -1);
}

Plane::~Plane() {

}

void Plane::buildGeometry()
{
	// define vertices
	vertices[0] = vec4(-10.0f, 0, -10.0f, 1.0f);
	vertices[1] = vec4(-10.0f, 0, 10.0f, 1.0f);
	vertices[2] = vec4(10.0f, 0, 10.0f, 1.0f);
	vertices[3] = vec4(10.0f, 0, -10.0f, 1.0f);

	// indices
	index = 0;
	makeIndexedQuad(0, 1, 2, 3);
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

int Plane::testCollision(vec4 direction, vec4 origin)
{
	for (int i = 0; i < numIndices; i = i + 3)
	{
		vec4 e = modelmatrix * vertices[indices[i]];
		vec4 f = modelmatrix * vertices[indices[i + 1]];
		vec4 g = modelmatrix * vertices[indices[i + 2]];
		vec3 n = cross((f - e), (g - e));

		if (dot(n, direction) == 0)
		{
			//fprintf(stdout, "parallel\n");
		}
		else
		{
			GLfloat t = -((dot(n, origin) + dot(-e, n)) / dot(n, direction));
			if (t < 0)
			{
				//fprintf(stdout, "ray points away\n");
			}
			else	// intersected triangle's plane, now test if actually inside triangle
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
						if (result > 0)
						{
							//fprintf(stdout, "hit\n");
						}
					}
				}
			}
		}
	}
	return -1;	// this function is not particularly useful since Plane is defined with indexing...
}

void Plane::draw(Camera cam, vector<Light> lights)
{
	mat4 mvp_matrix = cam.getProjectionMatrix() * cam.getViewMatrix() * modelmatrix;

	glBindVertexArray(VAO);
	glUseProgram(program);
	glUniformMatrix4fv(vMVPMatrix, 1, GL_TRUE, mvp_matrix);
	glUniform4fv(vColor, 1, vec4(0, 1, 0, 1));
	glDrawElements(GL_TRIANGLES, numIndices + numIndices, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
}
