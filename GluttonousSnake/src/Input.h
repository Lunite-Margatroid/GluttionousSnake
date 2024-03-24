#pragma once
namespace GS
{
	class Input
	{
	private:
		GLFWwindow* m_Window;
		static Input* s_Instance;
		static Input* GetInstance();
		Input(GLFWwindow* window);
	public:
		static void Init(GLFWwindow* window);

		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int mousecode);
		static bool IsKeyPressing(int keycode);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
		static void SetWindow(GLFWwindow* window);
		static void FreeInstance();
	};
}