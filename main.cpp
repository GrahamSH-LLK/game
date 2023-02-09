#include <SFML/Graphics.hpp>

#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;
bool isInArea(sf::Vector2i mousePos, sf::Vector2f otherPos) {
    return (abs(mousePos.x - otherPos.x) < 80) && (abs(mousePos.y - otherPos.y) < 110);
}
bool comp(int a, int b)
{
    return (a < b);
}
int level = 0;
int ticks = 0;
int main()
{
    srand((unsigned)time(NULL));
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1600, 1200), "Graham's Super Cool Game");
    window.setFramerateLimit(60);
    // Load textures
    sf::Texture stafftexture;
    stafftexture.loadFromFile("assets/staff.png");

    sf::Texture titleTexture;
    titleTexture.loadFromFile("assets/bg.png");

    sf::Texture texture;
    texture.loadFromFile("assets/monstah.png");

    sf::Texture desertTexture;
    desertTexture.loadFromFile("assets/desert.jpeg");


    sf::Texture oceanTexture;
    oceanTexture.loadFromFile("assets/ocean.jpg");
    sf::Vector2u sizeOcean = oceanTexture.getSize();
    sf::Vector2u sizeDesert = desertTexture.getSize();

    // Create Sprites
    // first level background
    sf::Sprite desertBg(desertTexture);
    desertBg.setOrigin(sizeDesert.x / 2, sizeDesert.y/2);
        sf::Sprite oceanBg(oceanTexture);
    oceanBg.setOrigin(sizeOcean.x/2, sizeOcean.y/2);
    oceanBg.setScale(2,2);
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
    sf::Sprite sprites[3] = {sf::Sprite(texture), sf::Sprite(texture), sf::Sprite(texture)};

    sf::Vector2u size = texture.getSize(); // get size of texture for monster

    for (sf::Sprite &sprite : sprites)
    {
        sprite.setScale(0.1f, 0.1f);
        sprite.setOrigin(size.x / 20, size.y / 20);
        sprite.setPosition(rand() % window.getSize().x, rand() % window.getSize().y);
    }

    // vectors to handle bouncing, one for each sprite
    sf::Vector2f increments[3] = {
        sf::Vector2f(5.0f, 5.0f),
        sf::Vector2f(5.0f, 5.0f),
        sf::Vector2f(5.0f, 5.0f)
        
    };

    // load font
    sf::Font font;
    if (!font.loadFromFile("assets/pixeloid.ttf"))
        return EXIT_FAILURE;

    int clicks = 0; // points
    int level = 0;
    sf::Sprite levels[2] = {desertBg, oceanBg};
    sf::Clock clock;
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
        window.clear(sf::Color(233, 196, 85, 255));

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
        window.draw(levels[level]);
        levels[level].setPosition(window.getSize().x /2, window.getSize().y / 2);
        for (int i = 0; i < 3; i++) // run bouncing and click listener for each sprite
        {
            // get respective variables for sprite
            sf::Sprite &sprite = sprites[i % 3];
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
                     windowSize.y - 400 &&
                 increment.y > 0) ||
                (spritePosition.y - (size.y / 20) < 0 &&
                 increment.y < 0))
            { // bounce on y axis
                increment.y = -increment.y ;
            }

            sprite.setPosition(sprite.getPosition() + increment); // set position to the current position + the computed directional change
            window.draw(sprite);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isInArea(sf::Mouse::getPosition(window), sprite.getPosition()))
            {
                sprite.setPosition(rand() % window.getSize().x + 100, (window.getSize().y - 300));
                clicks += 1;
                double roundedClicks = clicks / 20;
                increment.y *= min(std::max(roundedClicks, 1.0), 1.05);
                cout << min(std::max(roundedClicks, 1.0), 1.05);
                            increment.x *= min(std::max(roundedClicks, 1.0), 1.03); // speed up if
            }
            if (level == 1) {
                texture.loadFromFile("assets/tinted.png");
            }
        }
        staff.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
        window.draw(staff);

        // Write the score in the corner
        sf::Text text(to_string((clicks)), font, 63);
        text.setPosition(10, 10);
        window.draw(text);
        if (clicks == 100)
        {
            if (level == 1) {
                clicks = 0;
                level = 0;
                continue;
            }
            level++;
            clicks = 0;

        }
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}
