#pragma once
#include "background.h"
#include "ball.h"
#include "brick.h"
#include "paddle.h"


class GameManager
{
public:
	GameManager();

	void run();

	enum class game_state { paused, running, game_over };

private:
	void createBricks();
	void update();
	void display();
	void reset();
	void draw_text(const std::string textToDisplay, const sf::Vector2f& pos, const sf::Color& color, int size);

	sf::RenderWindow m_window;

	Background m_background;
	std::vector<Ball> m_vBalls;
	std::vector<Brick> m_vBricks;
	Paddle m_paddle;

	int m_lives{ 3 };

	game_state m_gameState{ game_state::running };
	sf::Font m_font;
};