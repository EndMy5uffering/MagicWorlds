#pragma once

#include "Noise.hpp"
#include "Chunk.h"


class ChunkGenerator
{

private:
	Noise::Perlin m_noise;

public:

	ChunkGenerator(int seed) : m_noise{ seed } {};

	Chunk GenerateAt(const glm::vec2& pos);
	Chunk GenerateAt(int cx, int cy);

};

