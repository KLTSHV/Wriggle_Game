#include "../include/Menu.h"
#include <iostream>
#include "../include/ChangeSkin.h"
#include "../include/Statistics.h"
#include "../include/Game.h"
#include "../include/Constants.h"

bool Menu::showWelcomeScreen(sf::RenderWindow& window, Game& game) {
    sf::Font font;
    if (!font.loadFromFile("../assets/arial.ttf")) {
        std::cerr << "Error loading font from assets/arial.ttf" << std::endl;
        return false; // Exit if font loading fails
    }

    sf::Text title("Arcade Game", font, TITLE_FONT_SIZE);
    title.setFillColor(TEXT_COLOR);
    title.setPosition(TITLE_X, TITLE_Y);

    sf::Text start("Start Game", font, OPTION_FONT_SIZE);
    start.setFillColor(TEXT_COLOR);
    start.setPosition(START_X, START_Y);

    sf::Text changeSkin("Change Skin", font, OPTION_FONT_SIZE);
    changeSkin.setFillColor(TEXT_COLOR);
    changeSkin.setPosition(CHANGE_SKIN_X, CHANGE_SKIN_Y);

    sf::Text statistics("Statistics", font, OPTION_FONT_SIZE);
    statistics.setFillColor(TEXT_COLOR);
    statistics.setPosition(STATISTICS_X, STATISTICS_Y);

    sf::Text exit("Exit", font, OPTION_FONT_SIZE);
    exit.setFillColor(TEXT_COLOR);
    exit.setPosition(EXIT_X, EXIT_Y);

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
                    game.gameClock.restart();
                    
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
