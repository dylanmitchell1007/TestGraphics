#pragma once
#include <Application.h>

//forward declare to reduce dependencies
class Shader;
class Mesh;
class Texture;
class Camera;
class RenderingGeometryApp : public Application
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

	Mesh* cube_Mesh; //plane
	Mesh* plane_Mesh; //square or cube
	Mesh* loadOBJ;
	Shader* myshader;
	Shader* triShader;
	Camera* m_camera;
};