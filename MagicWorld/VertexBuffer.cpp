#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned long long size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    if (data) 
    {
        mIsDynamic = false;
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }
    else 
    {
        mIsDynamic = true;
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    :
    m_RendererID{ other.m_RendererID },
    mIsDynamic{other.mIsDynamic}
{
    other.m_RendererID = 0;
}

VertexBuffer::VertexBuffer(const VertexBuffer& other)
    :
    m_RendererID{ other.m_RendererID },
    mIsDynamic{ other.mIsDynamic }
{

}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other)
{
    this->m_RendererID = other.m_RendererID;
    this->mIsDynamic = other.mIsDynamic;
    return *this;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    this->m_RendererID = other.m_RendererID;
    this->mIsDynamic = other.mIsDynamic;
    other.m_RendererID = 0;
    return *this;
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::SubData(unsigned int offset, const void* data, unsigned int size)
{
    Bind();
    if (mIsDynamic)
    {
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
    }
}

void VertexBuffer::SubData(const void* data, unsigned int size)
{
    Bind();
    if (mIsDynamic)
    {
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
    }
}

bool VertexBuffer::isDynamic()
{
    return mIsDynamic;
}
