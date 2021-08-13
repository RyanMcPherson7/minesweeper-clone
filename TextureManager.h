#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;

class TextureManager {

    static unordered_map<string, sf::Texture> textures;
    static void LoadTexture(const string& textureName);
public:
    static sf::Texture& GetTexture(const string& textureName);
    static void Clear();
};