#include "../include/Player.h"
#include "../include/Wall.h" 
#include "iostream"

Player::Player() : speed(8.0f), invincible(false), invincibilityTimer(0), speededUp(false), speededUpTimer(0) {
    texture.loadFromFile("assets/player.png");
    std::cout << "Error here pa" << std::endl;
    sprite.setTexture(texture);
    std::cout << "Error here pb" << std::endl;
    sprite.setScale(0.02f, 0.02f);
}
void Player::setPosition(float x, float y) {
    std::cout << "Error here sp1" << std::endl;
    sprite.setPosition(x, y);
    std::cout << "Error here sp2" << std::endl;
}
float Player::returnSpeed(){
    return this->speed;
}
void Player::move(sf::RenderWindow& window, float dx, float dy, const std::vector<std::unique_ptr<Wall>>& walls) {
    sf::Vector2f newPosition = sprite.getPosition() + sf::Vector2f(dx, dy);
    sf::FloatRect newBounds = sprite.getGlobalBounds();
    newBounds.left += dx;
    newBounds.top += dy;
    for (const auto& wall : walls) {
        if (newBounds.intersects(wall->getGlobalBounds())) {
            // Если пересечение есть, блокируем движение
            return;
        }
    }
    sprite.move(dx, dy);
    // Обработка границ карты
    sf::Vector2f position = sprite.getPosition();
    if (position.x < 0) position.x = window.getSize().x;
    else if (position.x > window.getSize().x) position.x = 0;
    if (position.y < 0) position.y = window.getSize().y;
    else if (position.y > window.getSize().y) position.y = 0;
    sprite.setPosition(position);
}
void Player::attemptDash(sf::Vector2f direction) {
    if (canDash) {
        sprite.move(direction * dashDistance);
        canDash = false;
        dashTimer = 0.0f;
    }
}
void Player::resetDash() {
    if (dashTimer >= 6.0f) {
        canDash = true;
    }
}
bool Player::isInvincible() const {
    return invincible;
}
bool Player::isSpeededUp() const{
    return speededUp;
}
void Player::makeSpeededUp(){
    speed = 15.0f;
    speededUpTimer = 7.0f;
    speededUp = true;
}
void Player::makeInvincible(){
    invincible = true;
    invincibilityTimer = 5.0f;
}
void Player::shrink(){
    this->setScale(0.01f,0.01f);
    shrinked = true;
    shrinkedTimer = 7.0f;
}
void Player::reset() {
    invincible = false;
    invincibilityTimer = 0;
    speededUp = false;
    speededUpTimer = 0;

}
void Player::updateTimers(float elapsedSeconds) {
    invincibilityTimer -= elapsedSeconds;
    speededUpTimer -= elapsedSeconds;
    dashTimer += elapsedSeconds;
    shrinkedTimer -= elapsedSeconds;
    if (invincibilityTimer <= 0) {
        invincible = false;
    }
    if (speededUpTimer <= 0) {
        speededUp = false;
        speed = 8.0f;
    }
    if (dashTimer >= 6.0f) {
        resetDash();
    }
    if(shrinkedTimer <= 0){
        shrinked = false;
        this->setScale(0.02, 0.02);
    }
}
void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}
sf::FloatRect Player::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}
void Player::setScale(float x, float y){
    this->sprite.setScale(x, y);
}
void Player::dash(float dx, float dy) {
    // Perform the dash movement by directly updating the sprite position
    sprite.move(dx, dy);
}


