#include <algorithm>

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
	m_previousPosition = m_sprite.getPosition();

	// move ball to new position
	m_sprite.move(m_velocity);

	// check for wall bounces
	const auto bounds = m_sprite.getGlobalBounds();
	const float halflSize = bounds.size.x * 0.5f;

	// X axis
	if (bounds.position.x <= 0)
	{
		m_velocity.x = -m_velocity.x;
		
		m_sprite.setPosition({ halflSize, m_sprite.getPosition().y });
	}
	else if (bounds.position.x + bounds.size.x >= constants::window_width)
	{
		m_velocity.x = -m_velocity.x;

		m_sprite.setPosition({ constants::window_width - halflSize, m_sprite.getPosition().y });
	}

	// Y axis
	if (bounds.position.y <= 0)
	{
		m_velocity.y = -m_velocity.y;

		m_sprite.setPosition({ m_sprite.getPosition().x, halflSize });
	}
	else if (bounds.position.y + bounds.size.y >= constants::window_height)
	{
		//m_velocity.y = -m_velocity.y;

		//m_sprite.setPosition({ m_sprite.getPosition().x, constants::window_height - halflSize });
		m_bDestroyed = true;
	}
}

void Ball::reset(float x, float y)
{
	m_previousPosition = { x, y };
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

void Ball::bounceVertical()
{
	m_velocity.y = -m_velocity.y;
}
void Ball::bounceHorizontal()
{
	m_velocity.x = -m_velocity.x;
}

sf::Vector2f Ball::getPreviousPosition() const noexcept
{
	return m_previousPosition;
}

void Ball::bounceFromPaddle(float relativeHitX)
{
	relativeHitX = std::clamp(relativeHitX, -1.0f, 1.0f);

	const float maxAngleDeg = 55.0f;
	const float angleDeg = 90.0f + relativeHitX * maxAngleDeg;
	const float angleRad = angleDeg * 3.14159265f / 180.0f;

	m_velocity.x = -std::cos(angleRad);
	m_velocity.y = -std::sin(angleRad);
	m_velocity = m_velocity.normalized() * m_speed;
}