#version 150

in vec4 vPosition;

out vec3 texCoord;

uniform mat4 modelview_matrix;
uniform mat4 proj_matrix;

void main() 
{
	gl_Position = proj_matrix*modelview_matrix*vPosition;
	texCoord = normalize(vPosition.xyz);
}