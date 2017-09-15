#pragma once
#include <App.h>

class Shader;
class Mesh;
class Texture;
class Camera;
class RenderingGeometryApp : public App
{
public:
	RenderingGeometryApp();
	~RenderingGeometryApp();

	// Inherited via Application
	virtual void startup() override;

	virtual void shutdown() override;

	virtual void update(float time) override;

	virtual void draw() override;

private:

	Mesh* object;
	Mesh* newobject;
	Mesh* loadOBJ;
	Shader* myshader;
	Shader* triShader;
	Camera* m_camera;
};