#include <iostream>
#include <string>

#include "background.h"


void draw_text(const std::string textToDisplay, const sf::Vector2f &pos, const sf::Color &color, const sf::Font &font, sf::RenderWindow &window)
{
    sf::Text text(font);
    text.setString(textToDisplay);
    text.setCharacterSize(24);
    text.setFillColor(color);
    text.setPosition(pos);

    window.draw(text);
}

int main()
{
    sf::Font font;
    if (!font.openFromFile("arial.ttf"))
    {
        std::cerr << "Could not load font\n";
        return 1;
    }

    Background bg(0.0f, 0.0f);

    sf::RenderWindow window(sf::VideoMode({ constants::window_width, constants::window_height}), "Breakout Clone");
    window.setFramerateLimit(60);

 
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                window.close();
            }
        }

        // updates
        bg.update();

        // displaying
        window.clear();
        bg.draw(window);
        window.display();
    }
}