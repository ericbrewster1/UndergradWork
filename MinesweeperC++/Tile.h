#pragma once
#include <iostream>
#include <vector>
#include "TextureManager.h"
using std::string;
using std::vector;

class Tile
{
private:
    sf::Vector2f position;
    vector<Tile*> neighbors;
    sf::Sprite sprite;
    bool clickable;
    bool right_clickable;
public:
    enum class State {HIDDEN, REVEALED, FLAGGED};
    State state;
    State GetState();
    enum class SecretState {EMPTY, MINE, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT};
    SecretState secret_state;
    SecretState GetSecretState();
    void SetSecretState(SecretState secret_state_change);
    Tile(sf::Vector2f _position, const char* texture, SecretState sec_state);
    sf::Sprite& GetSprite();
    Tile* GetNeighbor(unsigned int _neighbor);
    void SetClickable(bool t_or_f);
    void SetRightClickable(bool t_or_f);
    void SetSprite(const char* texture);
    bool Reveal();
    void ToggleFlag(bool debug);
    unsigned int GetNeighborCount();
    void AddNeighbor(Tile* _neighbor);
};