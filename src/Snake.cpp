// // Snake.cpp

// #include "../include/Snake.h"
// #include <cmath>
// #include <memory>
// #include "../include/Constants.h"

// Snake::Snake() 
//     : speed(0), 
//       angle(0), 
//       velocity(0, 0), 
//       amountOfSegments(0), 
//       sizeOfSegments(DEFAULT_SIZE_OF_SEGMENTS) {
//     // Создаём голову змеи с нужным размером и устанавливаем центр, чтобы позиционирование было корректным
//     auto head = std::make_unique<sf::CircleShape>(sizeOfSegments);
//     head->setFillColor(sf::Color::Green);
//     head->setOrigin(sizeOfSegments, sizeOfSegments); // Центрирование
//     segments.push_back(std::move(head));
// }

// void Snake::grow() {
//     if (segments.empty()) return;

//     // Получаем позицию последнего сегмента (начинаем с головы)
//     sf::Vector2f lastSegmentPosition = segments.back()->getPosition();

//     // Создаём новые сегменты позади головы
//     for (int i = 0; i < amountOfSegments; ++i) {
//         auto newSegment = std::make_unique<sf::CircleShape>(sizeOfSegments);
//         newSegment->setFillColor(sf::Color::Green);
//         newSegment->setOrigin(sizeOfSegments, sizeOfSegments); // Центрирование

//         // Смещаем позицию назад вдоль направления движения головы
//         lastSegmentPosition.x -= sizeOfSegments * std::cos(angle);
//         lastSegmentPosition.y -= sizeOfSegments * std::sin(angle);
//         newSegment->setPosition(lastSegmentPosition);

//         segments.push_back(std::move(newSegment));
//     }
// }

// // Исправленный метод отрисовки: больше не делаем лишнее смещение
// void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const {
//     for (const auto& segment : segments) {
//         target.draw(*segment, states);
//     }
// }


// void Snake::setPosition(float x, float y) {
//     if (!segments.empty()) {
//         segments[0]->setPosition(x, y);  
//     }
// }


// void Snake::setAngle(float angle) {
//     this->angle = angle;
    
//     velocity.x = speed * std::cos(angle);
//     velocity.y = speed * std::sin(angle);
// }


// void Snake::setSpeed(float speed) {
//     this->speed = speed;
    
//     velocity.x = speed * std::cos(angle);
//     velocity.y = speed * std::sin(angle);
//     isStopped = false;
// }


// void Snake::setAmountOfSegments(int amount) {
//     this->amountOfSegments = amount;
// }

// void Snake::setSizeOfSegments(float size) {
//     this->sizeOfSegments = size;
// }


// void Snake::move(float deltaTime) {
//     if (segments.empty()) return;

//     // Рассчитываем смещение головы с учетом deltaTime
//     float xOffset = speed * std::cos(angle) * deltaTime;
//     float yOffset = speed * std::sin(angle) * deltaTime;

//     // Сдвигаем голову
//     sf::Vector2f previousPosition = segments[0]->getPosition();
//     segments[0]->move(xOffset, yOffset);

//     // Перемещаем тело змеи, обновляя позицию каждого сегмента
//     for (size_t i = 1; i < segments.size(); ++i) {
//         sf::Vector2f tempPosition = segments[i]->getPosition();
//         segments[i]->setPosition(previousPosition);
//         previousPosition = tempPosition;
//     }
// }


// sf::FloatRect Snake::getGlobalBounds() const {
//     if (segments.empty()) {
//         return sf::FloatRect(); // Empty rectangle if no segment
//     }

//     // Get bounds of the first segment
//     float left = segments[0]->getGlobalBounds().left;
//     float top = segments[0]->getGlobalBounds().top;
//     float right = left + segments[0]->getGlobalBounds().width;
//     float bottom = top + segments[0]->getGlobalBounds().height;

//     for (const auto& segment : segments) {
//         const sf::FloatRect& rect = segment->getGlobalBounds();
//         left = std::min(left, rect.left);
//         top = std::min(top, rect.top);
//         right = std::max(right, rect.left + rect.width);
//         bottom = std::max(bottom, rect.top + rect.height);
//     }

//     return sf::FloatRect(left, top, right - left, bottom - top);
// }

// void Snake::makeStop() {
//     isStopped = true;
//     StopTimer = DEFAULT_STOP_TIMER;
//     speed = 0;
// }

// bool Snake::isSnakeStopped() {
//     return isStopped;
// }

// float Snake::getSpeed() {
//     return this->speed;
// }

// void Snake::setSpeedPrevious(float speedPrevious) {
//     this->speedPrevious = speedPrevious;
//     this->isStopped = false;
// }

// float Snake::getSpeedPrevious() {
//     return this->speedPrevious;
// }

// void Snake::updateTimers(float elapsedTime) {
//     StopTimer -= elapsedTime;
//     if (StopTimer <= 0 && isStopped == true) {
//         isStopped = false;
//         speed = speedPrevious;
//     }
// }

#include "../include/Snake.h"

Snake::Snake(float speed, float angle, int numSegments, float segmentSize)
    : speed(speed), angle(angle), segmentSize(segmentSize),
      fading(false), fadeAlpha(255.f), fadeRate(100.f), expired(false),
      stopped(false)
{
    // Задаём идеальное расстояние между сегментами
    desiredSpacing = segmentSize * 2.0f;
    // Вычисляем вектор скорости по углу (в радианах)
    velocity.x = speed * std::cos(angle);
    velocity.y = speed * std::sin(angle);

    // Создаем голову змеи; изначально располагаем её в центре экрана (ее можно переназначить через setPosition)
    sf::CircleShape head(segmentSize);
    head.setFillColor(sf::Color::Green);
    head.setOrigin(segmentSize, segmentSize);
    head.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
    segments.push_back(head);

    // Создаем остальные сегменты и располагаем их позади головы
    for (int i = 1; i < numSegments; ++i) {
        sf::CircleShape segment(segmentSize);
        segment.setFillColor(sf::Color::Green);
        segment.setOrigin(segmentSize, segmentSize);
        // Смещение в обратном направлении движения
        sf::Vector2f offset = - (velocity / speed) * desiredSpacing * static_cast<float>(i);
        segment.setPosition(segments[0].getPosition() + offset);
        segments.push_back(segment);
    }
}

void Snake::setPosition(const sf::Vector2f& pos) {
    if (segments.empty()) return;
    segments[0].setPosition(pos);
    // Нормализуем вектор направления
    sf::Vector2f norm = (speed != 0.f) ? (velocity / speed) : sf::Vector2f(1.f, 0.f);
    for (size_t i = 1; i < segments.size(); ++i) {
        sf::Vector2f offset = -norm * desiredSpacing * static_cast<float>(i);
        segments[i].setPosition(pos + offset);
    }
}

void Snake::update(float deltaTime) {
    // Если змея не остановлена, перемещаем голову
    if (!stopped) {
        sf::Vector2f headPos = segments[0].getPosition();
        headPos += velocity * deltaTime;
        segments[0].setPosition(headPos);
    }
    // Плавное следование тела: каждый сегмент стремится сохранить расстояние desiredSpacing от предыдущего
    for (size_t i = 1; i < segments.size(); ++i) {
        sf::Vector2f prevPos = segments[i - 1].getPosition();
        sf::Vector2f currPos = segments[i].getPosition();
        sf::Vector2f diff = prevPos - currPos;
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (distance > 0.001f) {
            sf::Vector2f targetPos = prevPos - (diff / distance) * desiredSpacing;
            float smoothing = 0.2f;  // Коэффициент сглаживания (настраивается при необходимости)
            sf::Vector2f newPos = currPos + (targetPos - currPos) * smoothing;
            segments[i].setPosition(newPos);
        }
    }

    // Проверка: если все сегменты вне экрана, запускаем эффект затухания
    bool allOffScreen = true;
    for (const auto &seg : segments) {
        sf::FloatRect bounds = seg.getGlobalBounds();
        if (bounds.left + bounds.width >= 0 && bounds.left <= WINDOW_WIDTH &&
            bounds.top + bounds.height >= 0 && bounds.top <= WINDOW_HEIGHT)
        {
            allOffScreen = false;
            break;
        }
    }
    if (allOffScreen) {
        fading = true;
    }

    // Если затухание активно, уменьшаем прозрачность
    if (fading) {
        fadeAlpha -= fadeRate * deltaTime;
        if (fadeAlpha < 0.f)
            fadeAlpha = 0.f;
        for (auto &seg : segments) {
            sf::Color col = seg.getFillColor();
            col.a = static_cast<sf::Uint8>(fadeAlpha);
            seg.setFillColor(col);
        }
        if (fadeAlpha <= 0.f) {
            expired = true;
        }
    }
}

sf::FloatRect Snake::getGlobalBounds() const {
    if (segments.empty()) return sf::FloatRect();
    float left = segments[0].getPosition().x;
    float top = segments[0].getPosition().y;
    float right = left;
    float bottom = top;
    for (const auto &seg : segments) {
        sf::Vector2f pos = seg.getPosition();
        if (pos.x < left) left = pos.x;
        if (pos.x > right) right = pos.x;
        if (pos.y < top) top = pos.y;
        if (pos.y > bottom) bottom = pos.y;
    }
    return sf::FloatRect(left, top, right - left, bottom - top);
}

bool Snake::isExpired() const {
    return expired;
}

void Snake::makeStop() {
    stopped = true;
}

bool Snake::isStopped() const {
    return stopped;
}

// Метод отрисовки (вызывается при window.draw(*snake))
void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto &seg : segments) {
         target.draw(seg, states);
    }
}

