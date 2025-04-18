#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;


class TextureManager
{
    static unordered_map<string, sf::Texture> textureList;
    static void LoadTexture(const char* fileName);
public:
    static sf::Texture& GetTexture(const char* texture);
    static void Clear();
};