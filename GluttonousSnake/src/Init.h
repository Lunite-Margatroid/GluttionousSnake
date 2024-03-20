#pragma once

namespace init
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	GLFWwindow* InitGL()
	{
		// 初始化glfw
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// 多重采样MSAA
		// 如果在使用帧缓冲的情况下 要在帧缓冲使用MSAA
		// glfwWindowHint(GLFW_SAMPLES, 4);

		// 初始化窗口
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GluttonousSnake", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Fail to create a window" << std::endl;
			glfwTerminate();
			return NULL;
		}

		// 生成上下文
		glfwMakeContextCurrent(window);

		// 加载opengl函数 要在生成上下文之后
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Fail to initialize glad" << std::endl;
			glfwTerminate();
			return NULL;
		}
		// 开启垂直同步
		glfwSwapInterval(1);

		// 设置视口
		GLCall(glViewport(0, 0, WIDTH, HEIGHT));

		// 捕捉cursor
		// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// 声明回调函数
		/*void framebuffer_size_callback(GLFWwindow * window, int width, int height);
		void scroll_callback(GLFWwindow * window, double offsetx, double offsery);
		void mouse_callback(GLFWwindow * window, double xpos, double ypos);
		void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);*/

		void framebuffer_size_callback(GLFWwindow * window, int width, int height);


		// 注册回调函数
		/*glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetKeyCallback(window, key_callback);*/

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		// 深度测试选项
		//GLCall(glEnable(GL_DEPTH_TEST));					// 开启深度检测
		//GLCall(glDepthMask(GL_FALSE));						// 只读深度缓冲
		GLCall(glDepthFunc(GL_LESS));						// 深度缓冲通过条件  默认为less


		// 模版缓冲选项
		// GLCall(glEnable(GL_STENCIL_TEST));

		GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.f));			// 清屏颜色

		// stbi_set_flip_vertically_on_load(true);
		return window;
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

}