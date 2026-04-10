#include "background.h"

sf::Texture Background::m_texture("images/background.jpg");

Background::Background(float x, float y) : Entity(m_texture)
{
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition({ x, y });
}

void Background::update()
{
	// empty, since no updates in this class
}

void Background::draw(sf::RenderWindow& wnd)
{
	wnd.draw(m_sprite);
}