#include "TextureApp.h"

int main()
{
	Application* app = new TextureApp();
	app->run("TextureApp", 800, 800, false);

	return 1;
}