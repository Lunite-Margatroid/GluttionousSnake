#include "pch.h"
#include "Application.h"
#include "Init.h"

namespace GS
{

    Application::Application() :m_Run(true), m_DemoWindow(false)
    {
        Init();
        m_Renderer = new Renderer();
        Input::Init(m_Window);
        m_GameScene = new GameScene(0.2f);
    }

    Application::~Application()
    {
        delete m_Renderer;
        delete m_GameScene;
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
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        const char* glsl_version = "#version 410";
        ImGui_ImplOpenGL3_Init(glsl_version);

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    }

    void Application::Run()
    {
        while (m_Run)
        {
            m_Run = (glfwWindowShouldClose(m_Window) == 0);
            glfwPollEvents();
            Render();

            m_GameScene->Update();
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
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("DemoWindow");
        ImGui::Checkbox("DemoWindow", &m_DemoWindow);
        ImGui::End();
        if(m_DemoWindow)
            ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void Application::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, 900, 900);

        m_Renderer->DrawBegin();
        for(int i=0;i<30;i++)
            for (int j = 0; j < 30; j++)
            {
                m_Renderer->DrawRect(glm::vec2(i, j), glm::vec4( float(i) / 30.f , float(j) / 30.0f, 0.5f, 1.0f));
            }
        m_Renderer->DrawEnd();

        m_Renderer->DrawBegin();
        m_Renderer->DrawRect(m_GameScene->GetFoodPos(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        m_GameScene->ForEach([&](const glm::vec2& pos) {
            m_Renderer->DrawRect(pos, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            });
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