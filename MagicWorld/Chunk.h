#pragma once

#include<glm/glm.hpp>

#include"Mesh.h"

struct MVert 
{

	glm::vec4 VPosition;
	glm::vec4 VNormal;
	glm::vec4 VColor;

};

struct ChunkData 
{
	const Mesh<MVert> mesh;
	const glm::vec2 pos;

};

class Chunk
{

private:
	ChunkData m_chunkData;

public:

	Chunk();

};

