#include "pch.h"
#include "Application.h"
#include "Init.h"

namespace GS
{

    Application::Application() :m_Run(true), m_DemoWindow(true)
    {
        Init();
        m_Renderer = new Renderer();
        Input::Init(m_Window);
    }

    Application::~Application()
    {
        delete m_Renderer;
        Input::FreeInstance();
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

        GLFW_PRESS;
    }

    void Application::Run()
    {
        while (m_Run)
        {
            m_Run = (glfwWindowShouldClose(m_Window) == 0);
            glfwPollEvents();
            Render();

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

        m_Renderer->DrawBegin();
        glViewport(0, 0, 900, 900);
        for(int i=0;i<30;i++)
            for (int j = 0; j < 30; j++)
            {
                m_Renderer->DrawRect(glm::vec2(i, j), glm::vec4( float(i) / 30.f , float(j) / 30.0f, 0.5f, 1.0f));
            }
        m_Renderer->DrawEnd();
        glViewport(0, 0, WIDTH, HEIGHT);
        RenderImGui();
        glfwSwapBuffers(m_Window);
    }

    void Application::UpdateTime()
    {
        static bool first = true;
        if (first)
        {
            m_CurrentTime= glfwGetTime();
            m_LastTime = m_CurrentTime;
            m_DeltaTime = 0.0f;
            first = false;
            return;
        }

        m_CurrentTime = glfwGetTime();
        m_DeltaTime = m_CurrentTime - m_LastTime;
        m_LastTime = m_CurrentTime;

    }

}