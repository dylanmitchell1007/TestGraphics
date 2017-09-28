#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>
#include <gl_core_4_4.h>
#include "Texture.h"


Texture::Texture() {};
Texture::~Texture() {};
void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::load()
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data = stbi_load("..//bin//textures//370z.jpg", &imageWidth, &imageHeight, &imageFormat, STBI_default);
	glGenTextures(1, &m_texture); 
	glBindTexture(GL_TEXTURE_2D, m_texture); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}
