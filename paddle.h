#pragma once
#include "entity.h"

class Paddle : public Entity
{
public:
	Paddle(float x, float y, float speed);

	void update() override;
	void draw(sf::RenderWindow& wnd) override;

	void reset(float x, float y);

private:
	void processKeyInput();

	static sf::Texture m_texture;

	float m_speed;
	sf::Vector2f m_velocity;
};