#include "../include/PowerUp.h"

PowerUp::PowerUp() {
    texture.loadFromFile("assets/powerup.png");
    sprite.setTexture(texture);
    sprite.setScale(0.01f, 0.01f);
}

void PowerUp::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void PowerUp::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}

sf::FloatRect PowerUp::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}
