#include "pch.h"
#include "Renderer.h"

namespace GS
{
	void Renderer::Draw()
	{
		glm::mat4 projectionTrans = glm::ortho(0.0f, 30.0f, 0.0f, 30.0f, -1.0f, 1.0f);
		glm::mat4 viewTrans = glm::lookAt(
			glm::vec3(15.0f, 15.0f, 0.0f),
			glm::vec3(15.0f, 15.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		m_Shader.Bind();
		m_Shader.SetUniform1f("u_Width", 1.0f);
		m_Shader.SetUniformMatrix4f("u_ProjectionTrans", false, glm::value_ptr(projectionTrans));
		m_Shader.SetUniformMatrix4f("u_ViewTrans", false, glm::value_ptr(viewTrans));
		GLCall(glBindVertexArray(m_vao));
		GLCall(glDrawArrays(GL_POINTS, 0, m_VerCount));
		m_vbo.ClearData();
	}
	Renderer::Renderer(unsigned int size, const void* data):m_vbo(size, data),m_VerCount(0),
		m_Shader("./res/shader/gs_VertexShader.shader","./res/shader/gs_FragShader.shader","./res/shader/gs_GeoShader.shader")
	{
		GLCall(glGenVertexArrays(1, &m_vao));
		m_vbo.Bind();
		GLCall(glBindVertexArray(m_vao));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0));
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float))));
		GLCall(glEnableVertexAttribArray(1));
	}
	Renderer::~Renderer()
	{
		GLCall(glDeleteVertexArrays(1, &m_vao));
	}
	void Renderer::DrawRect(const glm::vec2& coord, const glm::vec4& color)
	{
		float vertex[6];
		memcpy(vertex, glm::value_ptr(coord), 8);
		memcpy(vertex + 2, glm::value_ptr(color), 16);
		if (m_vbo.AddData(24, vertex))
		{
			m_VerCount += 1;
		}
		else
		{
			Draw();
			m_VerCount = 0;
			if (!m_vbo.AddData(24, vertex))
			{
				std::cout << "[error] 顶点数据超过了顶点缓冲的最大空间" << std::endl;
				__debugbreak();
			}
			m_VerCount+=1;
		}
	}
	void Renderer::DrawBegin()
	{
		m_vbo.ClearData();
	}
	void Renderer::DrawEnd()
	{
		Draw();
		m_VerCount = 0;
	}
}