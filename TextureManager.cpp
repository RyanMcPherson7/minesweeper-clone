#include "TextureManager.h"

unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(const string& fileName) {

    string path = "images/" + fileName + ".png";

    //assuming all fileNames are valid files
    textures[fileName].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(const string& textureName) {

    //if the texture does not already exist, create it
    if (textures.find(textureName) == textures.end())
        LoadTexture(textureName);

    return textures[textureName];
}

void TextureManager::Clear() { textures.clear(); }