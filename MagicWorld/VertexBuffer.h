#pragma once

#include<vector>
#include "glm/glm.hpp"

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned long long size);
	~VertexBuffer();

	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer(const VertexBuffer& other);

	VertexBuffer& operator=(const VertexBuffer& other);
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;

	void Bind() const;
	void Unbind() const;
	void SubData(unsigned int offset,const void* data, unsigned int size);
	void SubData(const void* data, unsigned int size);

	bool isDynamic();

private:
	unsigned int m_RendererID;
	bool mIsDynamic = false;
};

