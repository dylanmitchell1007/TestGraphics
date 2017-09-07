
#include "RenderingGeometryApp.h"

int main()
{
	Application * app = new RenderingGeometryApp();
	app->run("rendering geometry", 1200, 800, false);
	delete app;
}