#include "../include/Player.h"
#include "../include/Wall.h"
#include <iostream>
#include "../include/Constants.h"

Player::Player() : 
    speed(INITIAL_SPEED), 
    invincible(false), 
    invincibilityTimer(0), 
    speededUp(false), 
    speededUpTimer(0),
    canDash(true), 
    dashDistance(DASH_DISTANCE),
    dashTimer(0) {
    texture.loadFromFile("../assets/skin_2.png"); //По умолчанию
    sprite.setTexture(texture);
    sprite.setScale(NORMAL_SCALE_X, NORMAL_SCALE_Y);
}

void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

float Player::returnSpeed() {
    return this->speed;
}

void Player::move(float dx, float dy, const std::vector<std::unique_ptr<Wall>>& walls, float deltaTime) {
    // Получаем исходное положение
    sf::Vector2f originalPos = sprite.getPosition();
    sf::Vector2f newPos = originalPos;

    // Пытаемся переместиться по горизонтали
    newPos.x += dx * deltaTime;
    sf::FloatRect newBounds = sprite.getGlobalBounds();
    newBounds.left = newPos.x;  // обновляем только горизонтальную позицию
    bool collisionX = false;
    for (const auto& wall : walls) {
        if (newBounds.intersects(wall->getGlobalBounds())) {
            collisionX = true;
            break;
        }
    }
    if (!collisionX) {
        originalPos.x = newPos.x;  // если столкновения нет, применяем изменение по X
    }

    // Пытаемся переместиться по вертикали
    newPos = originalPos;
    newPos.y += dy * deltaTime;
    newBounds = sprite.getGlobalBounds();
    newBounds.top = newPos.y;  // обновляем только вертикальную позицию
    bool collisionY = false;
    for (const auto& wall : walls) {
        if (newBounds.intersects(wall->getGlobalBounds())) {
            collisionY = true;
            break;
        }
    }
    if (!collisionY) {
        originalPos.y = newPos.y;  // если столкновения нет, применяем изменение по Y
    }

    // Обновляем позицию спрайта
    sprite.setPosition(originalPos);

    // Реализация телепортации с учетом размеров спрайта
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    sf::Vector2f pos = sprite.getPosition();

    if (pos.x + spriteBounds.width < 0) {  
        pos.x = WINDOW_WIDTH;
    } else if (pos.x > WINDOW_WIDTH) {
        pos.x = -spriteBounds.width;
    }

    if (pos.y + spriteBounds.height < 0) {  
        pos.y = WINDOW_HEIGHT;
    } else if (pos.y > WINDOW_HEIGHT) {
        pos.y = -spriteBounds.height;
    }

    sprite.setPosition(pos);
}


void Player::resetDash() {
    if (dashTimer >= DASH_COOLDOWN) {
        canDash = true;
        dashTimer = 0;
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
    canDash = true;
    dashTimer = 0;
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
    resetDash();
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

void Player::dash(int direction, const std::vector<std::unique_ptr<Wall>>& walls) {
    if (!canDash)
        return;
    
    // Определяем вектор дэша в зависимости от направления
    float dx = 0, dy = 0;
    switch (direction) {
        case 0: dy = -dashDistance; break;              // Вверх
        case 1: dx = dashDistance; dy = -dashDistance; break; // Вверх-вправо
        case 2: dx = dashDistance; break;                // Вправо
        case 3: dx = dashDistance; dy = dashDistance; break;  // Вправо-вниз
        case 4: dy = dashDistance; break;                // Вниз
        case 5: dx = -dashDistance; dy = dashDistance; break; // Вниз-влево
        case 6: dx = -dashDistance; break;               // Влево
        case 7: dx = -dashDistance; dy = -dashDistance; break; // Влево-вверх
        default: break;
    }
    
    sf::Vector2f dashVector(dx, dy);
    float magnitude = std::sqrt(dashVector.x * dashVector.x + dashVector.y * dashVector.y);
    sf::Vector2f unitDash = (magnitude != 0) ? dashVector / magnitude : sf::Vector2f(0, 0);
    
    // Определяем шаг, например, 5 пикселей, и число шагов
    const float DASH_STEP = 5.0f;
    int steps = static_cast<int>(magnitude / DASH_STEP);
    
    // Текущая позиция игрока
    sf::Vector2f currentPos = sprite.getPosition();
    sf::Vector2f newPos = currentPos;
    bool collisionFound = false;
    
    // Пройдемся по пути дэша по шагам
    for (int i = 0; i < steps; ++i) {
        newPos += unitDash * DASH_STEP;
        // Вычисляем новые границы игрока (смещение относительно исходного положения)
        sf::FloatRect newBounds = sprite.getGlobalBounds();
        newBounds.left += (newPos.x - currentPos.x);
        newBounds.top  += (newPos.y - currentPos.y);
        
        // Проверяем столкновение с каждой стеной
        for (const auto& wall : walls) {
            if (newBounds.intersects(wall->getGlobalBounds())) {
                collisionFound = true;
                break;
            }
        }
        if (collisionFound)
            break;
    }
    
    // Если столкновение найдено, возвращаемся на один шаг назад, чтобы не попасть внутрь стены
    if (collisionFound)
        newPos -= unitDash * DASH_STEP;
    
    // Устанавливаем новую позицию и начинаем кулдаун дэша
    sprite.setPosition(newPos);
    canDash = false;
    dashTimer = 0;
}
void Player::setSkin(int skinIndex) {
    // Проверяем корректность индекса
    if (skinIndex < 0 || skinIndex >= static_cast<int>(CHANGESKIN_SKIN_PATHS.size())) {
        std::cerr << "Invalid skin index: " << skinIndex << std::endl;
        return;
    }
    // Загружаем текстуру из выбранного пути
    if (!texture.loadFromFile(CHANGESKIN_SKIN_PATHS[skinIndex])) {
        std::cerr << "Error loading skin from " << CHANGESKIN_SKIN_PATHS[skinIndex] << std::endl;
        return;
    }
    // Устанавливаем текстуру для спрайта
    sprite.setTexture(texture, true);
    // Приводим масштаб к стандартном
    sprite.setScale(NORMAL_SCALE_X, NORMAL_SCALE_Y);
}