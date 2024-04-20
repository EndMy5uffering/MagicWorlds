#ifndef PerlinNoise_H
#define PerlinNoise_H

#include <cstdlib>
#include <ctime>

namespace Noise 
{
	class Perlin
	{
	private:
        // https://flafla2.github.io/2014/08/09/perlinnoise.html

        // https ://mrl.cs.nyu.edu/~perlin/noise/
        // https ://github.com/nothings/stb/blob/master/stb_perlin.h
        // https ://github.com/stegu/perlin-noise/blob/master/src/noise1234.c

        //Permutation Table for pseudo random number generation
        unsigned char p[512] = {
            151, 160, 137, 91, 90, 15,
                131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
                190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
                88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
                77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
                102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
                135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
                5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
                223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
                129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
                251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
                49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
                138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
                151, 160, 137, 91, 90, 15,
                131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
                190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
                88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
                77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
                102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
                135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
                5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
                223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
                129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
                251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
                49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
                138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
        };

        unsigned int m_seed = 0;
        double m_lacunarity = 2.0;
        double m_persistence = 1.0;
        double m_frequency = 1.0;
        int m_octaves = 3.0;

        double fade(double t) { return t * t * t * (t * (t * 6.0 - 15.0) + 10.0); }

        double lerp(double a, double b, double t) { return a + ((b - a) * t); }

        double grad(int hash, double x, double y)
        {
            switch (hash & 0x7)
            {
            case 0x0: return  x + y;
            case 0x1: return  x;
            case 0x2: return  x - y;
            case 0x3: return -y;
            case 0x4: return -x - y;
            case 0x5: return -x;
            case 0x6: return -x + y;
            case 0x7: return  y;
            default: return 0;
            }
        }

        double grad(int hash, double x, double y, double z)
        {
            switch (hash & 0xF)
            {
            case 0x0: return  x + y;
            case 0x1: return -x + y;
            case 0x2: return  x - y;
            case 0x3: return -x - y;
            case 0x4: return  x + z;
            case 0x5: return -x + z;
            case 0x6: return  x - z;
            case 0x7: return -x - z;
            case 0x8: return  y + z;
            case 0x9: return -y + z;
            case 0xA: return  y - z;
            case 0xB: return -y - z;
            case 0xC: return  y + x;
            case 0xD: return -y + z;
            case 0xE: return  y - x;
            case 0xF: return -y - z;
            default: return 0;
            }
        }

        public:
            Perlin() 
            {
                std::srand(std::time(nullptr));
                SetSeed(std::rand());
            }

            Perlin(unsigned int seed) { SetSeed(seed); }
            Perlin(unsigned int seed, double frequency, double persistence, double lacunarity) 
                :
                m_persistence{ persistence },
                m_frequency{ frequency },
                m_lacunarity{ lacunarity }
            { SetSeed(seed); }

            double at(double x, double y, double z)
            {
                int xi = int(x) % 255;
                int yi = int(y) % 255;
                int zi = int(z) % 255;

                double xf = x - int(x);
                double yf = y - int(y);
                double zf = z - int(z);

                double u = fade(xf);
                double v = fade(yf);
                double w = fade(zf);

                int aaa, aba, aab, abb, baa, bba, bab, bbb;

                aaa = p[p[p[xi] + yi] + zi];
                aba = p[p[p[xi] + yi + 1] + zi];
                aab = p[p[p[xi] + yi] + zi + 1];
                abb = p[p[p[xi] + yi + 1] + zi + 1];
                baa = p[p[p[xi + 1] + yi] + zi];
                bba = p[p[p[xi + 1] + yi + 1] + zi];
                bab = p[p[p[xi + 1] + yi] + zi + 1];
                bbb = p[p[p[xi + 1] + yi + 1] + zi + 1];

                double x1, x2, y1, y2;

                x1 = lerp(grad(aaa, xf, yf, zf), grad(baa, xf - 1, yf, zf), u);
                x2 = lerp(grad(aba, xf, yf - 1, zf), grad(bba, xf - 1, yf - 1, zf), u);
                y1 = lerp(x1, x2, v);
                x1 = lerp(grad(aab, xf, yf, zf - 1), grad(bab, xf - 1, yf, zf - 1), u);
                x2 = lerp(grad(abb, xf, yf - 1, zf - 1), grad(bbb, xf - 1, yf - 1, zf - 1), u);
                y2 = lerp(x1, x2, v);

                return (lerp(y1, y2, w) + 1) / 2.0;
            }

            double at(double x, double y)
            {
                int xi = int(x) % 255;
                int yi = int(y) % 255;

                double xf = x - int(x);
                double yf = y - int(y);

                double u = fade(xf);
                double v = fade(yf);

                int h00 = p[p[xi + 0] + yi + 0];
                int h01 = p[p[xi + 0] + yi + 1];
                int h10 = p[p[xi + 1] + yi + 0];
                int h11 = p[p[xi + 1] + yi + 1];

                double x1 = lerp(grad(h00, xf, yf), grad(h10, xf - 1, yf), u);
                double x2 = lerp(grad(h01, xf, yf - 1), grad(h11, xf - 1, yf - 1), u);

                return lerp(x1, x2, v);
            }

            double at3D_octives(double x, double y, double z) 
            {
                double total = 0.0;
                double frequency = m_frequency;
                double amplitude = 1.0;
                double maxValue = 0.0;
                for (int i = 0; i < m_octaves; ++i) 
                {
                    total += at(x * frequency, y * frequency, z * frequency) * amplitude;

                    maxValue += amplitude;
                    amplitude *= m_persistence;
                    frequency *= m_lacunarity;
                }
                return total / maxValue;
            }

            double at2D_octives(double x, double y)
            {
                double total = 0.0;
                double frequency = m_frequency;
                double amplitude = 1.0;
                double maxValue = 0.0;
                for (int i = 0; i < m_octaves; ++i)
                {
                    total += at(x * frequency, y * frequency) * amplitude;

                    maxValue += amplitude;
                    amplitude *= m_persistence;
                    frequency *= m_lacunarity;
                }
                return total / maxValue;
            }

            unsigned int GetSeed() { return m_seed; }

            void SetSeed(unsigned int seed) 
            {
                m_seed = seed;
                std::srand(m_seed);
                for (int i = 0; i < 512; ++i)
                {
                    p[i] = std::rand() % 256;
                }
            }

            void SetPersistence(double persistence) { this->m_persistence = persistence; }

            void SetLacunarity(double lacunarity) { this->m_lacunarity = lacunarity; }

            void SetFrequency(double frequency) { this->m_frequency = frequency; }

            void SetOctaves(int octaves) { this->m_octaves = octaves; }
	};

}


#endif //PerlinNoise_H