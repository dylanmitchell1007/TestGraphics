#include "RenderingGeometryApp.h"


int main()
{
	App * app = new RenderingGeometryApp();
	app->run("rendering geometry", 1200, 800, false);
	delete app;
}