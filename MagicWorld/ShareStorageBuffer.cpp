#include "ShareStorageBuffer.h"

ShareStorageBuffer::ShareStorageBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

ShareStorageBuffer::~ShareStorageBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

ShareStorageBuffer::ShareStorageBuffer(ShareStorageBuffer&& other) noexcept
    :
    m_RendererID{ other.m_RendererID },
    m_IsDynamic{ other.m_IsDynamic }
{
    other.m_RendererID = 0;
}

ShareStorageBuffer::ShareStorageBuffer(const ShareStorageBuffer& other) 
    :
    m_RendererID{ other.m_RendererID },
    m_IsDynamic{ other.m_IsDynamic }
{
}

ShareStorageBuffer& ShareStorageBuffer::operator=(const ShareStorageBuffer& other)
{
    m_RendererID = other.m_RendererID;
    m_IsDynamic = other.m_IsDynamic;
    return *this;
}

ShareStorageBuffer& ShareStorageBuffer::operator=(ShareStorageBuffer&& other) noexcept
{
    m_RendererID = other.m_RendererID;
    m_IsDynamic = other.m_IsDynamic;
    other.m_RendererID = 0;
    return *this;
}

void ShareStorageBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
}

void ShareStorageBuffer::Bind(int position) const
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, position, m_RendererID);
}

void ShareStorageBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void ShareStorageBuffer::SubData(unsigned int offset, const void* data, unsigned int size) const
{
    Bind();
    GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data));
}

void ShareStorageBuffer::SubData(const void* data, unsigned int size) const
{
    Bind();
    GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data));
}
