#ifndef STATISTICS_H
#define STATISTICS_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class Statistics {
public:
    bool show(sf::RenderWindow& window);

private:
    void loadStatistics(); // Загрузить статистику

    std::map<std::string, int> stats; // Хранение статистики (пример: {"High Score": 1234})
};

#endif // STATISTICS_H
