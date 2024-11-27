#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>

class Wall : public sf::Drawable {
public:
    Wall();

    void setPosition(float x, float y);
    void setSize(float length, bool isVertical);
    sf::FloatRect getGlobalBounds() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::RectangleShape wallShape;
    static constexpr float thickness = 20.0f; // Толщина стены фиксирована
};

#endif // WALL_H
