#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

class Game;

class Menu {
public:
    bool showWelcomeScreen(sf::RenderWindow& window, Game& game);
    
    void handleChangeSkin(sf::RenderWindow& window, Game& game);
    void handleStatistics(sf::RenderWindow& window);
};

#endif // MENU_H
