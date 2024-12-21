#ifndef CHANGESKIN_H
#define CHANGESKIN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class ChangeSkin {
public:
    bool show(sf::RenderWindow& window);  // Public method to show the ChangeSkin screen
    bool loadResources();                 // Public method for loading resources (e.g., skins)
    bool loadSkins();                     // Private method for loading skins
private:
    void handleMouseClick(sf::RenderWindow& window);

    std::vector<std::string> skinPaths;
    int selectedSkin;
};

#endif // CHANGESKIN_H

