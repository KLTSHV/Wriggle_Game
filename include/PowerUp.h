#ifndef POWERUP_H
#define POWERUP_H

#include <SFML/Graphics.hpp>

class PowerUp : public sf::Drawable {
public:
    PowerUp();
    void setPosition(float x, float y);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getGlobalBounds() const;

private:
    sf::Sprite sprite;
    sf::Texture texture;
};

#endif // POWERUP_H
