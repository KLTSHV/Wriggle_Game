#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>


class Menu {
public:
    void showWelcomeScreen(sf::RenderWindow& window);
    
    void handleChangeSkin(sf::RenderWindow& window);
    void handleStatistics(sf::RenderWindow& window);
};

#endif // MENU_H
