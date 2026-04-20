#include "paddle.h"
#include "constants.h"

sf::Texture Paddle::m_texture("images/paddle.jpg");

Paddle::Paddle(float x, float y, float speed) : Entity(m_texture)
{
	m_speed = speed;
	m_velocity = { 0,0 };
	m_sprite.setOrigin(m_sprite.getLocalBounds().getCenter());

	reset(x, y);
}

void Paddle::update()
{
	processKeyInput();
	m_sprite.move(m_velocity);
}

void Paddle::draw(sf::RenderWindow& wnd)
{
	wnd.draw(m_sprite);
}

void Paddle::reset(float x, float y)
{
	m_sprite.setPosition({ x, y });
}

void Paddle::processKeyInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) 
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		if (m_sprite.getPosition().x > 0)
		{
			m_velocity.x = -m_speed;
		}
		else
		{
			m_velocity.x = 0;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		if (m_sprite.getPosition().x < constants::window_width)
		{
			m_velocity.x = m_speed;
		}
		else
		{
			m_velocity.x = 0;
		}
	}
	else
	{
		m_velocity.x = 0;
	}
}