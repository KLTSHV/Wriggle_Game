#ifndef STATISTICS_H
#define STATISTICS_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class Statistics {
public:
    bool show(sf::RenderWindow& window);
    void loadStatistics();
    void saveStatistics();

private:

    std::map<std::string, int> stats; // Хранение статистики
};

#endif // STATISTICS_H
