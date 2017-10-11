#version 410
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec4 Normal;
layout(location = 3) in vec2 TextureCoord;

out vec4 vertexPosition;
out vec4 vertexColor;
out vec4 vertexNormal;
out vec2 vertexTextureCoord;

uniform mat4 WVP;
uniform sampler2D perlinTexture;
uniform float scale;
void main()
{

vertexPosition = Position;
vertexColor = Color;
vertexNormal = Normal;
vertexTextureCoord = TextureCoord;

vec3 diffuseTexture = texture(perlinTexture, vertexTextureCoord).rrr;
vec4 point = Position;

point.y += diffuseTexture.y * scale;

gl_Position = WVP * point;
}