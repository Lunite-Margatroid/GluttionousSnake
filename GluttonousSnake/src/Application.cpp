#include "pch.h"
#include "Application.h"
#include "Init.h"

Application::Application():m_Run(true),m_DemoWindow(true)
{
    Init();
}

Application::~Application()
{
}

void Application::Init()
{
	m_Window = init::InitGL();

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    //ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
}

void Application::RenderImGui()
{
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

