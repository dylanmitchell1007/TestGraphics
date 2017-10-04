#pragma once
#include <Application.h>
struct Mesh;
struct Shader;
struct Camera;
struct Texture;
class TextureApp : public Application
{
public:
	TextureApp();
	~TextureApp();
	// Inherited via Application
	virtual void startup();

	virtual void update(float);

	virtual void shutdown();

	virtual void draw();
private:
	Mesh* plane_Mesh;
	Shader* m_textureShader;
	Texture* m_texture;
	Shader* m_shader;
	Shader* m_perlinShader;
};