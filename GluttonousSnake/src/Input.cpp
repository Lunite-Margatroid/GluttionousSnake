#include "pch.h"
#include "Input.h"

namespace GS
{
	Input* Input::s_Instance = nullptr;

	Input* Input::GetInstance()
	{
		if (!s_Instance)
		{
			std::cout << "ÉÐÎ´³õÊ¼»¯" << std::endl;
			__debugbreak();
		}
		return s_Instance;
	}

	Input::Input(GLFWwindow* window):m_Window(window)
	{}

	void Input::Init(GLFWwindow* window)
	{
		Input::s_Instance = new Input(window);
	}

	bool Input::IsKeyPressed(int keycode)
	{
		auto window = s_Instance->m_Window;
		int status = glfwGetKey(window, keycode);
		return status == GLFW_PRESS || status == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int mousecode)
	{
		auto window = s_Instance->m_Window;
		int status = glfwGetMouseButton(window, mousecode);
		return status == GLFW_PRESS;
	}

	bool Input::IsKeyPressing(int keycode)
	{
		auto window = s_Instance->m_Window;
		int status = glfwGetKey(window, keycode);
		return status == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		double xpos, ypos;
		glfwGetCursorPos(s_Instance->m_Window, &xpos, &ypos);

		return std::pair<float, float>((float)xpos, (float)ypos);
	}

	float Input::GetMouseX()
	{
		auto coord = GetMousePosition();
		return coord.first;
	}

	float Input::GetMouseY()
	{
		auto coord = GetMousePosition();
		return coord.second;
	}
	void Input::SetWindow(GLFWwindow* window)
	{
		GetInstance()->m_Window = window;
	}
	void Input::FreeInstance()
	{
		if (s_Instance)
			delete s_Instance;
	}
}
