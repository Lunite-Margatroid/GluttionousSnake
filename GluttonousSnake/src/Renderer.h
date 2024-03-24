#pragma once
#include "VBO.h"
#include "Shader.h"
namespace GS
{
	class Renderer
	{
		VertexBuffer m_vbo;
		unsigned int m_VerCount;
		unsigned int m_vao;

		LM::Shader m_Shader;

		void Draw();
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="size"> ���㻺���С </param>
		/// <param name="data"> Ĭ�� = NULL </param>
		Renderer(unsigned int size = 2048, const void* data = NULL);
		~Renderer();

		void DrawRect(const glm::vec2& coord, const glm::vec4& color);
		void DrawBegin();
		void DrawEnd();
	};
}