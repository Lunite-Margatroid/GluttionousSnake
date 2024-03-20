#include "pch.h"
#include "Application.h"
#include "Init.h"

namespace demo_alpha
{

    Application::Application() :m_Run(true), m_DemoWindow(true)
    {
        Init();
    }

    Application::~Application()
    {
        AppEnd();
    }

    void Application::Init()
    {
        m_Window = init::InitGL();

        // Setup ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        glfwSwapInterval(1); // Enable vsync
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        const char* glsl_version = "#version 410";
        ImGui_ImplOpenGL3_Init(glsl_version);

        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    }

    void Application::Run()
    {
        while (m_Run)
        {
            glfwPollEvents();
            Render();


            m_Run = !glfwWindowShouldClose(m_Window);
        }
    }

    void Application::AppEnd()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Application::RenderImGui()
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow(&m_DemoWindow);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Application::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        RenderImGui();
        glfwSwapBuffers(m_Window);
    }

}