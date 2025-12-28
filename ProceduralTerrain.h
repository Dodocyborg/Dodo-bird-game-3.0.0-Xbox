#ifndef PROCEDURALTERRAIN_H
#define PROCEDURALTERRAIN_H

#include <iostream>
#include <vector>

class ProceduralTerrain {
public:
    static const int WIDTH = 100;
    static const int HEIGHT = 100;
    float terrainData[WIDTH][HEIGHT];

    float perlinNoise3D(float x, float y, float z);
    void generateTerrain();
};

#endif // PROCEDURALTERRAIN_H
