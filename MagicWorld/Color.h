#pragma once

#include <glm/glm.hpp>

struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	static Color hsl(float h, float s, float l);
	static Color hsla(float h, float s, float l, float a);
	static Color rgb(unsigned char r, unsigned char g, unsigned char b);
	static Color rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};