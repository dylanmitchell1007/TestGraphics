#pragma once




class Texture
{
public:
	Texture();
	~Texture();
	void bind();
	void load(const char* path);
	void generate2D(unsigned int imageWidth, unsigned int imageHeight, float* data);




private:
	unsigned int m_texture;
};


