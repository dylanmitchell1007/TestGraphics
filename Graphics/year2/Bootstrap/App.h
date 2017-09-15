#pragma once
#include "gl_core_4_4.h"
struct GLFWmonitor;
struct GLFWwindow;
class App
{
public:
	App();
	~App();
	virtual void startup();
	virtual void shutdown();
	virtual void update(float deltatime);
	virtual void draw();
	virtual void run(const char* title, unsigned int width, unsigned int height, bool fullscreen);
	
protected:
	GLFWwindow* _window;
	GLFWmonitor* _monitor;
	
};

