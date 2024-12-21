#ifndef STATISTICS_H
#define STATISTICS_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class Statistics {
public:
    bool show(sf::RenderWindow& window);  // Public method to show statistics screen
    bool loadResources();
    bool loadStatistics();                   // Public method to load statistics
private:             
    bool saveStatistics();                // Private method for saving statistics

    std::map<std::string, int> stats;
};

#endif // STATISTICS_H
