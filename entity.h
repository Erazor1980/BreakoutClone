#pragma once

#include <SFML/Graphics.hpp>

// Abstract base class for graphical entities of the game
class Entity
{
protected:
    sf::Sprite m_sprite;

public:
    Entity(const sf::Texture& texture)
        : m_sprite(texture)
    {
        m_sprite.setOrigin(m_sprite.getLocalBounds().getCenter());
    }

    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& wnd) = 0;

    virtual ~Entity() {}
};