#pragma once
#include "Renderer.h"
namespace GS
{

	class Application
	{
		GLFWwindow* m_Window;
		bool m_Run;
		bool m_DemoWindow;

		Renderer* m_Renderer;


		void Init();
		void AppEnd();
		void RenderImGui();
		void Render();
	public:
		Application();
		~Application();
		void Run();
	};
}