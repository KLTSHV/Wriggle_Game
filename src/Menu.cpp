#include "/Users/egorkoltysev/Desktop/PROG/Wriggle/include/Menu.h"
#include <iostream>
#include "/Users/egorkoltysev/Desktop/PROG/Wriggle/include/ChangeSkin.h"
#include "/Users/egorkoltysev/Desktop/PROG/Wriggle/include/Statistics.h"

void Menu::showWelcomeScreen(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("assets/arial.ttf");

    // Заголовок
    sf::Text title("Arcade Game", font, 50);
    title.setFillColor(sf::Color::White);
    title.setPosition(250, 100);

    // Опции меню
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

                // Проверяем, попадает ли мышь в текст
                if (start.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    return; // Переход к игре
                }
                if (changeSkin.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    // Логика для смены скина
                    this->handleChangeSkin(window);
                }
                if (statistics.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    // Логика для отображения статистики
                    this->handleStatistics(window);
                }
                if (exit.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    window.close(); // Закрыть окно
                }
            }
        }

        // Рисуем меню
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