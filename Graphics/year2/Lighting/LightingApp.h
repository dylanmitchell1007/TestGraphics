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

	virtual void Diffuse();

	virtual void Ambient();

	virtual void Specular();

	virtual void Phong();

	virtual void BlinPhong();

	virtual void ColorSphere();

	virtual void shutdown();

	virtual void update(float);

	virtual void draw();


private:
	Camera* m_camera;
	
	Mesh* plane_Mesh;
	
	Mesh* m_sphere;
	
	Shader* myshader;
	
	Shader* m_ambient;

	Shader* m_diffuse;
	
	Shader* m_specular;
	
	Shader* m_phong;

	Shader* m_blinphong;

	Shader* m_shader;
};