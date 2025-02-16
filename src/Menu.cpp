#include "../include/Menu.h"
#include <iostream>
#include "../include/ChangeSkin.h"
#include "../include/Statistics.h"
#include "../include/Game.h"
#include "../include/Constants.h"
#include <SFML/Audio.hpp>
#include <vector>
#include <algorithm>

bool Menu::showWelcomeScreen(sf::RenderWindow& window, Game& game) {
    sf::Font font;
    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "Error loading font" << std::endl;
        return false;
    }

    sf::Text title("Wriggle", font, TITLE_FONT_SIZE);
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

    // Флаги для отслеживания входа курсора на элемент (чтобы звук запускался один раз при входе)
    bool wasHoveredStart = false;
    bool wasHoveredChangeSkin = false;
    bool wasHoveredStatistics = false;
    bool wasHoveredExit = false;

    // Параметр анимации – коэффициент интерполяции (чем выше значение, тем быстрее анимация)
    const float animationSpeed = 0.1f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (start.getGlobalBounds().contains(mousePos)) {
                    game.setGameRunning(true);
                    game.gameClock.restart();
                    return true;
                }
                if (changeSkin.getGlobalBounds().contains(mousePos)) {
                    this->handleChangeSkin(window, game);
                }
                if (statistics.getGlobalBounds().contains(mousePos)) {
                    this->handleStatistics(window);
                }
                if (exit.getGlobalBounds().contains(mousePos)) {
                    window.close();
                }
            }
        }

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Лямбда для обновления анимации и звука для одного пункта меню
        auto updateText = [&](sf::Text &text, bool &wasHovered) {
            bool isHovered = text.getGlobalBounds().contains(mousePos);

            // Задаю целевой масштаб и цвет
            float targetScale = isHovered ? 1.1f : 1.0f;
            sf::Color targetColor = isHovered ? HOVER_TEXT_COLOR : TEXT_COLOR;

            // Плавное изменение масштаба
            sf::Vector2f currentScale = text.getScale();
            float newScale = currentScale.x + (targetScale - currentScale.x) * animationSpeed;
            text.setScale(newScale, newScale);

            // Плавное изменение цвета
            sf::Color currentColor = text.getFillColor();
            auto lerp = [&](sf::Uint8 current, sf::Uint8 target) -> sf::Uint8 {
                float c = static_cast<float>(current);
                float t = static_cast<float>(target);
                c += (t - c) * animationSpeed;
                return static_cast<sf::Uint8>(c);
            };

            sf::Color newColor(
                lerp(currentColor.r, targetColor.r),
                lerp(currentColor.g, targetColor.g),
                lerp(currentColor.b, targetColor.b),
                255
            );
            text.setFillColor(newColor);
        };

        updateText(start, wasHoveredStart);
        updateText(changeSkin, wasHoveredChangeSkin);
        updateText(statistics, wasHoveredStatistics);
        updateText(exit, wasHoveredExit);

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

void Menu::handleChangeSkin(sf::RenderWindow &window, Game& game) {
    ChangeSkin changeSkin;
    if (!changeSkin.loadSkins()) {
        std::cerr << "Error loading skin resources" << std::endl;
        return;
    }
    changeSkin.show(window);
    game.setSelectedSkin(changeSkin.getSelectedSkin());
}
void Menu::handleStatistics(sf::RenderWindow& window) {
    Statistics statistics;
    if (!statistics.loadStatistics()) {
        std::cerr << "Error loading statistics resources" << std::endl;
        return;
    }
    statistics.show(window);
}
