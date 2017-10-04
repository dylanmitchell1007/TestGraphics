#version 410
 
in vec2 vertexTextureCoord;
out vec4 fragColour;

uniform sampler2D perlinTexture;

void main() 

{
 //fragColour = texture(perlinTexture, vTexCoord).rrrr;
 //fragColour.a = 1;
 
vec3 diffuseTexture = texture(perlinTexture, vertexTextureCoord).rrr;
 
fragColour = vec4(diffuseTexture, 1);

}

