#pragma once
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>

class PerlinNoise {
public:
    explicit PerlinNoise(unsigned int seed = 0) {
        p.resize(256);
        std::iota(p.begin(), p.end(), 0);
        std::default_random_engine engine(seed);
        std::shuffle(p.begin(), p.end(), engine);
        p.insert(p.end(), p.begin(), p.end());
    }

    // 2D Perlin noise
    double noise(double x, double y) const {
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;
        x -= std::floor(x);
        y -= std::floor(y);
        double u = fade(x);
        double v = fade(y);

        int A = p[X] + Y, AA = p[A], AB = p[A + 1];
        int B = p[X + 1] + Y, BA = p[B], BB = p[B + 1];

        double res = lerp(v,
            lerp(u, grad(p[AA], x, y), grad(p[BA], x - 1, y)),
            lerp(u, grad(p[AB], x, y - 1), grad(p[BB], x - 1, y - 1))
        );
        return (res + 1.0) / 2.0;
    }

private:
    std::vector<int> p;

    static double fade(double t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }
    static double lerp(double t, double a, double b) {
        return a + t * (b - a);
    }
    static double grad(int hash, double x, double y) {
        int h = hash & 7;
        double u = h < 4 ? x : y;
        double v = h < 4 ? y : x;
        return ((h & 1) ? -u : u) + ((h & 2) ? -2.0 * v : 2.0 * v);
    }
};