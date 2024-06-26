#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	
}

VertexArray::VertexArray(VertexArray&& other) noexcept
	:
	m_RendererID { other.m_RendererID }
{
	other.m_RendererID = 0;
}

VertexArray::VertexArray(const VertexArray& other)
	:
	m_RendererID{ other.m_RendererID }
{

}

VertexArray& VertexArray::operator=(const VertexArray& other)
{
	m_RendererID = other.m_RendererID;
	return *this;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	m_RendererID = other.m_RendererID;
	other.m_RendererID = 0;
	return *this;
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	
	const auto& elemetns = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elemetns.size(); ++i) 
	{
		const auto& element = elemetns[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::Delete()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

unsigned int VertexArray::GetVertexArrayObjectID()
{
	return m_RendererID;
}
