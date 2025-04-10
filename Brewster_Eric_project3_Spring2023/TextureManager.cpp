#include "TextureManager.h"

unordered_map<string, sf::Texture> TextureManager::textureList;

void TextureManager::LoadTexture(const char* fileName)
{
    string filePath;
    filePath += fileName;
    textureList[fileName].loadFromFile(filePath);
}

sf::Texture& TextureManager::GetTexture(const char* texture)
{
    if (textureList.find(texture) == textureList.end())
    {
        LoadTexture(texture);
    }
    return textureList[texture];
}

void TextureManager::Clear()
{
    textureList.clear();
}
