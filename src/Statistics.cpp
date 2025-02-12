#include "../include/Statistics.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Constants.h"
#include <SFML/Audio.hpp>

bool Statistics::show(sf::RenderWindow& window) {
    if (!loadResources()) { 
        std::cerr << "Failed to load statistics!" << std::endl;
        return false;
    }

    sf::Font font;
    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "Error loading font from " << FONT_PATH << std::endl;
        return false;
    }

    sf::Text title("Statistics", font, STAT_TITLE_FONT_SIZE);
    title.setFillColor(STAT_TEXT_COLOR);
    title.setPosition(STAT_TITLE_POSITION);

    sf::Text backButton("Back", font, STAT_BUTTON_FONT_SIZE);
    backButton.setFillColor(STAT_TEXT_COLOR);
    backButton.setPosition(STAT_BACK_BUTTON_POSITION);

    // Флаг для отслеживания наведения на кнопку
    bool wasHoveredBack = false;

    // коэффициент интерполяции
    const float animationSpeed = 0.1f;

    std::vector<sf::Text> statsTexts;
    int yPosition = STAT_TEXT_POSITION.y;
    for (const auto& stat : stats) {
        sf::Text statText(stat.first + ": " + std::to_string(stat.second), font, STAT_TEXT_FONT_SIZE);
        statText.setFillColor(STAT_TEXT_COLOR);
        statText.setPosition(STAT_TEXT_POSITION.x, yPosition);
        yPosition += STAT_TEXT_Y_INCREMENT;
        statsTexts.push_back(statText);
    }

    // Лямбда-функция для обновления анимации интерактивного текста
    auto updateInteractiveText = [&](sf::Text &text, bool &wasHovered, const sf::Color &defaultColor) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool isHovered = text.getGlobalBounds().contains(mousePos);

        // Целевой масштаб и цвет при наведении
        float targetScale = isHovered ? 1.1f : 1.0f;
        sf::Color targetColor = isHovered ? HOVER_TEXT_COLOR : defaultColor;

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

        wasHovered = isHovered;
    };

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
                    if (!saveStatistics()) {
                        std::cerr << "Failed to save statistics!" << std::endl;
                    }
                    return true;  // Возврат в меню
                }
            }
        }

        // Обновляем анимацию для кнопки "Back"
        updateInteractiveText(backButton, wasHoveredBack, STAT_TEXT_COLOR);

        window.clear();
        window.draw(title);
        window.draw(backButton);
        for (const auto& statText : statsTexts) {
            window.draw(statText);
        }
        window.display();
    }

    return false;
}


bool Statistics::loadResources() {
    return loadStatistics();
}

bool Statistics::loadStatistics() {
    std::ifstream file(STAT_FILE_PATH);

    if (!file.is_open()) {

        stats["High Score"] = DEFAULT_HIGH_SCORE;
        stats["Games Played"] = DEFAULT_GAMES_PLAYED;
        stats["Total Time"] = DEFAULT_TOTAL_TIME;
        return true;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string key;
        int value;
        if (std::getline(ss, key, ':') && ss >> value) {
            stats[key] = value;
        }
    }

    file.close();
    return true;
}

bool Statistics::saveStatistics() {
    std::ofstream file(STAT_FILE_PATH);

    if (!file.is_open()) {
        std::cerr << "Failed to open statistics file for saving!" << std::endl;
        return false;
    }

    for (const auto& stat : stats) {
        file << stat.first << ":" << stat.second << std::endl;
    }

    file.close();
    return true;
}
