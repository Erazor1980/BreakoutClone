#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "FPS Counter");
    window.setFramerateLimit(120);

    sf::RectangleShape rec(sf::Vector2f(20.f, 100.f));
    rec.setFillColor(sf::Color::Blue);
    rec.setPosition({ 100.f, 100.f });

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/Arial.ttf"))
    {
        std::cerr << "Could not load font\n";
        return 1;
    }

    sf::Text text(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition({ 20.f, 20.f });

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

        text.setString("fps = " + std::to_string(fps));

        window.clear();
        window.draw(rec);
        window.draw(text);
        window.display();
    }
}