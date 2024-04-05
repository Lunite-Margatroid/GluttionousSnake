#include "pch.h"
#include "Application.h"
#include "Init.h"

namespace GS
{

    Application::Application() :m_Run(true)
    {
        Init();
        m_Renderer = new Renderer();
        Input::Init(m_Window);
        m_GameScene = GameScene::GetInstance();
    }

    Application::~Application()
    {
        delete m_Renderer;
        delete m_GameScene;
        Input::FreeInstance();
        AppEnd();
        MutexLock::Free();
    }

    void Application::Init()
    {
        m_Window = init::InitGL();
        // glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GL_TRUE);


        // Setup ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        glfwSwapInterval(1); // Enable vsync
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows


        io.FontGlobalScale = 2.f;

        // For correct scaling, prefer to reload font + rebuild ImFontAtlas + call style.ScaleAllSizes().


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

        style.ScaleAllSizes(2.0f);

        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

        glfwSetKeyCallback(m_Window, ApplicationKeyCallback);

        
    }

    void Application::Run()
    {
        m_GameScene->Interrupt();
        std::thread thr(&(GameScene::Update), m_GameScene);

        while (m_Run)
        {
            m_Run = (glfwWindowShouldClose(m_Window) == 0);
            glfwPollEvents();
            Render();
            //std::cout << "[info] Application Running.\n";
            //m_GameScene->Update();
        }
        thr.join();
    }

    void Application::ApplicationKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        ImGui_ImplGlfw_KeyCallback(window,  key, scancode, action, mods);
        GameScene::GetInstance()->KeyCallback(key, action);
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
        static double speed = 1.0f;
        static int lengthen = 1;
        static bool throughWall = true;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Console");

        //ImGui::SetWindowFontScale(3.f);

        if (m_GameScene->GetInterruptFlag() && !m_GameScene->IsGameOver())    // 暂停状态
        {
            
            ImGui::Text("Status: Pause", ImVec2(40 ,40));
            if (ImGui::Button("Restart"))
            {
                m_GameScene->Continue();
            }
        }
        if(!m_GameScene->GetInterruptFlag() && !m_GameScene->IsGameOver())
        {
            ImGui::Text("Status: Run");
            if(ImGui::Button("Game Pause"))
                m_GameScene->Interrupt();
        }

        if (m_GameScene->IsGameOver())          // 撞墙或者吃到自己
        {
            ImGui::Text("Status: Game Over");
            if (ImGui::Button("Start"))
            {
                m_GameScene->Reset();
                m_GameScene->SetHitWall(!throughWall);
                m_GameScene->SetSpeed(speed);
                m_GameScene->SetLengthen(lengthen);
            }
            // 调节速度
            speed = m_GameScene->GetSpeed();
            ImGui::Text("Speed(s/quad): ");
            ImGui::SameLine();
            if (ImGui::Button("-"))
            {
                if(speed > 0.0)
                    speed -= 1.0 / 64;
                m_GameScene->SetSpeed(speed);
            }
            ImGui::SameLine();
            ImGui::Text("%lf", speed);
            ImGui::SameLine();
            if (ImGui::Button("+"))
            {
                if (speed < 1.0)
                    speed += 1.0 / 64;
                m_GameScene->SetSpeed(speed);
            }

            // 调节伸长
            lengthen = m_GameScene->GetLengthen();
            ImGui::Text("Lengthen(quad/food): ");
            ImGui::SameLine();
            if (ImGui::Button("down"))
            {
                if (lengthen > 1)
                {
                    lengthen -= 1;
                    m_GameScene->SetLengthen(lengthen);
                }
            }
            ImGui::SameLine();
            ImGui::Text("%d", lengthen);
            ImGui::SameLine();
            if (ImGui::Button("up"))
            {
                if (lengthen < 10)
                {
                    lengthen += 1;
                    m_GameScene->SetLengthen(lengthen);
                }
            }

            // 调节撞墙
            throughWall = !(m_GameScene->GetHitWall());
            ImGui::Checkbox("Through Wall",&throughWall);
            m_GameScene->SetHitWall(!throughWall);
        }
        if (ImGui::Button("Exit"))
        {
            m_GameScene->GameThreadOver();
            m_Run = false;
        }
        ImGui::End();
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
                m_Renderer->DrawRect(glm::vec2(i, j), glm::vec4( 0.0, 0.0, 0.0, 1.0f));
            }
        
        m_Renderer->DrawEnd();

        m_Renderer->DrawBegin();
        MutexLock::Lock("Food");
        m_Renderer->DrawRect(m_GameScene->GetFoodPos(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        MutexLock::Unlock("Food");
        MutexLock::Lock("Snake");
        m_GameScene->ForEach([&](const glm::vec2& pos) {
            m_Renderer->DrawRect(pos, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            });
        MutexLock::Unlock("Snake");
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