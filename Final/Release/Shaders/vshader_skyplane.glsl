#version 150

in vec4 vPosition;
in vec2 vTexture; 

out vec2 texCoord;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

void main() 
{
	gl_Position = proj_matrix*view_matrix*model_matrix*vPosition;	// vertex position in world coordinates
	texCoord = vTexture;											// pass through texture location to fragment shader
}