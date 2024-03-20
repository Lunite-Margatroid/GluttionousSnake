#include "pch.h"
#include "Application.h"

int main()
{
	demo_alpha::Application* app = new demo_alpha::Application();

	app->Run();

	delete app;
	return 0;
}
