#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>

class Wall : public sf::Drawable {
public:
    Wall();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setPosition(float x, float y);
    void setSize(float length, bool isVertical);
    sf::FloatRect getGlobalBounds() const;
private:
    sf::RectangleShape wallShape;
    static constexpr float thickness = 20.0f; // Толщина стены ФИКСИРОВАНА
};

#endif // WALL_H
