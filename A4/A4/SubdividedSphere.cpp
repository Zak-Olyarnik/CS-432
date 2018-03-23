#include "SubdividedSphere.h"

SubdividedSphere::SubdividedSphere()
{
	// set shape defaults
	tetrahedron(4);
	translation = Translate(0, 1, -4.0f);
	setModelMatrix(translation);

	// get buffers for the attributes and indices
	glGenBuffers(1, &VBO);

	// put the vertex and color data on the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

	// get a vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);				// make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// associate the VBO with the active VAO

	// load shader program
	assert((program = InitShader("vshader_multiColor.glsl", "fshader.glsl")) != -1);
	glUseProgram(program);

	// get the location of the vPosition attribute in the shader
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// get the location of the vColor attribute in the shader
	assert((vColor = glGetAttribLocation(program, "vColor")) != -1);
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	// get the location of the model-view-projection matrix in the shader
	assert((vMVPMatrix = glGetUniformLocation(program, "mvp_matrix")) != -1);
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
	vec4 color((GLfloat)1.0*rand() / RAND_MAX, (GLfloat)1.0*rand() / RAND_MAX, (GLfloat)1.0*rand() / RAND_MAX, 1.0f);
	vertices[index] = a;
	colors[index] = color;
	index++;
	vertices[index] = b;
	colors[index] = color;
	index++;
	vertices[index] = c;
	colors[index] = color;
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

int SubdividedSphere::testCollision(vec4 direction, vec4 origin)
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
		{
			//fprintf(stdout, "parallel\n");
		}
		else
		{
			GLfloat t = -((dot(n, origin) + dot(-e, n)) / dot(n, direction));
			if (t < 0)	// tested ray points away from plane, no collision
			{
				//fprintf(stdout, "ray points away\n");
			}
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
							//fprintf(stdout, "hit\n");
							if (t < smallestZDistance)	// this triangle is closer to camera than any previously hit triangle
							{
								smallestZDistance = t;	// save distance to triangle
								firstIndex = i;			// save index of first vertex of this triangle to find it later
							}
						}
					}
				}
			}
		}
	}
	return firstIndex;
}

void SubdividedSphere::recolorTriangle(int firstIndex)
{
	vec4 black(0, 0, 0, 1.0f);
	vec4 triColors[3] = { black, black, black };
	
	// use index of first vertex' position to replace colors of all three
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + (firstIndex * sizeof(vec4)), sizeof(triColors), triColors);
}

void SubdividedSphere::draw(Camera cam, vector<Light> lights)
{
	mat4 mvp_matrix = cam.getProjectionMatrix() * cam.getViewMatrix() * modelmatrix;

	// draw the sphere
	glBindVertexArray(VAO);
	glUseProgram(program);  // also switch to using this shader program
	glUniformMatrix4fv(vMVPMatrix, 1, GL_TRUE, mvp_matrix);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}
