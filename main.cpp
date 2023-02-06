// #include "Audio.hpp"
#include <SFML/Graphics.hpp>

#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;
bool isInArea(sf::Vector2i mousePos, sf::Vector2f otherPos)
{
    return (abs(mousePos.x - otherPos.x) < 80) && (abs(mousePos.y - otherPos.y) < 110);
}
bool comp(int a, int b)
{
    return (a < b);
}

int main()
{
    srand(time(NULL));
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1600, 1200), "Graham's Super Cool Game");
    // Load a sprite to display
    sf::Texture stafftexture;
    stafftexture.loadFromFile("staff.png");
    sf::Vector2u sizeS = stafftexture.getSize();
    sf::Sprite staff(stafftexture);
    staff.setOrigin(sizeS.x / 2, sizeS.y / 2);
    staff.setScale(2, 2);
    window.setMouseCursorVisible(false);
    sf::Texture texture;
    if (!texture.loadFromFile("monstah.png"))
        return EXIT_FAILURE;
    sf::Sprite sprites[4] = {sf::Sprite(texture), sf::Sprite(texture), sf::Sprite(texture), sf::Sprite(texture)};

    sf::Texture titleTexture;
    titleTexture.loadFromFile("bg.png");
    sf::Sprite title(titleTexture);
    title.setScale(5.5, 5.5);
    title.setPosition(100, 200);

    // Create a graphical text to display
    sf::Vector2u size = texture.getSize();
    for (sf::Sprite &sprite : sprites)
    {
        sprite.setScale(0.1f, 0.1f);
        sprite.setOrigin(size.x / 20, size.y / 20);
        sprite.setPosition(rand() % 1600, rand() % 1200);
    }
    sf::Vector2f increments[4] = {
        sf::Vector2f(0.2f, 0.2f),
        sf::Vector2f(0.2f, 0.2f),
        sf::Vector2f(0.2f, 0.2f),
        sf::Vector2f(0.2f, 0.2f),
    };
    sf::Font font;
    if (!font.loadFromFile("pixeloid.ttf"))
        return EXIT_FAILURE;

    int clicks = 0;
    bool started = false;
    while (window.isOpen())
    {

        // Process events
        sf::Event event;

        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear(sf::Color(243, 196, 85, 255));
        // Draw the sprite
        if (!started)
        {
            sf::Text text("Click to start", font, 128);
            text.setPosition(250, 600);
            window.draw(text);
            window.draw(title);
            window.display();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                started = true;
            }
            continue;
        }
        for (int i = 0; i < 4; i++)
        {
            sf::Sprite &sprite = sprites[i];

            sf::Vector2f &increment = increments[i];

            if ((sprite.getPosition().x + (size.x / 20) >
                     window.getSize().x &&
                 increment.x > 0) ||
                (sprite.getPosition().x - (size.x / 20) < 0 &&
                 increment.x < 0))
            {
                // Reverse the direction on X axis.
                increment.x = -increment.x;
            }
            if ((sprite.getPosition().y + (size.y / 20) >
                     window.getSize().y &&
                 increment.y > 0) ||
                (sprite.getPosition().y - (size.y / 20) < 0 &&
                 increment.y < 0))
            { // Reverse the direction on Y axis.
                increment.y = -increment.y;
            }

            sprite.setPosition(sprite.getPosition() + increment);
            window.draw(sprite);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isInArea(sf::Mouse::getPosition(window), sprite.getPosition()))
            {
                sprite.setPosition(rand() % 1700, rand() % 1300);
                clicks += 1;
                double roundedClicks = clicks / 20;
                increment.y *= min(std::max(roundedClicks, 1.0), 1.04);
                increment.x *= min(std::max(roundedClicks, 1.0), 1.04);
            }
        }
        staff.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
        window.draw(staff);
        // Draw the string
        sf::Text text(to_string((clicks)), font, 64);
        text.setPosition(10, 10);
        window.draw(text);
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}
