#pragma once
#include <SFML/Graphics.hpp>

// Abstract base class for graphical entities of the game
class Entity
{
protected:
    sf::Sprite m_sprite;

    bool m_bDestroyed{ false };

public:
    Entity(const sf::Texture& texture)
        : m_sprite(texture)
    {      
    }

    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& wnd) = 0;

    sf::FloatRect get_bounding_box() const noexcept;
    bool is_destroyed() const noexcept;

    virtual ~Entity() {}
};