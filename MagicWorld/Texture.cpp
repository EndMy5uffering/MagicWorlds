#include "Texture.h"

void printRaw(int width, int height, unsigned char* data)
{
    std::cout << "Width: " << width << " Height: " << height << "\n";
    for (int y = 0; y < height; ++y)
    {
        std::cout << y << "\t";
        for (int x = 0; x < width*4; x += 4)
        {
            std::cout << "(" << int(data[x + (y * width)]) << ";" << int(data[x+1 + (y * width)]) << ";" << int(data[x+2 + (y * width)]) << ";" << int(data[x+3 + (y * width)]) << ")";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n\n";
}

Texture::Texture() : m_packed{ false }
{
    GLCall(glGenTextures(1, &m_texture_id));
}

Texture::Texture(ColorData data, CMode cmode, int width, int height)
    :
    m_width{width},
    m_height{height},
    m_data{data},
    m_cmode{cmode},
    m_packed{ false }
{
    if (data)
    {
        GLCall(glGenTextures(1, &m_texture_id));
    }
    else
    {
        m_width = 0;
        m_height = 0;
        m_data = nullptr;
        std::cout << "Could not create image, no image data provided.\n";
    }

}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_texture_id));
}

void Texture::Pack()
{
    if (m_data == nullptr) 
    {
        std::cout << "No image data found, could not pack texture.\n";
        return;
    }
    if (m_texture_id == 0)
    {
        std::cout << "Textrue was not generated.\n";
        return;
    }
    m_packed = true;

    Bind();
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_filter));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_mag_filter));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_s));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_t));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, m_cmode, m_width, m_height, 0, m_cmode, GL_UNSIGNED_BYTE, m_data));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    Unbind();

}

void Texture::Bind()
{
    if (!m_packed) 
    {
        std::cout << "Pack texture at least once before binding!\n";
        return;
    }
    GLCall(glBindTexture(GL_TEXTURE_2D, m_texture_id));
}

void Texture::BindToSlot(int unit)
{
    glBindTextureUnit(unit, m_texture_id);
    m_last_bound_unit = unit;
}

void Texture::Unbind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::LoadFromFile(std::string path)
{
    int numColCh;
    m_data = stbi_load(path.c_str(), &m_width, &m_height, &numColCh, remaped_stbi_color_mode(m_cmode));

    if (!m_data)
    {
        std::cout << "Could not load image: " << path << "\n";
        GLCall(glDeleteTextures(1, &m_texture_id));
        m_width = 0;
        m_height = 0;
        m_texture_id = 0;
    }

}

void Texture::SetPixel(unsigned int x, unsigned int y, const Color& c)
{
    if (!m_data) return; // Faulty data
    if (x < 0 || y < 0 || x > m_width || y > m_height) return; // Bound check

    unsigned int idx = xy_idx(x, y);
    switch (m_cmode)
    {
    case RGB:
    {
        m_data[idx] = c.r;
        m_data[idx + 1] = c.g;
        m_data[idx + 2] = c.b;
    } break;
    case RGBA: 
    {
        m_data[idx] = c.r;
        m_data[idx + 1] = c.g;
        m_data[idx + 2] = c.b;
        m_data[idx + 3] = c.a;
    } break;
    default:
        std::cout << "Color mode not supported!\n";
        break;
    }
}

Color Texture::GetPixel(unsigned int x, unsigned int y)
{
    if (!m_data) return {0, 0, 0, 0}; // Faulty data
    if (x > m_width || y > m_height) return {0, 0, 0, 0}; // Bound check

    unsigned int idx = xy_idx(x, y);
    switch (m_cmode)
    {
    case RGB:
    {
        return { m_data[idx], m_data[idx + 1], m_data[idx + 2], 255 };
    }
    case RGBA:
    {
        return { m_data[idx], m_data[idx + 1], m_data[idx + 2], m_data[idx + 3] };
    }
    default:
        std::cout << "Color mode not supported!\n";
        break;
    }
}

void Texture::DrawLine(const glm::vec2& start, const glm::vec2& end, const Color& c)
{
    int x = start.x;
    int x2 = end.x;
    int y = start.y;
    int y2 = end.y;

    int w = x2 - x;
    int h = y2 - y;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
    if (w < 0)
        dx1 = -1;
    else if (w > 0)
        dx1 = 1;
    if (h < 0)
        dy1 = -1;
    else if (h > 0)
        dy1 = 1;
    if (w < 0)
        dx2 = -1;
    else if (w > 0)
        dx2 = 1;
    int longest = abs(w);
    int shortest = abs(h);
    if (!(longest > shortest)) {
        longest = abs(h);
        shortest = abs(w);
        if (h < 0)
            dy2 = -1;
        else if (h > 0)
            dy2 = 1;
        dx2 = 0;
    }
    int numerator = longest >> 1;
    for (int i = 0; i <= longest; i++) {
        SetPixel(x, y, c);
        numerator += shortest;
        if (!(numerator < longest)) {
            numerator -= longest;
            x += dx1;
            y += dy1;
        }
        else {
            x += dx2;
            y += dy2;
        }
    }
}

void Texture::DrawLineH(const int x0, const int x1, const int y0, const Color& c)
{
    int _x0 = glm::min(x0, x1);
    int _x1 = glm::max(x0, x1);
    for (int x = _x0; x <= _x1; ++x) 
    {
        SetPixel(x, y0, c);
    }
}

void Texture::DrawLineV(const int x0, const int y0, const int y1, const Color& c)
{
    int _y0 = glm::min(y0, y1);
    int _y1 = glm::max(y0, y1);
    for (int y = _y0; y <= _y1; ++y)
    {
        SetPixel(x0, y, c);
    }
}

void Texture::DrawCicle(const glm::vec2& center, int radius, const Color& c)
{
    int t1 = radius / 16;
    int t2 = 0;
    int x = radius;
    int y = 0;
    while (x >= y)
    {
        DrawLineH(center.x + x, center.x - x, center.y + y, c);
        DrawLineH(center.x + y, center.x - y, center.y + x, c);
        DrawLineH(center.x + x, center.x - x, center.y - y, c);
        DrawLineH(center.x + y, center.x - y, center.y - x, c);
        y = y + 1;
        t1 = t1 + y;
        t2 = t1 - x;
        if (t2 >= 0)
        {
            t1 = t2;
            x = x - 1;
        }
    }
}

void Texture::DrawHCicle(const glm::vec2& center, int radius, const Color& c)
{
    // https://en.wikipedia.org/wiki/Midpoint_circle_algorithm

    int t1 = radius / 16;
    int t2 = 0;
    int x = radius;
    int y = 0;
    while (x >= y) 
    {
        SetPixel(center.x + x, center.y + y, c);
        SetPixel(center.x - x, center.y + y, c);
        SetPixel(center.x - x, center.y - y, c);
        SetPixel(center.x + x, center.y - y, c);
        SetPixel(center.x + y, center.y + x, c);
        SetPixel(center.x - y, center.y + x, c);
        SetPixel(center.x + y, center.y - x, c);
        SetPixel(center.x - y, center.y - x, c);
        y = y + 1;
        t1 = t1 + y;
        t2 = t1 - x;
        if (t2 >= 0) 
        {
            t1 = t2;
            x = x - 1;
        }
    }
}

void Texture::DrawRect(const glm::vec2& center, const glm::vec2& dim, const Color& c)
{
    glm::vec2 hdim = 0.5f * dim;
    glm::vec2 tl = center - hdim;
    for (int y = 0; y < dim.y; ++y)
    {
        for (int x = 0; x < dim.x; ++x)
        {
            SetPixel(x + tl.x,y + tl.y,c);
        }
    }
    
}

void Texture::DrawHRect(const glm::vec2& center, const glm::vec2& dim, const Color& c)
{
    glm::vec2 hdim = 0.5f * dim;
    glm::vec2 tl = center - hdim;
    glm::vec2 br = center + hdim;

    DrawLineH(tl.x, tl.x + dim.x, tl.y, c);
    DrawLineH(br.x, br.x - dim.x, br.y, c);
    DrawLineV(tl.x, tl.y + dim.y, tl.y, c);
    DrawLineV(br.x, br.y - dim.y, br.y, c);
}

void Texture::SetMinFilter(MinFilter minFilter)
{
    m_min_filter = minFilter;
}

void Texture::SetMagFilter(MagFilter magFilter)
{
    m_mag_filter = magFilter;
}

void Texture::SetWrapS(WrapST wrap)
{
    m_wrap_s = wrap;
}

void Texture::SetWrapT(WrapST wrap)
{
    m_wrap_t = wrap;
}

void Texture::SetWrapST(WrapST wrap)
{
    m_wrap_s = wrap;
    m_wrap_t = wrap;
}

void Texture::SetColorMode(CMode cmode)
{
    m_cmode = cmode;
}


