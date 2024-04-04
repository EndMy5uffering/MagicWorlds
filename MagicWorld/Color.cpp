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