#include "ball.h"
#include "constants.h"

sf::Texture Ball::m_texture("images/ball.png");

Ball::Ball(float x, float y, float speed) : Entity(m_texture)
{
	m_speed = speed;

	m_sprite.setPosition({ x, y });

	m_velocity.y = -m_speed;
	m_velocity.x = (std::rand() % 2 == 0) ? m_speed : -m_speed;
}

void Ball::update()
{
	// check for wall bounces
	const auto bounds = m_sprite.getGlobalBounds();
	if (bounds.position.x <= 0)
	{
		m_velocity.x = m_speed;
	}
	else if (bounds.position.x + bounds.size.x >= constants::window_width)
	{
		m_velocity.x = -m_speed;
	}

	if (bounds.position.y <= 0)
	{
		m_velocity.y = m_speed;
	}
	else if (bounds.position.y + bounds.size.y >= constants::window_height)
	{
		m_velocity.y = -m_speed;
	}

	// move ball to new position
	m_sprite.move(m_velocity);
}

void Ball::draw(sf::RenderWindow& wnd)
{
	wnd.draw(m_sprite);
}