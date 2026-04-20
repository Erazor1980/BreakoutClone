#pragma once

#include "entity.h"

class Brick : public Entity
{
public:
	Brick(float x, float y, int strength);

	void update() override;
	void draw(sf::RenderWindow& wnd) override;

	void damage();

private:
	static sf::Texture m_texture;
	int m_strength;
};