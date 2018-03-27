#version 150

in vec3 texCoord;

out vec4 fColor;

uniform samplerCube cubeMap;


void main() 
{ 
	fColor = texture(cubeMap, texCoord);
	fColor.a = 1.0;
} 

