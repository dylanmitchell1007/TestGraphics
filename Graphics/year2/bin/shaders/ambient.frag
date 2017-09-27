#version 410



in vec4 vertexNormal;
in vec3 vertexTextureCoord;

out vec4 fragColor;

uniform vec3 upVector;
uniform vec3 skyColor;
uniform vec3 groundColor;

void main()
{
float vertNormDotUp = dot(normalize(vertexNormal.xyz), vec3(0,1,0));
float influence = vertNormDotUp + (0.5f * 0.5f);

vec3 hemisphereLighting = mix(groundColor, skyColor, influence);
fragColor = vec4(hemisphereLighting, 1);
fragColor = vec4(normalize(vertexNormal.xyz), 1);
}





