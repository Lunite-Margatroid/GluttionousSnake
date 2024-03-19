#pragma once
class Appliaction
{
	GLFWwindow* m_Window;
	bool m_Run;
	Appliaction();
	~Appliaction();
	void Init();
	void Run();
	void AppEnd();
};