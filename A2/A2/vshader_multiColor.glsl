#version 150

in vec3 vPosition;
in vec4 vColor;
out vec4 color;
uniform mat3 model_matrix;
uniform float brightness;

void main() 
{
  gl_Position = vec4((model_matrix * vPosition).xy, 0, 1);
  color = brightness * vColor;
}