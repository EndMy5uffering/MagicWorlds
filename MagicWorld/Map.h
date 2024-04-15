#pragma once

#include<vector>
#include<unordered_map>

#include<glm/glm.hpp>

#include "Chunk.h"
#include "Renderer.h"

class Map
{
private:
	std::vector<Chunk> m_chunks;

	const int m_seed;

public:

	Map(int seed) : m_seed{ seed } 
	{}

	~Map() {}

	void Update();
	void Render(Renderer& r);


};

