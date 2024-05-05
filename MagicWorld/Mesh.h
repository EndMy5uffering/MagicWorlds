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

	unsigned int m_vbo_id;
	unsigned int m_ibo_id;

	bool m_packed = false;

public:

	Mesh();
	Mesh(const std::vector<T>& verts, const std::vector<unsigned int>& idx_buffer, const VertexBufferLayout& vbol);

	~Mesh();

	void Bind();
	void Unbind();
	void Delete();

	void Pack();
	void PackDynamic();

	void Repack();
	void RepackDynamic();

	void Draw(Renderer& render);

	void AddVertex(const T vertex);
	void SetVertexBufferContent(const std::vector<T>& vBuffer);
	/**
	* Updates the mesh data
	* @param offset the item offset as an index to the beginig of the data buffer (not in bytes) 
	* @param vBuffer the new vertex buffer data that is beeing set at index offset
	*/
	void UpdateVertexBufferContent(size_t offset, const std::vector<T>& vBuffer);
	void AddIndex(unsigned int idx);
	void SetIndexBuffer(const std::vector<unsigned int>& idxBuffer);

};

template<typename T>
inline Mesh<T>::Mesh()
{
}

template<typename T>
inline Mesh<T>::Mesh(const std::vector<T>& verts, const std::vector<unsigned int>& idx_buffer, const VertexBufferLayout& vaol)
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
inline void Mesh<T>::Delete()
{
	GLCall(glDeleteBuffers(1, &m_ibo_id));
	m_ibo_id = 0;
	GLCall(glDeleteBuffers(1, &m_vbo_id));
	m_vbo_id = 0;
	m_vao.Delete();
}

template<typename T>
inline void Mesh<T>::Pack()
{ 
	if (m_packed)
	{
		std::cout << "Can not pack mesh twice!\n";
		return;
	}
	m_vao.Bind();
	VertexBuffer vbo{ m_Verts.data(), sizeof(T) * static_cast<unsigned long long>(m_Verts.size())};
	m_vbo_id = vbo.GetVertexBufferObjectID();
	IndexBuffer ibo{ m_IdxBuffer };
	m_ibo_id = ibo.GetIndexBufferObjectID();
	m_vao.AddBuffer(vbo, m_vaol);

	m_vao.Unbind();
	vbo.Unbind();
	ibo.Unbind();

	m_packed = true;
}

template<typename T>
inline void Mesh<T>::PackDynamic()
{
	if (m_packed) 
	{
		std::cout << "Can not pack mesh twice!\n";
		return;
	}
	m_vao.Bind();
	VertexBuffer vbo{ nullptr, sizeof(T) * static_cast<unsigned long long>(m_Verts.size()) };
	vbo.SubData(m_Verts.data(), sizeof(T) * static_cast<unsigned long long>(m_Verts.size()));
	m_vbo_id = vbo.GetVertexBufferObjectID();
	IndexBuffer ibo{ m_IdxBuffer };
	m_ibo_id = ibo.GetIndexBufferObjectID();
	m_vao.AddBuffer(vbo, m_vaol);

	m_vao.Unbind();
	vbo.Unbind();
	ibo.Unbind();

	m_packed = true;
}

template<typename T>
inline void Mesh<T>::Repack()
{
	if (!m_packed) 
	{
		std::cout << "Can not repack mesh if it was not packed before!\n";
		return;
	}
	Delete();

	m_vao = VertexArray{};
	m_vao.Bind();
	VertexBuffer vbo{ m_Verts.data(), sizeof(T) * static_cast<unsigned long long>(m_Verts.size()) };
	m_vbo_id = vbo.GetVertexBufferObjectID();
	IndexBuffer ibo{ m_IdxBuffer };
	m_ibo_id = ibo.GetIndexBufferObjectID();
	m_vao.AddBuffer(vbo, m_vaol);

	m_vao.Unbind();
	vbo.Unbind();
	ibo.Unbind();

	m_packed = true;
}

template<typename T>
inline void Mesh<T>::RepackDynamic()
{
	if (!m_packed)
	{
		std::cout << "Can not repack mesh if it was not packed before!\n";
		return;
	}
	Delete();

	m_vao = VertexArray{};
	m_vao.Bind();
	VertexBuffer vbo{ nullptr, sizeof(T) * static_cast<unsigned long long>(m_Verts.size()) };
	vbo.SubData(m_Verts.data(), sizeof(T) * static_cast<unsigned long long>(m_Verts.size()));
	m_vbo_id = vbo.GetVertexBufferObjectID();
	IndexBuffer ibo{ m_IdxBuffer };
	m_ibo_id = ibo.GetIndexBufferObjectID();
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
inline void Mesh<T>::SetVertexBufferContent(const std::vector<T>& vBuffer)
{
	m_Verts = vBuffer;
}

template<typename T>
inline void Mesh<T>::UpdateVertexBufferContent(size_t offset, const std::vector<T>& vBuffer)
{
	if (m_vbo_id == 0) 
	{
		std::cout << "Vertex buffer obejct not set in mesh!\n";
		return;
	}
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(T), vBuffer.size(), vBuffer.data()));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
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
