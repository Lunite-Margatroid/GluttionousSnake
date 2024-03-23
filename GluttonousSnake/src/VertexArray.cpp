#include "pch.h"
#include "VertexArray.h"

LM::VertexArray::VertexArray(GLenum type)
	:m_stride(0),m_MetaType(type)
{
	GLCall(glGenVertexArrays(1, &m_id));
	Bind();
}

LM::VertexArray::VertexArray(VertexBuffer* vb, ElementBuffer* eb, GLenum type)
	:m_vb(vb),m_eb(eb)
{
	vb->Bind();
	eb->Bind();
	GLCall(glGenVertexArrays(1, &m_id));
	Bind();
}


LM::VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_id));
}

void LM::VertexArray::ApplyLayout()
{
	int i = 0;
	unsigned int offset = 0;
	if (m_eb)
		m_eb->Bind();
	if (m_vb)
		m_vb->Bind();
	Bind();
	for (auto& attr : m_attribLayout)
	{
		glVertexAttribPointer(i, attr.count , attr.type, attr.bNormalize, \
			m_stride, (void*)offset);
		glEnableVertexAttribArray(i);

		/*GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
		glEnableVertexAttribArray(0);*/

		offset += AttribLayout::GetSizeOfElem(attr.type) * attr.count;

		i++;
	}
}

unsigned int LM::VertexArray::GetID() const
{
	return m_id;
}

GLenum LM::VertexArray::GetMetaType() const
{
	return m_MetaType;
}

void LM::VertexArray::SetMetaType(GLenum metaType)
{
	m_MetaType = metaType;
}

void LM::VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_id));
}

void LM::VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void LM::VertexArray::Draw() const
{
	Bind();
	GLCall(glDrawElements(m_MetaType, m_eb->GetCount(), GL_UNSIGNED_INT, 0));
}

void LM::VertexArray::Draw()
{
	Bind();
	GLCall(glDrawElements(m_MetaType, m_eb->GetCount(), GL_UNSIGNED_INT, 0));
}

void LM::VertexArray::SetVB(VertexBuffer* vb)
{
	m_vb = vb;
}

void LM::VertexArray::SetEB(ElementBuffer* eb)
{
	m_eb = eb;
}

