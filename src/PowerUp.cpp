#include "../include/PowerUp.h"
#include "../include/Constants.h"

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
