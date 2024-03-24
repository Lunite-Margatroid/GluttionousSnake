#include "pch.h"
#include "VBO.h"

namespace GS
{
	VertexBuffer::VertexBuffer(unsigned int size, const void *data):m_ValidSize(0)
	{
		Init(size, data);
	}
	VertexBuffer::~VertexBuffer()
	{
	}
	void VertexBuffer::Init(unsigned int size, const void* data)
	{
		m_uSize = size;
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_id));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
	}
	bool VertexBuffer::AddData( unsigned int size, const void* data)
	{
		if(size + m_ValidSize > m_uSize)
			return false;
		Bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_ValidSize, size, data));
		m_ValidSize += size;
		return true;
	}
	void VertexBuffer::ClearData()
	{
		m_ValidSize = 0;
	}
	bool VertexBuffer::IsDataFull()
	{
		return m_ValidSize >= m_uSize;
	}
}