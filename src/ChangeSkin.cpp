#include "../include/ChangeSkin.h"
#include <iostream>

bool ChangeSkin::show(sf::RenderWindow& window) {
    if (!loadResources()) {  // Ensure resources are loaded successfully
        std::cerr << "Failed to load resources for ChangeSkin!" << std::endl;
        return false;
    }

    sf::Font font;
    if (!font.loadFromFile("../assets/arial.ttf")) {
        std::cerr << "Error loading font from assets/arial.ttf" << std::endl;
        return false;
    }

    sf::Text title("Change Skin", font, 40);
    title.setFillColor(sf::Color::White);
    title.setPosition(200, 50);

    sf::Text backButton("Back", font, 30);
    backButton.setFillColor(sf::Color::White);
    backButton.setPosition(50, 500);

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
    // Example of predefined skin paths
    skinPaths = {"../assets/powerup.png", "../assets/player.png"};
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
