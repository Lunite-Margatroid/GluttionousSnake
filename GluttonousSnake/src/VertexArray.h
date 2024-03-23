#pragma once
#include "VertexBuffer.h"
#include "ElementBuffer.h"
namespace LM {
	struct AttribLayout
	{
		unsigned int count;
		unsigned int type;
		bool bNormalize;
		
		AttribLayout(unsigned int c, unsigned int t, bool b)
			:count(c), type(t),bNormalize(b)
		{}

		static unsigned int GetSizeOfElem(unsigned int typ)
		{
			switch (typ)
			{
			case GL_FLOAT:return 4;
			case GL_UNSIGNED_INT:return 4;
			case GL_UNSIGNED_BYTE:return 1;
			}
			ASSERT(false);
			return 0;
		}
	};



	class VertexArray
	{
	private:
		unsigned int m_id;
		unsigned int m_stride;
		VertexBuffer* m_vb;
		ElementBuffer* m_eb;
		std::vector<AttribLayout> m_attribLayout;
		GLenum m_MetaType;
	public:
		VertexArray(GLenum type = GL_TRIANGLES);
		VertexArray(VertexBuffer*, ElementBuffer*, GLenum type = GL_TRIANGLES);
		virtual ~VertexArray();

		//void AddBuffer();

		// 输入当前属性的维度
		template<class T>
		void PushAttrib(int count) {ASSERT(false)};

		// 输入当前属性的维度
		template<>
		void PushAttrib<float>(int count)
		{
			m_attribLayout.push_back(AttribLayout(count, GL_FLOAT, GL_FALSE));
			m_stride += count * 4;
		}

		// 输入当前属性的维度
		template<>
		void PushAttrib<unsigned int>(int count)
		{
			m_attribLayout.push_back(AttribLayout(count, GL_UNSIGNED_INT, GL_FALSE ));
			m_stride += count * 4;
		}

		// 输入当前属性的维度
		template<>
		void PushAttrib<unsigned char>(int count)
		{
			m_attribLayout.push_back(AttribLayout(count, GL_UNSIGNED_BYTE, GL_TRUE ));
			m_stride += count;
		}
	
		void ApplyLayout();

		unsigned int GetID() const;

		GLenum GetMetaType() const;

		void SetMetaType(GLenum);

		void Bind() const;
		void Unbind() const;

		void Draw() const;
		void Draw();

		void SetVB(VertexBuffer* vb);
		void SetEB(ElementBuffer* eb);
	};
}