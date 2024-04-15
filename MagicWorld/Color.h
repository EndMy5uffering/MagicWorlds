#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <math.h>

struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	static Color hsl(float h, float s, float l);
	static Color hsla(float h, float s, float l, float a);
	static Color rgb(unsigned char r, unsigned char g, unsigned char b);
	static Color rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	static Color lerp(const Color a, const Color b, float t);
	static Color lerp(const std::vector<Color> colors, float t);
};