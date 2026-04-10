#include "ball.h"
#include "constants.h"

sf::Texture Ball::m_texture("images/ball.png");

Ball::Ball(float x, float y, float speed) : Entity(m_texture)
{
	m_speed = speed;
	m_sprite.setOrigin(m_sprite.getLocalBounds().getCenter());

	reset(x, y);
}

void Ball::update()
{
	// check for wall bounces
	const auto bounds = m_sprite.getGlobalBounds();
	if (bounds.position.x <= 0 || bounds.position.x + bounds.size.x >= constants::window_width)
	{
		m_velocity.x = -m_velocity.x;
	}

	if (bounds.position.y <= 0 || bounds.position.y + bounds.size.y >= constants::window_height)
	{
		m_velocity.y = -m_velocity.y;
	}

	// move ball to new position
	m_sprite.move(m_velocity);

	//todo
	// maybe make sure, that after moving the ball is not partially outside of the screen! which can happen right now, if it is too fast
	
}

void Ball::reset(float x, float y)
{
	m_sprite.setPosition({ x, y });

	// random angle between 45 and 135° (90° would be up) for the ball moving direction
	int angleDeg = 45 + (std::rand() % 91);
	float angleRad = angleDeg * 3.14159265f / 180.0f;

	m_velocity.x = -std::cos(angleRad);
	m_velocity.y = -std::sin(angleRad);
	m_velocity = m_velocity.normalized() * m_speed;
}

void Ball::draw(sf::RenderWindow& wnd)
{
	wnd.draw(m_sprite);
}