#pragma once

#include <string>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>

#include "Logger.h"
#include "Color.h"

typedef unsigned char* ColorData;

enum MinFilter 
{
	MIN_NEAREST = GL_NEAREST,
	MIN_LINEAR = GL_LINEAR,
	NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

enum MagFilter 
{
	MAG_NEAREST = GL_NEAREST,
	MAG_LINEAR = GL_LINEAR
};

enum WrapST 
{
	CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
	MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
	REPEAT = GL_REPEAT,
	MIRROR_CLAMP_TO_EDGE  = GL_MIRROR_CLAMP_TO_EDGE
};

enum CMode 
{
	RGB = GL_RGB,
	RGBA = GL_RGBA
};

class Texture
{

private:
	unsigned int m_width{0};
	unsigned int m_height{0};

	unsigned int m_texture_id{0};
	ColorData m_data{0};

	CMode m_cmode{ RGBA };

	MinFilter m_min_filter{ MIN_LINEAR }; //GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
	MagFilter m_mag_filter{ MAG_LINEAR }; //GL_NEAREST, GL_LINEAR
	WrapST m_wrap_s{ WrapST::REPEAT }; //GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_REPEAT, or GL_MIRROR_CLAMP_TO_EDGE
	WrapST m_wrap_t{ WrapST::REPEAT }; //GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_REPEAT, or GL_MIRROR_CLAMP_TO_EDGE

	int m_last_bound_unit{0};

	bool m_packed = false;
	bool m_data_allocated = false;

	int remaped_stbi_color_mode(CMode cmode)
	{
		switch (cmode)
		{
		case RGB:
			return STBI_rgb;
		case RGBA:
			return STBI_rgb_alpha;
		default:
			return STBI_default;
		}
	}
	


public:
	
	Texture();
	Texture(ColorData data, CMode cmode, unsigned int width, unsigned int height);
	~Texture();

	void Pack();
	void Bind();
	void BindToSlot(int unit);
	void Unbind();
	void Delete();
	void LoadFromFile(std::string path);

	void SetPixel(unsigned int x, unsigned int y, const Color& c);
	Color GetPixel(unsigned int x, unsigned int y);
	Texture GetPixels(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
	//Color GetPixelsUV(unsigned int u0, unsigned int v0, unsigned u1, unsigned v1);

	void DrawLine(const glm::vec2& start, const glm::vec2& end, const Color& c);
	void DrawLineH(const int x0, const int x1, const int y0, const Color& c);
	void DrawLineV(const int x0, const int y0, const int y1, const Color& c);
	void DrawCicle(const glm::vec2& center, int radius, const Color& c);
	void DrawHCicle(const glm::vec2& center, int radius, const Color& c);
	void DrawRect(const glm::vec2& center, const glm::vec2& dim, const Color& c);
	void DrawHRect(const glm::vec2& center, const glm::vec2& dim, const Color& c);

	int GetWidth() { return this->m_width; }
	int GetHeight() { return this->m_height; }
	CMode GetColorMode() { return this->m_cmode; }
	MinFilter GetMinFilter() { return this->m_min_filter; }
	MagFilter GetMagFilter() { return this->m_mag_filter; }
	WrapST GetWrapS() { return this->m_wrap_s; }
	WrapST GetWrapT() { return this->m_wrap_t; }

	bool IsEmpty() { return m_texture_id != 0 && m_data_allocated && m_data != nullptr; }

	void SetMinFilter(MinFilter minFilter);
	void SetMagFilter(MagFilter magFilter);
	void SetWrapS(WrapST wrap);
	void SetWrapT(WrapST wrap);
	void SetWrapST(WrapST wrap);

	void SetColorMode(CMode cmode);
	ColorData GetColorDataRef() { return m_data; }

	Color operator[](const glm::vec2& px_pos);
	
	static glm::vec2 TranslateToUV(const glm::vec2& px_xy, unsigned int width, unsigned int height);
	static glm::vec2 TranslateToXY(const glm::vec2& uv, unsigned int width, unsigned int height);

	unsigned int TranslateXY(unsigned int x, unsigned int y);

	// Generates an empty unusable image texture
	static Texture Empty();

};

