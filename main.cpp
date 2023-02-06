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

    // Load textures
    sf::Texture stafftexture;
    stafftexture.loadFromFile("assets/staff.png");
    sf::Texture titleTexture;
    titleTexture.loadFromFile("assets/bg.png");
    sf::Texture texture;
    texture.loadFromFile("assets/monstah.png");

    // Create Sprites

    // cursor texture
    sf::Vector2u sizeS = stafftexture.getSize();
    sf::Sprite staff(stafftexture);
    staff.setOrigin(sizeS.x / 2, sizeS.y / 2);
    staff.setScale(2, 2);
    window.setMouseCursorVisible(false);

    // title
    sf::Sprite title(titleTexture);
    title.setScale(5.5, 5.5);
    title.setPosition(100, 200);

    // four bouncing sprites
    sf::Sprite sprites[4] = {sf::Sprite(texture), sf::Sprite(texture), sf::Sprite(texture), sf::Sprite(texture)};

    sf::Vector2u size = texture.getSize(); // get size of texture for monster

    for (sf::Sprite &sprite : sprites)
    {
        sprite.setScale(0.1f, 0.1f);
        sprite.setOrigin(size.x / 20, size.y / 20);
        sprite.setPosition(rand() % window.getSize().x, rand() % window.getSize().y);
    }

    // vectors to handle bouncing, one for each sprite
    sf::Vector2f increments[4] = {
        sf::Vector2f(0.2f, 0.2f),
        sf::Vector2f(0.2f, 0.2f),
        sf::Vector2f(0.2f, 0.2f),
        sf::Vector2f(0.2f, 0.2f),
    };

    // load font
    sf::Font font;
    if (!font.loadFromFile("assets/pixeloid.ttf"))
        return EXIT_FAILURE;

    int clicks = 0; // points
    bool started = false;
    while (window.isOpen()) // run game loop while the window is open
    {

        // process events from the library -- for our purposes, just window close
        sf::Event event;

        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        // fill screen with yellow color
        window.clear(sf::Color(243, 196, 85, 255));

        if (!started) // if game isn't started, display logo and start screen + listen for clicks
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

        for (int i = 0; i < 4; i++) // run bouncing and click listener for each sprite
        {
            // get respective variables for sprite
            sf::Sprite &sprite = sprites[i];
            sf::Vector2f &increment = increments[i];
            sf::Vector2u windowSize = window.getSize();
            sf::Vector2f spritePosition = sprite.getPosition();
            if ((spritePosition.x + (size.x / 20) >
                     windowSize.x &&
                 increment.x > 0) ||
                (spritePosition.x - (size.x / 20) < 0 &&
                 increment.x < 0))
            {
                // bounce on x axis
                increment.x = -increment.x;
            }
            if ((spritePosition.y + (size.y / 20) >
                     windowSize.y &&
                 increment.y > 0) ||
                (spritePosition.y - (size.y / 20) < 0 &&
                 increment.y < 0))
            { // bounce on y axis
                increment.y = -increment.y;
            }

            sprite.setPosition(sprite.getPosition() + increment); // set position to the current position + the computed directional change
            window.draw(sprite);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isInArea(sf::Mouse::getPosition(window), sprite.getPosition()))
            {
                sprite.setPosition(rand() % window.getSize().x + 100, rand() % window.getSize().y + 100);
                clicks += 1;
                double roundedClicks = clicks / 20;
                increment.y *= min(std::max(roundedClicks, 1.0), 1.04);
                increment.x *= min(std::max(roundedClicks, 1.0), 1.04); // speed up if 
            }
        }
        staff.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
        window.draw(staff);

        // Write the score in the corner
        sf::Text text(to_string((clicks)), font, 64);
        text.setPosition(10, 10);
        window.draw(text);
        
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}
