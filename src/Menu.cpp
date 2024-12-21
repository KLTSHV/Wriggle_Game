#include "../include/Menu.h"
#include <iostream>
#include "../include/ChangeSkin.h"
#include "../include/Statistics.h"
#include "../include/Game.h"

bool Menu::showWelcomeScreen(sf::RenderWindow& window, Game& game) {
    sf::Font font;
    if (!font.loadFromFile("../assets/arial.ttf")) {
        std::cerr << "Error loading font from assets/arial.ttf" << std::endl;
        return false; // Exit if font loading fails
    }

    sf::Text title("Arcade Game", font, 50);
    title.setFillColor(sf::Color::White);
    title.setPosition(250, 100);

    sf::Text start("Start Game", font, 30);
    start.setFillColor(sf::Color::White);
    start.setPosition(250, 250);

    sf::Text changeSkin("Change Skin", font, 30);
    changeSkin.setFillColor(sf::Color::White);
    changeSkin.setPosition(250, 300);

    sf::Text statistics("Statistics", font, 30);
    statistics.setFillColor(sf::Color::White);
    statistics.setPosition(250, 350);

    sf::Text exit("Exit", font, 30);
    exit.setFillColor(sf::Color::White);
    exit.setPosition(250, 400);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (start.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    game.setGameRunning(true);
                    return true;
                }
                if (changeSkin.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    this->handleChangeSkin(window);
                }
                if (statistics.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    this->handleStatistics(window);
                }
                if (exit.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    window.close();
                }
            }
        }

        window.clear();
        window.draw(title);
        window.draw(start);
        window.draw(changeSkin);
        window.draw(statistics);
        window.draw(exit);
        window.display();
    }
    return false;
}

void Menu::handleChangeSkin(sf::RenderWindow& window) {
    ChangeSkin changeSkin;
    if (!changeSkin.loadSkins()) {
        std::cerr << "Error loading skin resources" << std::endl;
        return;
    }
    changeSkin.show(window);
}

void Menu::handleStatistics(sf::RenderWindow& window) {
    Statistics statistics;
    if (!statistics.loadStatistics()) {
        std::cerr << "Error loading statistics resources" << std::endl;
        return;
    }
    statistics.show(window);
}