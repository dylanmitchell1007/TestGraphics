#version 410
 
in vec2 vertexTextureCoord;
out vec4 fragColour;

uniform sampler2D perlinTexture;

void main() 

{ 
vec3 diffuseTexture = texture(perlinTexture, vertexTextureCoord).xyz;
fragColour = vec4(diffuseTexture, 1);

}

