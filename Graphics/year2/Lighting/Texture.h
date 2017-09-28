#pragma once




class Texture
{
public:
	Texture();
	~Texture();
	void bind();
	void load();




private:
	unsigned int m_texture;
};


