#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "constants.h"
#include "background.h"
#include "ball.h"
#include "brick.h"


void draw_text(const std::string textToDisplay, const sf::Vector2f &pos, const sf::Color &color, const sf::Font &font, sf::RenderWindow &window)
{
    sf::Text text(font);
    text.setString(textToDisplay);
    text.setCharacterSize(24);
    text.setFillColor(color);
    text.setPosition(pos);

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

void handle_collision(const Ball& ball, Brick& brick)
{
    if (is_overlapping(ball, brick))
    {
        brick.damage();
    }
}

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

    Background bg(0.0f, 0.0f);
    Ball ball(constants::window_width / 2.0f, constants::window_height / 2.0f, constants::ball_speed);
    Brick brick(200, 200, 5);
    std::vector<Brick> vBricks;
    create_bricks(vBricks);

    sf::RenderWindow window(sf::VideoMode({ constants::window_width, constants::window_height}), "Breakout Clone");
    window.setFramerateLimit(60);

    bool bRPressedLastFrame = false;
 
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                window.close();
            }
        }

        bool bRCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R);

        if (bRCurrentlyPressed && !bRPressedLastFrame)
        {
            ball.reset(constants::window_width / 2.0f, constants::window_height / 2.0f);
        }

        bRPressedLastFrame = bRCurrentlyPressed;

        

        // updates
        bg.update();
        ball.update();
        for (auto& b : vBricks)
        {
            b.update();
            handle_collision(ball, b);
        }

        vBricks.erase(std::remove_if(std::begin(vBricks), std::end(vBricks), [](const Brick& b) {return b.is_destroyed(); }), std::end(vBricks));

        // displaying
        window.clear();
        bg.draw(window);
        ball.draw(window);
        for (auto& b : vBricks)
        {
            b.draw(window);
        }
        window.display();
    }
}