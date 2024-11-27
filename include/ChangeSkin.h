#ifndef CHANGESKIN_H
#define CHANGESKIN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class ChangeSkin {
public:
    bool show(sf::RenderWindow& window); // Возвращает true, если нужно вернуться в меню

private:
    void loadSkins(); // Загрузить доступные скины
    void handleMouseClick(sf::RenderWindow& window);

    std::vector<std::string> skinPaths; // Пути к файлам скинов
    int selectedSkin = 0; // Индекс выбранного скина
};

#endif // CHANGESKIN_H
