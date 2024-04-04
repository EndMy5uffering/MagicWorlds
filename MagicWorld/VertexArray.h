#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	VertexArray(VertexArray&& other) noexcept;
	VertexArray(const VertexArray& other);

	VertexArray& operator=(const VertexArray& other);
	VertexArray& operator=(VertexArray&& other) noexcept;

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
};

