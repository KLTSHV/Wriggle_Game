#include "../include/Statistics.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Constants.h"

bool Statistics::show(sf::RenderWindow& window) {
    if (!loadResources()) {  // Ensure statistics are loaded successfully
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

    std::vector<sf::Text> statsTexts;
    int yPosition = STAT_TEXT_POSITION.y;
    for (const auto& stat : stats) {
        sf::Text statText(stat.first + ": " + std::to_string(stat.second), font, STAT_TEXT_FONT_SIZE);
        statText.setFillColor(STAT_TEXT_COLOR);
        statText.setPosition(STAT_TEXT_POSITION.x, yPosition);
        yPosition += STAT_TEXT_Y_INCREMENT;
        statsTexts.push_back(statText);
    }

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
                    return true;  // Return to menu
                }
            }
        }

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
    return loadStatistics();  // Delegate to the private loader
}

bool Statistics::loadStatistics() {
    std::ifstream file(STAT_FILE_PATH);

    if (!file.is_open()) {
        // If file doesn't exist, initialize with default values
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
