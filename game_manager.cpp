#include <cstdlib>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

#include "game_manager.h"
#include "constants.h"
#include "interactions.h"



GameManager::GameManager()
	: m_background(0.0f, 0.0f),
	  m_paddle(constants::window_width / 2.0f, constants::window_height - int(1.3 * constants::paddle_height), constants::paddle_speed)
{
	// Seed the random number generator once to get different results each run
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Create game window
	m_window = sf::RenderWindow(sf::VideoMode({ constants::window_width, constants::window_height }), "Breakout Clone");
	m_window.setFramerateLimit(60);

    // Load font file
    if (!m_font.openFromFile("arial.ttf"))
    {
        std::cout << "Could not load font\n";
    }

    reset();
}

void GameManager::run()
{
    bool bRPressedLastFrame = false;
    bool bPPressedLastFrame = false;

    while (m_window.isOpen())
    {
        while (const std::optional event = m_window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                m_window.close();
            }
        }

        // Reset game
        bool bRCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R);
        if (bRCurrentlyPressed && !bRPressedLastFrame)
        {
            reset();
        }
        bRPressedLastFrame = bRCurrentlyPressed;

        // Pause game
        bool bPCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P);
        if (bPCurrentlyPressed && !bPPressedLastFrame)
        {
            if (m_gameState == game_state::running)
            {
                m_gameState =game_state::paused;
            }
            else if (m_gameState == game_state::paused)
            {
                m_gameState = game_state::running;
            }
        }
        bPPressedLastFrame = bPCurrentlyPressed;

        if (m_gameState == game_state::running)
        {
            update();
        }
        if (m_gameState == game_state::waiting)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                m_gameState = game_state::running;
            }
        }
        display();
    }
}

void GameManager::update()
{
	m_background.update();
    
    // Update all bricks once per frame.
    // Bricks do not depend on a specific ball, so this stays separate.
    for (auto& brick : m_vBricks)
    {
        brick.update();
    }

    // Update the paddle once per frame.
    m_paddle.update();

    // Process each ball independently.
    // This is important because each ball can hit a different set of bricks
    // in the same frame.
    for (auto& ball : m_vBalls)
    {
        // First move the ball and update its wall collision logic.
        ball.update();

        // These flags collect the final bounce decision for this frame.
        // We do not bounce immediately for each brick hit.
        // Instead, we gather all brick collisions first and then bounce once.
        bool bBounceX = false;
        bool bBounceY = false;

        // Check the ball against all bricks.
        for (auto& brick : m_vBricks)
        {
            const BrickCollisionResult collisionResult = handle_collision(ball, brick);

            // If this brick was hit from the side, remember that X must flip.
            if (collisionResult.m_bBounceX)
            {
                bBounceX = true;
            }

            // If this brick was hit from above or below, remember that Y must flip.
            if (collisionResult.m_bBounceY)
            {
                bBounceY = true;
            }
        }

        // Apply the collected bounce exactly once per axis.
        // This prevents double bouncing on the same axis in one frame.
        if (bBounceX)
        {
            ball.bounceHorizontal();
        }

        if (bBounceY)
        {
            ball.bounceVertical();
        }

        // Paddle collision is still handled after brick collisions.
        // That can stay as it is for now.
        handle_collision(ball, m_paddle);
    }


	m_vBalls.erase(std::remove_if(std::begin(m_vBalls), std::end(m_vBalls),
		[](const Ball& b) {return b.is_destroyed(); }), std::end(m_vBalls));
	m_vBricks.erase(std::remove_if(std::begin(m_vBricks), std::end(m_vBricks),
		[](const Brick& b) {return b.is_destroyed(); }), std::end(m_vBricks));

	if (m_vBalls.empty())
	{
		m_lives--;
		if (m_lives <= 0)
		{
			m_gameState = game_state::game_over;
		}
		else
		{
			m_vBalls.emplace_back(Ball(constants::window_width / 2.0f, constants::window_height / 2.0f, constants::ball_speed));
            m_paddle.reset(constants::window_width / 2.0f, constants::window_height - int(1.3 * constants::paddle_height));
            m_gameState = game_state::waiting;
		}
	}
}

void GameManager::display()
{
    m_window.clear();
    m_background.draw(m_window);
    for (auto& ball : m_vBalls)
    {
        ball.draw(m_window);
    }
    for (auto& b : m_vBricks)
    {
        b.draw(m_window);
    }
    m_paddle.draw(m_window);


    draw_text(std::string("Lives: ") + std::to_string(m_lives), { constants::window_width - 60, 20 }, sf::Color::Cyan, 15);

    if (m_gameState == game_state::game_over)
    {
        draw_text("Game Over", { constants::window_width / 2 - 120, constants::window_height / 2 - 80 }, sf::Color::Red, 45);
    }
    else if (m_gameState == game_state::paused)
    {
        draw_text("Game Paused", { constants::window_width / 2 - 120, constants::window_height / 2 - 80 }, sf::Color::Green, 45);
    }
    m_window.display();
}

void GameManager::reset()
{
    createBricks();
    m_vBalls.clear();
    m_vBalls.emplace_back(Ball(constants::window_width / 2.0f, constants::window_height / 2.0f, constants::ball_speed));
    m_paddle.reset(constants::window_width / 2.0f, constants::window_height - int(1.3 * constants::paddle_height));

    m_lives = 3;
    m_gameState = game_state::waiting;
}

void GameManager::draw_text(const std::string textToDisplay, const sf::Vector2f& pos, const sf::Color& color, int size)
{
    sf::Text text(m_font);
    text.setString(textToDisplay);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(pos);
    text.setOutlineThickness(3);

    m_window.draw(text);
}

void GameManager::createBricks()
{
    // we start brick width away from the sides, and 3* brick height away from the top

    // clear existing bricks
    m_vBricks.clear();

    // reserve memory (performance, avoids reallocations)
    m_vBricks.reserve(constants::brick_cols * constants::brick_rows);

    const int spacing = 5;

    // start offsets
    const float totalWidth = constants::brick_cols * constants::brick_width + (constants::brick_cols - 1) * spacing;

    const float startX = (constants::window_width - totalWidth) * 0.5f;
    const float startY = static_cast<float>(constants::brick_height * 2);

    for (int row = 0; row < constants::brick_rows; ++row)
    {
        for (int col = 0; col < constants::brick_cols; ++col)
        {
            float x = startX + col * (constants::brick_width + spacing);
            float y = startY + row * (constants::brick_height + spacing);

            Brick brick(x, y, rand() % 4);

            m_vBricks.push_back(brick);
        }
    }
}


