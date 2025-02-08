#ifndef STATISTICS_H
#define STATISTICS_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class Statistics {
public:
    bool show(sf::RenderWindow& window);  
    bool loadResources();
    bool loadStatistics();                  
private:             
    bool saveStatistics();                

    std::map<std::string, int> stats;
};

#endif // STATISTICS_H
