#pragma once
#include "Shader.h"
#include "Mesh.h"
#include "Application.h"
#include <glm/glm.hpp>


typedef void(*KeyFunc)(GLFWwindow *window, int key, int scancode, int action, int mods);
typedef void(*ScrollFunc)(GLFWwindow* window, double xoffset, double yoffset);
class Transform;
class Camera;


class RenderingGeometryApp : public Application
{
public:
	RenderingGeometryApp();
	~RenderingGeometryApp();

	// Inherited via Application
	virtual void startup() override;
	virtual void shutdown() override;
	virtual void update(float) override;
	virtual void draw() override;

	static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
	static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
protected:
	void setKeyCallback(KeyFunc func);
	void setScrollCallback(ScrollFunc func);


private:
	
	bool perspective;
	bool m_angleAxis;

	float m_runningTime;

	Shader* myshader;
	Mesh* object;
	Camera* m_camera;
	Transform* m_earth;
	Transform* m_sun;
	float m_deltaTime;
	double m_mouseX;
	double m_mouseY;
	bool m_mouseButtonDown;
	bool m_mouseButtonUp;
	bool m_isDragging;
	float m_deltaX;
	float m_deltaY;
	double m_startX;
	double m_startY;
	double m_endX;
	double m_endY;
	float m_fov;
	float m_aspectRatio;
	float m_near;
	float m_far;
	float m_top;
	float m_bottom;
	float m_left;
	float m_right;

};

