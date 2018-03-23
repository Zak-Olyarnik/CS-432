#version 150

in vec4 vPosition;
flat out vec4 color;
uniform vec4 vColor;
uniform mat4 mvp_matrix;

void main() 
{ 
    gl_Position = mvp_matrix*vPosition;
    color = vColor;
}