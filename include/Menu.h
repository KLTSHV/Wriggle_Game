#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

// Forward declare the Game class instead of including Game.h here
class Game;

class Menu {
public:
    bool showWelcomeScreen(sf::RenderWindow& window, Game& game);
    
    void handleChangeSkin(sf::RenderWindow& window);
    void handleStatistics(sf::RenderWindow& window);
};

#endif // MENU_H
