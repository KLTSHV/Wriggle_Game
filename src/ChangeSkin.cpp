#include "/Users/egorkoltysev/Desktop/PROG/Wriggle/include/ChangeSkin.h"

#include <iostream>

bool ChangeSkin::show(sf::RenderWindow& window) {
    loadSkins();

    sf::Font font;
    font.loadFromFile("assets/arial.ttf");

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
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    return true; // Вернуться в меню
                }
                handleMouseClick(window);
            }
        }

        window.clear();
        window.draw(title);
        window.draw(backButton);
        // Добавить логику отображения скинов
        window.display();
    }

    return false;
}

void ChangeSkin::loadSkins() {
    // Доработать
    skinPaths = {"assets/powerup.png", "assets/player.png"};
    selectedSkin = 0;
}

void ChangeSkin::handleMouseClick(sf::RenderWindow& window) {
    // Обработка клика, доработать
    std::cout << "Skin clicked: " << skinPaths[selectedSkin] << std::endl;
}
