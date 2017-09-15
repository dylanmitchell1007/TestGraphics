#include "gl_core_4_4.h"
#include "glfw3.h"
#include "App.h"


int main()
{
	
	//Starts the apps using the App Class and uses the run function to get the details about the window.
	App* app = new App();
	app->run("dat", 500, 500, false);
	return 0;
}