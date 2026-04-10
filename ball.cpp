#include "ball.h"

sf::Texture Ball::m_texture("images/ball.png");

Ball::Ball(float x, float y, float speed) : Entity(m_texture)
{
	m_speed = speed;

	m_sprite.setPosition({ x, y });
}

void Ball::update()
{
	
}

void Ball::draw(sf::RenderWindow& wnd)
{
	wnd.draw(m_sprite);
}