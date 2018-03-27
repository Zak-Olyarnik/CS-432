#version 150

in vec2 texCoord;

out vec4 fColor;

uniform sampler2D textureID;  // the texture unit to sample from

void main() 
{ 
	fColor = texture(textureID, texCoord);
	fColor.a = 1.0;
} 

