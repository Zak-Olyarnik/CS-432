#version 150

in vec4 vPosition;
in vec4 vColor;
flat out vec4 color;
uniform mat4 mvp_matrix;

void main() 
{ 
    gl_Position = mvp_matrix*vPosition;
    color = vColor;
}