#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>


class Wall;

class Player : public sf::Drawable {
public:
    Player();


    void resetDash();

    void setPosition(float x, float y);
    void move(float dx, float dy, const std::vector<std::unique_ptr<Wall>>& walls, float deltaTime);
    void dash(int direction, const std::vector<std::unique_ptr<Wall>>& walls);
    void updateTimers(float elapsedSeconds);
    bool isInvincible() const;
    bool isSpeededUp() const;
    void activateRandomPowerUp();
    void reset();
    void shrink();
    void setScale(float x, float y);
    float returnSpeed();
    void makeInvincible();
    void makeSpeededUp();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getGlobalBounds() const;
    sf::Sprite sprite;
    
private:

    float dashTimer;      
    bool canDash;         
    float dashDistance;   
    
    sf::Texture texture;
    bool invincible;
    float invincibilityTimer;
    bool speededUp;
    float speededUpTimer;
    bool shrinked;
    float shrinkedTimer;
    float speed = 8.0f;
    int direction;
};

#endif // PLAYER_H
