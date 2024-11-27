#include "/Users/egorkoltysev/Desktop/PROG/Wriggle/include/Wall.h"

Wall::Wall() {
    wallShape.setFillColor(sf::Color(128, 128, 128)); // Серый цвет
}

void Wall::setPosition(float x, float y) {
    wallShape.setPosition(x, y);
}

void Wall::setSize(float length, bool isVertical) {
    if (isVertical) {
        wallShape.setSize(sf::Vector2f(thickness, length)); // Вертикальная стена
    } else {
        wallShape.setSize(sf::Vector2f(length, thickness)); // Горизонтальная стена
    }
}

sf::FloatRect Wall::getGlobalBounds() const {
    return wallShape.getGlobalBounds();
}

void Wall::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(wallShape, states);
}
