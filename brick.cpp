#include "brick.h"
#include "constants.h"

sf::Texture Brick::m_texture("images/brick.png");

Brick::Brick(float x, float y, int strength) : Entity(m_texture)
{
	m_sprite.setPosition({ x, y });
	m_strength = strength;
}

void Brick::update()
{
	// todo
}

void Brick::damage()
{
    m_strength--;

    if (m_strength <= 0)
    {
        m_bDestroyed = true;
    }
}

void Brick::draw(sf::RenderWindow& wnd)
{
    wnd.draw(m_sprite);

    if (m_strength > 1)
    {
        sf::RectangleShape outline;
        outline.setSize({ static_cast<float>(constants::brick_width), static_cast<float>(constants::brick_height) });
        outline.setPosition(m_sprite.getPosition());
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineThickness(1.0f);

        if (m_strength == 2)
        {
            outline.setOutlineColor(sf::Color(200, 120, 40));
        }
        else if (m_strength >= 3)
        {
            outline.setOutlineColor(sf::Color(140, 40, 40));
        }

        wnd.draw(outline);
    }
}