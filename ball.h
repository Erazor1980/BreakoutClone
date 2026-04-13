#pragma once

#include "entity.h"

class Ball : public Entity
{
public:
	Ball(float x, float y, float speed);

	void update() override;
	void draw(sf::RenderWindow& wnd) override;

	void reset(float x, float y);

	void bounceVertical();
	void bounceHorizontal();
	void bounceFromPaddle(float relativeHitX);
private:
	static sf::Texture m_texture;

	float m_speed;
	sf::Vector2f m_velocity;	
};