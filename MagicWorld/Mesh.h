#pragma once
#include "glm/glm.hpp"

#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Renderer.h"

template<typename T>
class Mesh
{
private:
	std::vector<T> m_Verts{};
	std::vector<unsigned int> m_IdxBuffer{};
	
	VertexArray m_vao{};
	VertexBufferLayout m_vaol{};

	bool m_packed = false;

public:

	Mesh();
	Mesh(std::vector<T>& verts, std::vector<unsigned int>& idx_buffer, VertexBufferLayout& vbol);

	~Mesh();

	void Bind();
	void Unbind();

	void Pack();

	void Draw(Renderer& render);

	void AddVertex(const T vertex);
	void SetVertexBuffer(const std::vector<T>& vBuffer);
	void AddIndex(unsigned int idx);
	void SetIndexBuffer(const std::vector<unsigned int>& idxBuffer);

};

template<typename T>
inline Mesh<T>::Mesh()
{
}

template<typename T>
inline Mesh<T>::Mesh(std::vector<T>& verts, std::vector<unsigned int>& idx_buffer, VertexBufferLayout& vaol)
	:
	m_vaol{ vaol },
	m_IdxBuffer{ idx_buffer },
	m_Verts{ verts }
{
	Pack();
}

template<typename T>
inline Mesh<T>::~Mesh()
{
}

template<typename T>
inline void Mesh<T>::Bind()
{
	if (!m_packed)
	{
		std::cout << "Mesh not packed! If a mesh is created without a vertex buffer or index buffer it has to be packed first.\n";
		return;
	}

	m_vao.Bind();

}

template<typename T>
inline void Mesh<T>::Unbind()
{

	m_vao.Unbind();

}

template<typename T>
inline void Mesh<T>::Pack()
{ 
	m_vao.Bind();
	VertexBuffer vbo{ m_Verts.data(), sizeof(T) * static_cast<unsigned long long>(m_Verts.size())};
	IndexBuffer ibo{ m_IdxBuffer };
	m_vao.AddBuffer(vbo, m_vaol);

	m_vao.Unbind();
	vbo.Unbind();
	ibo.Unbind();

	m_packed = true;
}

template<typename T>
inline void Mesh<T>::Draw(Renderer& render)
{
	render.Draw(m_vao, m_IdxBuffer.size());
}

template<typename T>
inline void Mesh<T>::AddVertex(const T vertex)
{
	m_Verts.push_back(vertex);
}

template<typename T>
inline void Mesh<T>::SetVertexBuffer(const std::vector<T>& vBuffer)
{
	m_Verts = vBuffer;
}

template<typename T>
inline void Mesh<T>::AddIndex(unsigned int idx)
{
	m_IdxBuffer.push_back(idx);
}

template<typename T>
inline void Mesh<T>::SetIndexBuffer(const std::vector<unsigned int>& idxBuffer)
{
	m_IdxBuffer = idxBuffer;
}
