#pragma once

#include<glm/glm.hpp>

#include"Mesh.h"

struct MVert 
{

	glm::vec4 VPosition;
	glm::vec4 VNormal;
	glm::vec4 VColor;

};

struct ChunkPos 
{
	int x;
	int y;
};

namespace std {
	template <>
	struct hash<ChunkPos> {
		size_t operator()(const ChunkPos& pos) const {
			return hash<int>()(pos.x) ^ hash<int>()(pos.y);
		}
	};
}

struct ChunkData 
{
	const Mesh<MVert> mesh;
	const ChunkPos pos;

};

class Chunk
{

private:
	ChunkData m_chunkData;

public:

	Chunk();

};

