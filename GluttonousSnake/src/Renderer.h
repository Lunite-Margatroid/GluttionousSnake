#pragma once
#include "VBO.h"
namespace GS
{
	class Renderer
	{
		VertexBuffer m_vbo;

	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="size"> 顶点缓冲大小 </param>
		/// <param name="data"> 默认 = NULL </param>
		Renderer(unsigned int size, const void* data = NULL);
		~Renderer();

		void DrawRect(const glm::vec2& coord, const glm::vec4& color);
		void DrawBegin();
		void DrawEnd();
	};
}