#version 410

in vec4 vColor;
in vec4 vPosition;

out vec4 FragColor;

void main() 
{
	FragColor = vColor;
	FragColor = vPosition;
 }