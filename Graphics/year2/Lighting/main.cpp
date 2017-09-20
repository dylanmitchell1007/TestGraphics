#include "LightingApp.h"

int main()
{
	Application* app = new LightingApp();
	app->run("LightingApp", 800, 800, false);

	return 1;
}