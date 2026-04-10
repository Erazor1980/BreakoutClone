#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "constants.h"

void draw_text(const std::string textToDisplay, const sf::Vector2f &pos, const sf::Color &color, sf::RenderWindow &window)
{
    sf::Font font;
    if (!font.openFromFile("arial.ttf"))
    {
        std::cerr << "Could not load font\n";
        return;
    }

    sf::Text text(font);
    text.setString(textToDisplay);
    text.setCharacterSize(24);
    text.setFillColor(color);
    text.setPosition(pos);

    window.draw(text);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ constants::window_width, constants::window_height}), "Breakout Clone");
    window.setFramerateLimit(60);

    sf::RectangleShape rec(sf::Vector2f(20.f, 100.f));
    rec.setFillColor(sf::Color::Blue);
    rec.setPosition({ 100.f, 100.f });

    

    std::uint64_t counter = 0;
    sf::Clock totalClock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        ++counter;

        float seconds = totalClock.getElapsedTime().asSeconds();
        float fps = 0.f;

        if (seconds > 0.f)
        {
            fps = static_cast<float>(counter) / seconds;
        }


        window.clear();
        window.draw(rec);
        draw_text("fps = " + std::to_string(fps), { 20.0f, 20.0f }, sf::Color::White, window);
        window.display();
    }
}