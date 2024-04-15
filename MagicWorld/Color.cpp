#include "Color.h"

float minf(float a, float b)
{
    return a < b ? a : b;
}

float maxf(float a, float b)
{
    return a > b ? a : b;
}

float hsl_f(float n, float h, float s, float l)
{
    float k = fmod((n + (h / 30.0f)), 12.0f);
    return l - (s * minf(l, 1.0f - l)) * maxf(-1.0f, minf(k - 3.0f, minf(9.0f - k, 1.0f)));
}

Color Color::hsl(float h, float s, float l)
{
    return {
        static_cast<unsigned char>(255.0f * hsl_f(0.0f, h, s, l)),
        static_cast<unsigned char>(255.0f * hsl_f(8.0f, h, s, l)),
        static_cast<unsigned char>(255.0f * hsl_f(4.0f, h, s, l)),
        255
    };
}

Color Color::hsla(float h, float s, float l, float a)
{
    return {
        static_cast<unsigned char>(255.0f * hsl_f(0.0f, h, s, l)),
        static_cast<unsigned char>(255.0f * hsl_f(8.0f, h, s, l)),
        static_cast<unsigned char>(255.0f * hsl_f(4.0f, h, s, l)),
        static_cast<unsigned char>(255.0f * a)
    };
}

Color Color::rgb(unsigned char r, unsigned char g, unsigned char b)
{
    return { r, g, b, 255 };
}

Color Color::rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return { r, g, b, a };
}

Color Color::lerp(const Color a, const Color b, float t)
{
    float _r = float(a.r) + (float(b.r - a.r) * t);
    float _g = float(a.g) + (float(b.g - a.g) * t);
    float _b = float(a.b) + (float(b.b - a.b) * t);
    float _a = float(a.a) + (float(b.a - a.a) * t);

    return 
    { 
        static_cast<unsigned char>(_r), 
        static_cast<unsigned char>(_g), 
        static_cast<unsigned char>(_b), 
        static_cast<unsigned char>(_a) 
    };
}

Color Color::lerp(const std::vector<Color> colors, float t)
{
    float f = float(colors.size()) * t;
    int c0idx = int((colors.size()-1) * t);
    int c1idx = int(ceil((colors.size()-1) * t));
    float fc = f - float(c0idx);
    Color c0 = colors.at(c0idx);
    Color c1 = colors.at(c1idx);
    return lerp(c0, c1, f - float(c0idx));
}
