#pragma once
#include <vector>
#include <iostream>

class IndexBuffer
{
public:

	IndexBuffer(std::vector<unsigned int>& data); //Normal Constructor
	IndexBuffer(IndexBuffer&& other) noexcept; //Move Constructor
	IndexBuffer(const IndexBuffer& other); //Copy Constructor

	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	void Delete();

	unsigned int GetIndexBufferObjectID();

	inline size_t GetCount() const { return m_data.size(); }
	IndexBuffer& operator=(const IndexBuffer& other); //Copy Assignment
	IndexBuffer& operator=(IndexBuffer&& other) noexcept; //Move Assignment

private:
	unsigned int m_RendererID;
	std::vector<unsigned int> m_data;
};
