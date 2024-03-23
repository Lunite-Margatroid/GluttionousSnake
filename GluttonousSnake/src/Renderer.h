#pragma once
#include "VBO.h"
namespace GS
{
	class Renderer
	{
		VertexBuffer m_vbo;

	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="size"> ���㻺���С </param>
		/// <param name="data"> Ĭ�� = NULL </param>
		Renderer(unsigned int size, const void* data = NULL);
		~Renderer();

		void DrawRect(const glm::vec2& coord, const glm::vec4& color);
		void DrawBegin();
		void DrawEnd();
	};
}