#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>
#include "../include/Constants.h"

class Wall : public sf::Drawable {
public:
    Wall();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setPosition(float x, float y);
    void setSize(float length, bool isVertical);
    sf::FloatRect getGlobalBounds() const;
private:
    sf::RectangleShape wallShape;
    sf::Texture wallTexture; // Текстура для стены
    float thickness = WALL_THICKNESS;
};

#endif // WALL_H
