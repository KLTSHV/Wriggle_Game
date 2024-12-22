#include "../include/ChangeSkin.h"
#include <iostream>

// Constants
const std::string FONT_PATH = "../assets/arial.ttf";
const unsigned int TITLE_FONT_SIZE = 40;
const unsigned int BACK_BUTTON_FONT_SIZE = 30;
const sf::Color TITLE_COLOR = sf::Color::White;
const sf::Color BACK_BUTTON_COLOR = sf::Color::White;
const sf::Vector2f TITLE_POSITION = {200, 50};
const sf::Vector2f BACK_BUTTON_POSITION = {50, 500};
const std::vector<std::string> SKIN_PATHS = {"../assets/powerup.png", "../assets/player.png"};

bool ChangeSkin::show(sf::RenderWindow& window) {
    if (!loadResources()) {  // Ensure resources are loaded successfully
        std::cerr << "Failed to load resources for ChangeSkin!" << std::endl;
        return false;
    }

    sf::Font font;
    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "Error loading font from " << FONT_PATH << std::endl;
        return false;
    }

    sf::Text title("Change Skin", font, TITLE_FONT_SIZE);
    title.setFillColor(TITLE_COLOR);
    title.setPosition(TITLE_POSITION);

    sf::Text backButton("Back", font, BACK_BUTTON_FONT_SIZE);
    backButton.setFillColor(BACK_BUTTON_COLOR);
    backButton.setPosition(BACK_BUTTON_POSITION);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    return true;  // Go back to the menu
                }
                handleMouseClick(window);
            }
        }

        window.clear();
        window.draw(title);
        window.draw(backButton);
        window.display();
    }

    return false;
}

bool ChangeSkin::loadResources() {
    return loadSkins();  // Delegate to the private skin loader
}

bool ChangeSkin::loadSkins() {
    skinPaths = SKIN_PATHS;
    if (skinPaths.empty()) {
        std::cerr << "No skins found to load!" << std::endl;
        return false;
    }
    selectedSkin = 0;
    return true;
}

void ChangeSkin::handleMouseClick(sf::RenderWindow& window) {
    // Handle mouse click on skins
    if (!skinPaths.empty() && selectedSkin < skinPaths.size()) {
        std::cout << "Skin clicked: " << skinPaths[selectedSkin] << std::endl;
    }
}
