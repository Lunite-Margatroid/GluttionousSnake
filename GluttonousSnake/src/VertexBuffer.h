#pragma once

namespace LM
{
	class VertexBuffer
	{
	protected:
		unsigned int m_id;
		unsigned int m_uSize;
	public:
		VertexBuffer(unsigned int size, void* data);
		VertexBuffer();
		virtual ~VertexBuffer();

		virtual void Init(unsigned int size, const void* data);

		void Bind() const;
		void Unbind() const;

		unsigned int GetID() const;
		unsigned int GetSize() const;

	};
}