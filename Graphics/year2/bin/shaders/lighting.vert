#version 410

layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 Color;

out vec4 vPosition;
out vec4 vColor;

uniform mat4 ProjectionViewWorld;

void main()
{
	vColor = Color;
	vPosition = Position;
	gl_Position = ProjectionViewWorld * Position;
}