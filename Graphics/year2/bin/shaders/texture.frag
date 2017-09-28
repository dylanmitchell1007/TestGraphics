#version 410

in vec4 vertexColor;
in vec2 vertexTextureCoord;

out vec4 fragColor;

uniform sampler2D diffuseTexture;

void main() 
{
fragColor = texture(diffuseTexture,vertexTextureCoord);
}





