#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "constants.h"
#include "background.h"
#include "ball.h"
#include "brick.h"
#include "paddle.h"


void draw_text(const std::string textToDisplay, const sf::Vector2f &pos, const sf::Color &color, const sf::Font &font, int size, sf::RenderWindow &window)
{
    sf::Text text(font);
    text.setString(textToDisplay);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(pos);
    text.setOutlineThickness(3);

    window.draw(text);
}

void create_bricks(std::vector<Brick>& vBricks)
{
    // we start brick width away from the sides, and 3* brick height away from the top

    // clear existing bricks
    vBricks.clear();

    // reserve memory (performance, avoids reallocations)
    vBricks.reserve(constants::brick_cols * constants::brick_rows);

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

            vBricks.push_back(brick);
        }
    }
}

bool is_overlapping(const Entity& e1, const Entity& e2)
{
    const auto bb_e1 = e1.get_bounding_box();
    const auto bb_e2 = e2.get_bounding_box();

    return bb_e1.findIntersection(bb_e2).has_value();
}

void handle_collision(Ball& ball, Brick& brick)
{
    if (!is_overlapping(ball, brick))
    {
        return;
    }

    brick.damage();

    const auto ballBB = ball.get_bounding_box();
    const auto brickBB = brick.get_bounding_box();

    const float overlapLeft = (ballBB.position.x + ballBB.size.x) - brickBB.position.x;
    const float overlapRight = (brickBB.position.x + brickBB.size.x) - ballBB.position.x;
    const float overlapTop = (ballBB.position.y + ballBB.size.y) - brickBB.position.y;
    const float overlapBottom = (brickBB.position.y + brickBB.size.y) - ballBB.position.y;

    const float minOverlapX = std::min(overlapLeft, overlapRight);
    const float minOverlapY = std::min(overlapTop, overlapBottom);

    if (minOverlapX < minOverlapY)
    {
        ball.bounceHorizontal();
    }
    else
    {
        ball.bounceVertical();
    }
}

void handle_collision(Ball& ball, Paddle& paddle)
{
    if (!is_overlapping(ball, paddle))
    {
        return;
    }

    const auto ballBB = ball.get_bounding_box();
    const auto paddleBB = paddle.get_bounding_box();

    const float overlapLeft = (ballBB.position.x + ballBB.size.x) - paddleBB.position.x;
    const float overlapRight = (paddleBB.position.x + paddleBB.size.x) - ballBB.position.x;
    const float overlapTop = (ballBB.position.y + ballBB.size.y) - paddleBB.position.y;
    const float overlapBottom = (paddleBB.position.y + paddleBB.size.y) - ballBB.position.y;

    const float minOverlapX = std::min(overlapLeft, overlapRight);
    const float minOverlapY = std::min(overlapTop, overlapBottom);

    if (minOverlapX < minOverlapY)
    {
        ball.bounceHorizontal();
        return;
    }

    if (ballBB.getCenter().y > paddleBB.position.y) // we are not bouncing up, if the ball is too low
    {
        return;
    }

    const float ballCenterX = ballBB.position.x + ballBB.size.x * 0.5f;
    const float paddleCenterX = paddleBB.position.x + paddleBB.size.x * 0.5f;
    const float deltaX = ballCenterX - paddleCenterX;

    if (std::abs(deltaX) <= 5.0f)   // bounce up if ball hits paddle center +/- 5 pixels
    {
        ball.bounceFromPaddle(0.0f);
        return;
    }

    const float halfPaddleWidth = paddleBB.size.x * 0.5f;
    const float relativeHitX = deltaX / halfPaddleWidth;

    ball.bounceFromPaddle(relativeHitX);
}

enum class game_state { paused, running, game_over };

int main()
{
    // Seed the random number generator once to get different results each run
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::Font font;
    if (!font.openFromFile("arial.ttf"))
    {
        std::cerr << "Could not load font\n";
        return 1;
    }

    int lives = 3;
    game_state gs = game_state::running;

    Background bg(0.0f, 0.0f);
    std::vector<Ball> vBalls;
    vBalls.emplace_back(Ball(constants::window_width / 2.0f, constants::window_height / 2.0f, constants::ball_speed));
    vBalls.emplace_back(Ball(constants::window_width / 2.0f, constants::window_height / 2.0f, constants::ball_speed));
    //Ball ball(constants::window_width / 2.0f, constants::window_height / 2.0f, constants::ball_speed);
    Brick brick(200, 200, 5);
    std::vector<Brick> vBricks;
    create_bricks(vBricks);
    Paddle paddle(constants::window_width / 2.0f, constants::window_height - int(1.3 * constants::paddle_height), constants::paddle_speed);

    sf::RenderWindow window(sf::VideoMode({ constants::window_width, constants::window_height}), "Breakout Clone");
    window.setFramerateLimit(60);

    bool bRPressedLastFrame = false;
    bool bPPressedLastFrame = false;
 
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                window.close();
            }
        }

        // Reset game
        bool bRCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R);
        if (bRCurrentlyPressed && !bRPressedLastFrame)
        {
            vBalls.clear();
            //ball.reset(constants::window_width / 2.0f, constants::window_height / 2.0f);
            vBalls.emplace_back(Ball(constants::window_width / 2.0f, constants::window_height / 2.0f, constants::ball_speed));
            vBalls.emplace_back(Ball(constants::window_width / 2.0f, constants::window_height / 2.0f, constants::ball_speed));
            paddle.reset(constants::window_width / 2.0f, constants::window_height - int(1.3 * constants::paddle_height));
            vBricks.clear();
            create_bricks(vBricks);
            gs = game_state::running;
            lives = 3;
        }
        bRPressedLastFrame = bRCurrentlyPressed;

        // Pause game
        bool bPCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P);
        if (bPCurrentlyPressed && !bPPressedLastFrame)
        {
            if (gs == game_state::running)
            {
                gs = game_state::paused;
            }
            else if (gs == game_state::paused)
            {
                gs = game_state::running;
            }
        }
        bPPressedLastFrame = bPCurrentlyPressed;

        if (gs == game_state::running)
        {        
            // updates
            bg.update();
            for (auto& ball : vBalls)
            {
                ball.update();
            }
            for (auto& brick : vBricks)
            {
                brick.update();
                for (auto& ball : vBalls)
                {
                    handle_collision(ball, brick);
                }
            }
            paddle.update();
            for (auto& ball : vBalls)
            {
                handle_collision(ball, paddle);
            }

            vBalls.erase(std::remove_if(std::begin(vBalls), std::end(vBalls), [](const Ball& b) {return b.is_destroyed(); }), std::end(vBalls));
            vBricks.erase(std::remove_if(std::begin(vBricks), std::end(vBricks), [](const Brick& b) {return b.is_destroyed(); }), std::end(vBricks));

            if (vBalls.empty())
            {
                lives--;
                if (lives <= 0)
                {
                    gs = game_state::game_over;
                }
                else
                {
                    vBalls.emplace_back(Ball(constants::window_width / 2.0f, constants::window_height / 2.0f, constants::ball_speed));
                }
            }            
        }

        // displaying
        window.clear();
        bg.draw(window);
        for (auto& ball : vBalls)
        {
            ball.draw(window);
        }
        for (auto& b : vBricks)
        {
            b.draw(window);
        }
        paddle.draw(window);
               

        draw_text(std::string("Lives: ") + std::to_string(lives), { constants::window_width - 60, 20 }, sf::Color::Cyan, font, 15, window);

        if (gs == game_state::game_over)
        {
            draw_text("Game Over", { constants::window_width / 2 - 120, constants::window_height / 2 - 80 }, sf::Color::Red, font, 45, window);
        }
        else if (gs == game_state::paused)
        {
            draw_text("Game Paused", { constants::window_width / 2 - 120, constants::window_height / 2 - 80 }, sf::Color::Green, font, 45, window);
        }
        window.display();
    }
}