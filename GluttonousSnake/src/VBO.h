#pragma once
#include "VertexBuffer.h"

namespace GS
{
	class VertexBuffer : public LM::VertexBuffer
	{
	protected:
		unsigned int m_ValidSize;
	public:
		VertexBuffer(unsigned int size, const void* data = NULL);
		~VertexBuffer();
		void Init(unsigned int size, const void* data = NULL) override;
		bool AddData( unsigned int size, const void* data);
		void ClearData();
		bool IsDataFull();
	};
}