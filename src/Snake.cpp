#include "../include/Snake.h"
#include <cmath>
#include <iostream>

Snake::Snake(float speed, float angle, int numSegments, float segmentSize)
    : speed(speed), angle(angle), segmentSize(segmentSize),
      fading(false), fadeAlpha(255.f), fadeRate(100.f),
      expired(false), stopped(false),
      totalSegments(numSegments), spawnTimer(0.f)
{
    desiredSpacing = segmentSize;
    velocity.x = speed * std::cos(angle);
    velocity.y = speed * std::sin(angle);
    segmentSpawnInterval = desiredSpacing / speed;

    // Создаём только "голову"
    sf::CircleShape head(segmentSize);
    head.setFillColor(sf::Color::Green);
    head.setOrigin(segmentSize, segmentSize);
    head.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
    segments.push_back(head);
}

void Snake::setPosition(const sf::Vector2f &pos) {
    if (!segments.empty()) {
        segments[0].setPosition(pos);
    }
}

void Snake::update(float deltaTime) {
    // Если не остановлены, двигаем "голову"
    if (!stopped) {
        sf::Vector2f headPos = segments[0].getPosition();
        headPos += velocity * deltaTime;
        segments[0].setPosition(headPos);
    }

    // Сегменты следуют друг за другом
    for (size_t i = 1; i < segments.size(); ++i) {
        sf::Vector2f prevPos = segments[i - 1].getPosition();
        sf::Vector2f currPos = segments[i].getPosition();

        sf::Vector2f diff = prevPos - currPos;
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (distance > 0.001f) {
            sf::Vector2f targetPos = prevPos - (diff / distance) * desiredSpacing;
            float smoothing = 0.2f;
            sf::Vector2f newPos = currPos + (targetPos - currPos) * smoothing;
            segments[i].setPosition(newPos);
        }
    }

    // Постепенно добавляем сегменты, пока их число < totalSegments
    if ((int)segments.size() < totalSegments) {
        spawnTimer += deltaTime;
        if (spawnTimer >= segmentSpawnInterval) {
            sf::Vector2f norm = velocity / speed;
            sf::Vector2f lastPos = segments.back().getPosition();
            sf::Vector2f newPos = lastPos - norm * desiredSpacing;

            sf::CircleShape newSegment(segmentSize);
            newSegment.setFillColor(sf::Color::Green);
            newSegment.setOrigin(segmentSize, segmentSize);
            newSegment.setPosition(newPos);
            segments.push_back(newSegment);

            spawnTimer -= segmentSpawnInterval;
        }
    }

    // Если вся змея вышла за экран, начинаем тухнуть
    bool allOffScreen = true;
    for (auto &seg : segments) {
        sf::FloatRect b = seg.getGlobalBounds();
        if (b.left + b.width >= 0 && b.left <= WINDOW_WIDTH &&
            b.top + b.height >= 0 && b.top <= WINDOW_HEIGHT)
        {
            allOffScreen = false;
            break;
        }
    }
    if (allOffScreen) {
        fading = true;
    }

    if (fading) {
        fadeAlpha -= fadeRate * deltaTime;
        if (fadeAlpha < 0.f) fadeAlpha = 0.f;

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

void Snake::makeStop() {
    stopped = true;
}

void Snake::makeResume() {
    stopped = false;
}

bool Snake::isExpired() const {
    return expired;
}

bool Snake::isStopped() const {
    return stopped;
}

sf::FloatRect Snake::getGlobalBounds() const {
    if (segments.empty()) {
        return sf::FloatRect();
    }
    float left   = std::numeric_limits<float>::max();
    float right  = std::numeric_limits<float>::lowest();
    float top    = std::numeric_limits<float>::max();
    float bottom = std::numeric_limits<float>::lowest();

    for (auto &seg : segments) {
        auto bounds = seg.getGlobalBounds();
        if (bounds.left < left)                 left = bounds.left;
        if (bounds.top < top)                   top = bounds.top;
        if ((bounds.left + bounds.width) > right)   right = bounds.left + bounds.width;
        if ((bounds.top + bounds.height) > bottom)  bottom = bounds.top + bounds.height;
    }
    return sf::FloatRect(left, top, right - left, bottom - top);
}

void Snake::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &seg : segments) {
        target.draw(seg, states);
    }
}

const std::vector<sf::CircleShape>& Snake::getSegments() const {
    return segments;
}