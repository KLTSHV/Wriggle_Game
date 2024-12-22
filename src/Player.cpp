#include "../include/Player.h"
#include "../include/Wall.h" 
#include <iostream>

// Константы
constexpr float INITIAL_SPEED = 8.0f;
constexpr float DASH_DISTANCE = 100.0f; // Предполагаемое расстояние рывка
constexpr float SPEED_UP_SPEED = 15.0f;
constexpr float SPEED_UP_DURATION = 7.0f;
constexpr float INVINCIBILITY_DURATION = 5.0f;
constexpr float SHRINK_SCALE_X = 0.01f;
constexpr float SHRINK_SCALE_Y = 0.01f;
constexpr float SHRINK_DURATION = 7.0f;
constexpr float NORMAL_SCALE_X = 0.02f;
constexpr float NORMAL_SCALE_Y = 0.02f;
constexpr float DASH_COOLDOWN = 6.0f;

Player::Player() : 
    speed(INITIAL_SPEED), 
    invincible(false), 
    invincibilityTimer(0), 
    speededUp(false), 
    speededUpTimer(0) {
    texture.loadFromFile("../assets/player.png");
    std::cout << "Error here pa" << std::endl;
    sprite.setTexture(texture);
    std::cout << "Error here pb" << std::endl;
    sprite.setScale(NORMAL_SCALE_X, NORMAL_SCALE_Y);
}

void Player::setPosition(float x, float y) {
    std::cout << "Error here sp1" << std::endl;
    sprite.setPosition(x, y);
    std::cout << "Error here sp2" << std::endl;
}

float Player::returnSpeed() {
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
        sprite.move(direction * DASH_DISTANCE);
        canDash = false;
        dashTimer = 0.0f;
    }
}

void Player::resetDash() {
    if (dashTimer >= DASH_COOLDOWN) {
        canDash = true;
    }
}

bool Player::isInvincible() const {
    return invincible;
}

bool Player::isSpeededUp() const {
    return speededUp;
}

void Player::makeSpeededUp() {
    speed = SPEED_UP_SPEED;
    speededUpTimer = SPEED_UP_DURATION;
    speededUp = true;
}

void Player::makeInvincible() {
    invincible = true;
    invincibilityTimer = INVINCIBILITY_DURATION;
}

void Player::shrink() {
    this->setScale(SHRINK_SCALE_X, SHRINK_SCALE_Y);
    shrinked = true;
    shrinkedTimer = SHRINK_DURATION;
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
        speed = INITIAL_SPEED;
    }
    if (dashTimer >= DASH_COOLDOWN) {
        resetDash();
    }
    if (shrinkedTimer <= 0) {
        shrinked = false;
        this->setScale(NORMAL_SCALE_X, NORMAL_SCALE_Y);
    }
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}

sf::FloatRect Player::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

void Player::setScale(float x, float y) {
    this->sprite.setScale(x, y);
}

void Player::dash(float dx, float dy) {
    sprite.move(dx, dy);
}
