#pragma once

#include<vector>
#include<map>
#include<unordered_map>
#include<tuple>

#include<glm/glm.hpp>

#include "Chunk.h"
#include "Renderer.h"

class Map
{
private:
	std::vector<Chunk> m_chunks;
	std::unordered_map<ChunkPos, Chunk> m_pos_chunk;

	const int m_seed;

public:

	Map(int seed) : m_seed{ seed } 
	{}

	~Map() {}

	void Update();
	void Render(Renderer& r);


};

