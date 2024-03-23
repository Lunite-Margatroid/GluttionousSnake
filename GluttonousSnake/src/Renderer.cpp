#include "pch.h"
#include "Renderer.h"

namespace GS
{
	void Renderer::Draw()
	{
		m_vbo.Bind();
		GLCall(glDrawArrays(GL_POINTS, 0, m_VerCount));
	}
	Renderer::Renderer(unsigned int size, const void* data):m_vbo(size, data),m_VerCount(0)
	{
	}
	Renderer::~Renderer()
	{
	}
	void Renderer::DrawRect(const glm::vec2& coord, const glm::vec4& color)
	{
		float vertex[6];
		memcpy(vertex, glm::value_ptr(coord), 8);
		memcpy(vertex + 2, glm::value_ptr(color), 16);
		if (m_vbo.AddData(0, 24, vertex))
		{
			m_VerCount += 1;
		}
		else
		{
			Draw();
			m_VerCount = 0;
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