#include <Application.h>
#include "CameraApp.h"

int main()
{
	Application* app = new CameraApp();
	app->run("camera", 1600, 900, false);
	return 0;
}
