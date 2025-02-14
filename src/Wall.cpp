#include "../include/Wall.h"

Wall::Wall() {
    if (!wallTexture.loadFromFile(WALL_TEXTURE_PATH)) {
        // Ошибка при загрузке текстуры
        std::cerr << "Failed to load wall texture!" << std::endl;
    }
    wallShape.setTexture(&wallTexture); // Устанавливаем текстуру
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
