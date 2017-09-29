#pragma once




class Texture
{
public:
	Texture();
	~Texture();
	void bind();
	void load(const char* path);




private:
	unsigned int m_texture;
};


