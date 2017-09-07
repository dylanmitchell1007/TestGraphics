#pragma once
#include "Application.h"
class Camera;
class Transform;
#define BLUE	glm::vec4(0, 0, 1, 1)
#define YELLOW	glm::vec4(1, 1, 0, 1)
#define WHITE	glm::vec4(1, 1, 1, 1)
#define BLACK	glm::vec4(0, 0, 0, 1)

typedef void(*KeyFunc)(GLFWwindow *window, int key, int scancode, int action, int mods);
typedef void(*ScrollFunc)(GLFWwindow* window, double xoffset, double yoffset);

class CameraApp : public Application
{

public:

	CameraApp();
	virtual ~CameraApp();
	static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
	static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
protected:
	
	void startup() override;
	void shutdown() override;
	void update(float) override;	
	void draw() override;
	void setKeyCallback(KeyFunc func);
	void setScrollCallback(ScrollFunc func);
	

private:
	bool perspective;
	bool m_angleAxis;

	float m_runningTime;

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
