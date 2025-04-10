#include "Tile.h"

Tile::Tile(sf::Vector2f _position, const char* texture, SecretState sec_state)
{
    sprite = sf::Sprite(TextureManager::GetTexture(texture));
    sprite.setPosition(_position.x, _position.y);
    position = _position;
    state = State::HIDDEN;
    secret_state = sec_state;
    clickable = true;
    right_clickable = true;
}

Tile::State Tile::GetState()
{
    return state;
}

Tile::SecretState Tile::GetSecretState()
{
    return secret_state;
}

void Tile::SetSecretState(SecretState secret_state_change)
{
    secret_state = secret_state_change;
}

sf::Sprite& Tile::GetSprite()
{
    return sprite;
}

void Tile::SetSprite(const char* texture)
{
    sprite.setTexture(TextureManager::GetTexture(texture));
    sprite.setPosition(position);
}

bool Tile::Reveal()
{
    if (clickable)
    {
        if (state == State::HIDDEN)
        {
            state = State::REVEALED;
            if (secret_state == SecretState::MINE)
            {
                SetSprite("files/images/mine.png");
                return true;
            }
            else if (secret_state == SecretState::ONE)
            {
                SetSprite("files/images/number_1.png");
                return false;
            }
            else if (secret_state == SecretState::TWO)
            {
                SetSprite("files/images/number_2.png");
                return false;
            }
            else if (secret_state == SecretState::THREE)
            {
                SetSprite("files/images/number_3.png");
                return false;
            }
            else if (secret_state == SecretState::FOUR)
            {
                SetSprite("files/images/number_4.png");
                return false;
            }
            else if (secret_state == SecretState::FIVE)
            {
                SetSprite("files/images/number_5.png");
                return false;
            }
            else if (secret_state == SecretState::SIX)
            {
                SetSprite("files/images/number_6.png");
                return false;
            }
            else if (secret_state == SecretState::SEVEN)
            {
                SetSprite("files/images/number_7.png");
                return false;
            }
            else if (secret_state == SecretState::EIGHT)
            {
                SetSprite("files/images/number_8.png");
                return false;
            }
            else
            {
                SetSprite("files/images/tile_revealed.png");
                for (unsigned int i = 0; i < GetNeighborCount(); i++)
                    GetNeighbor(i)->Reveal();
                return false;
            }
        }
    }
    return false;
}

void Tile::SetClickable(bool t_or_f)
{
    if (t_or_f == true)
        clickable = true;
    else if (t_or_f == false)
        clickable = false;
}

void Tile::SetRightClickable(bool t_or_f)
{
    if (t_or_f == true)
        right_clickable = true;
    else if (t_or_f == false)
        right_clickable = false;
}

void Tile::ToggleFlag(bool debug)
{
    if (right_clickable)
    {
        if (state == State::HIDDEN)
        {
            clickable = false;
            state = State::FLAGGED;
            SetSprite("files/images/flag.png");
        }
        else if (state == State::FLAGGED && debug && secret_state == SecretState::MINE)
        {
            clickable = true;
            state = State::HIDDEN;
            SetSprite("files/images/mine.png");
        }
        else if (state == State::FLAGGED)
        {
            clickable = true;
            state = State::HIDDEN;
            SetSprite("files/images/tile_hidden.png");
        }
    }
}

unsigned int Tile::GetNeighborCount()
{
    return neighbors.size();
}


void Tile::AddNeighbor(Tile* _neighbor)
{
    neighbors.push_back(_neighbor);
}

Tile* Tile::GetNeighbor(unsigned int _neighbor)
{
    return neighbors.at(_neighbor);
}


