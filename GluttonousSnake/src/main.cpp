#include "pch.h"
#include "Application.h"

int main()
{
	GS::Application* app = new GS::Application();

	app->Run();

	delete app;
	return 0;
}
