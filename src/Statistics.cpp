#include "../include/Statistics.h"
#include <iostream>
#include <fstream>
#include <sstream>

bool Statistics::show(sf::RenderWindow& window) {
    if (!loadResources()) {  // Ensure statistics are loaded successfully
        std::cerr << "Failed to load statistics!" << std::endl;
        return false;
    }

    sf::Font font;
    if (!font.loadFromFile("../assets/arial.ttf")) {
        std::cerr << "Error loading font from assets/arial.ttf" << std::endl;
        return false;
    }

    sf::Text title("Statistics", font, 40);
    title.setFillColor(sf::Color::White);
    title.setPosition(200, 50);

    sf::Text backButton("Back", font, 30);
    backButton.setFillColor(sf::Color::White);
    backButton.setPosition(50, 500);

    std::vector<sf::Text> statsTexts;
    int yPosition = 150;
    for (const auto& stat : stats) {
        sf::Text statText(stat.first + ": " + std::to_string(stat.second), font, 30);
        statText.setFillColor(sf::Color::White);
        statText.setPosition(200, yPosition);
        yPosition += 50;
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
    std::ifstream file("statistics.txt");

    if (!file.is_open()) {
        // If file doesn't exist, initialize with default values
        stats["High Score"] = 1234;
        stats["Games Played"] = 56;
        stats["Total Time"] = 12345;
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
    std::ofstream file("statistics.txt");

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
