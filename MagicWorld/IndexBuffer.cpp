#include "IndexBuffer.h"
#include "Renderer.h"
#include "Logger.h"

IndexBuffer::IndexBuffer(std::vector<unsigned int>& data)
    : m_data{ data }
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data.size() * sizeof(unsigned int), m_data.data(), GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    :
    m_RendererID { other.m_RendererID }
{
    other.m_RendererID = 0;
    this->m_data = std::move(other.m_data);
}

IndexBuffer::IndexBuffer(const IndexBuffer& other)
    :
    m_data{ other.m_data }
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data.size() * sizeof(unsigned int), m_data.data(), GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::Delete()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

unsigned int IndexBuffer::GetIndexBufferObjectID()
{
    return m_RendererID;
}

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& other)
{
    m_RendererID = other.m_RendererID;
    m_data = other.m_data;
    return *this;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    if (this != &other) 
    {
        m_RendererID = other.m_RendererID;
        m_data = std::move(other.m_data);
        other.m_RendererID = 0;
    }
    return *this;
}
