#include "../include/PowerUp.h"
#include "../include/Constants.h"

PowerUp::PowerUp() {
    if (!texture.loadFromFile(POWERUP_TEXTURE_PATH)) {
         std::cerr << "Error: Could not load bonus texture from " << POWERUP_TEXTURE_PATH << std::endl;
         // При желании можно установить запасную текстуру или задать цвет
    }
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
