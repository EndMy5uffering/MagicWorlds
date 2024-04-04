#pragma once
#include "glm/glm.hpp"

#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

template<typename T>
class Mesh
{
private:
	std::vector<T> m_Verts;
	std::vector<unsigned int> m_IdxList;
	
	VertexArray m_vao;

public:

};

