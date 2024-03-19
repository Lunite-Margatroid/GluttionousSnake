#pragma once

class Application
{
	GLFWwindow* m_Window;
	bool m_Run;
	bool m_DemoWindow;
	Application();
	~Application();
	void Init();
	void Run();
	void AppEnd();
	void RenderImGui();
};