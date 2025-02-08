#ifndef CHANGESKIN_H
#define CHANGESKIN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class ChangeSkin {
public:
    bool show(sf::RenderWindow& window);  // Показ экрана выбора скина
    bool loadResources();                 // Загрузка ресурсов
    bool loadSkins();                     // Загрузка скинов
    
    int getSelectedSkin() const { return selectedSkin; }
private:
    void handleMouseClick(sf::RenderWindow& window);

    std::vector<std::string> skinPaths;
    int selectedSkin;
    std::vector<sf::Texture> skinTextures;  // Загруженные текстуры скинов
    std::vector<sf::Sprite> skinSprites;      // Спрайты скинов для отображения
    sf::Font font;     
};

#endif 

