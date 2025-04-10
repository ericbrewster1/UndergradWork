#pragma once
#include <random>

class Random
{
public:
    static int mineSpots(int min, int max);
private:
    static std::mt19937 random;
};
