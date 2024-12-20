#include "../include/Menu.h"
#include <iostream>
#include "../include/ChangeSkin.h"
#include "../include/Statistics.h"
#include "../include/Game.h"  // Include Game.h here, where the full definition is needed

bool Menu::showWelcomeScreen(sf::RenderWindow& window, Game& game) {
    sf::Font font;
    font.loadFromFile("assets/arial.ttf");

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

                if (start.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    // Изменяем флаг через объект game
                    
                    game.setGameRunning(true); // Теперь игра начнется
                    return true; // Выход из меню
                }
                if (changeSkin.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    this->handleChangeSkin(window);
                }
                if (statistics.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    this->handleStatistics(window);
                }
                if (exit.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    window.close(); // Закрыть окно
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
}



void Menu::handleChangeSkin(sf::RenderWindow& window) {
    ChangeSkin changeSkin;
    if (changeSkin.show(window)) {
        return; // Вернуться в главное меню
    }
}

void Menu::handleStatistics(sf::RenderWindow& window) {
    Statistics statistics;
    if (statistics.show(window)) {
        return; // Вернуться в главное меню
    }
}
