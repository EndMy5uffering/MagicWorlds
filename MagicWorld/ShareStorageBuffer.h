#pragma once

#include "Renderer.h"

class ShareStorageBuffer
{
public:
	ShareStorageBuffer(const void* data, unsigned int size);
	~ShareStorageBuffer();

	ShareStorageBuffer(ShareStorageBuffer&& other) noexcept;
	ShareStorageBuffer(const ShareStorageBuffer& other);

	ShareStorageBuffer& operator=(const ShareStorageBuffer& other);
	ShareStorageBuffer& operator=(ShareStorageBuffer&& other) noexcept;

	void Bind() const;
	void Bind(int position) const;
	void Unbind() const;
	void SubData(unsigned int offset, const void* data, unsigned int size) const;
	void SubData(const void* data, unsigned int size) const;

	bool isDynamic() { return this->m_IsDynamic; }

private:
	unsigned int m_RendererID;
	bool m_IsDynamic = false;

};

