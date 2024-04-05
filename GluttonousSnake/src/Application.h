#pragma once
#include "Renderer.h"
#include "Input.h"
#include "GameScene.h"

namespace GS
{

	class Application
	{
		GLFWwindow* m_Window;
		bool m_Run;

		Renderer* m_Renderer;
		float m_LastTime;
		float m_CurrentTime;
		float m_DeltaTime;

		GameScene* m_GameScene;

		void Init();
		void AppEnd();
		void RenderImGui();
		void Render();
		void UpdateTime();
	public:
		Application();
		~Application();
		void Run();

		static void ApplicationKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};
}