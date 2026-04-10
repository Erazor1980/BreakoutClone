#include "brick.h"

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

void Brick::draw(sf::RenderWindow& wnd)
{
	wnd.draw(m_sprite);
}