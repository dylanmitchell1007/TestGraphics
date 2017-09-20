#pragma once
#include <Application.h>

struct Mesh;
struct Shader;
struct Camera;
class LightingApp : public Application
{
public:
	// Inherited via Application
	virtual void startup();

	virtual void shutdown();

	virtual void update(float);

	virtual void draw();


private:
	Camera* m_camera;
	Mesh* m_sphere;
	Shader* m_shader;
};