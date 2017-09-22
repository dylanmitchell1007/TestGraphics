// vertex shader 
#version 410 

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;
layout(location = 2) in vec4 Color;
out vec4 vNormal; 
out vec4 vPosition;
out vec4 vColor; 
uniform mat4 ProjectionViewWorld;
void main() 
 { 
	vColor = Color;
	vNormal = Normal;
	vPosition = Position; 
	gl_Position = ProjectionViewWorld * Position;
 } 
 // fragment shader 
