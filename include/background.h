#pragma once

#include "entity.h"

class Background : public Entity
{
public:
	Background(float x, float y);

	void update() override;
	void draw(sf::RenderWindow& wnd) override;

private:
	static sf::Texture m_texture;
};