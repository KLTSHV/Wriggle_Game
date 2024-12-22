#include "../include/PowerUp.h"

// Constants
const std::string POWERUP_TEXTURE_PATH = "assets/powerup.png";
const float SCALE_X = 0.01f;
const float SCALE_Y = 0.01f;

PowerUp::PowerUp() {
    texture.loadFromFile(POWERUP_TEXTURE_PATH);
    sprite.setTexture(texture);
    sprite.setScale(SCALE_X, SCALE_Y);
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
