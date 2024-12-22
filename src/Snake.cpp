#include "../include/Snake.h"
#include <cmath>
#include <memory>

// Constants
const float PI = 3.14159f;
const float DEFAULT_SIZE_OF_SEGMENTS = 10.0f;
const float DEFAULT_STOP_TIMER = 6.0f;

Snake::Snake() 
    : speed(0), 
      angle(0), 
      velocity(0, 0), 
      amountOfSegments(0), 
      sizeOfSegments(DEFAULT_SIZE_OF_SEGMENTS) {
    
    segments.push_back(std::make_unique<sf::CircleShape>(sizeOfSegments));
    segments.back()->setFillColor(sf::Color::Green);
}

void Snake::setPosition(float x, float y) {
    if (!segments.empty()) {
        segments[0]->setPosition(x, y);  
    }
}

void Snake::setAngle(float angle) {
    this->angle = angle;
    velocity.x = speed * std::cos(angle * PI / 180.0f);
    velocity.y = speed * std::sin(angle * PI / 180.0f);
}

void Snake::setSpeed(float speed) {
    this->speed = speed;
    velocity.x = speed * std::cos(angle * PI / 180.0f);
    velocity.y = speed * std::sin(angle * PI / 180.0f);
    isStopped = false;
}

void Snake::setAmountOfSegments(int amount) {
    this->amountOfSegments = amount;
}

void Snake::setSizeOfSegments(float size) {
    this->sizeOfSegments = size;
}

void Snake::grow() {
    if (segments.empty()) return;

    // Get the position of the last segment
    sf::Vector2f lastSegmentPosition = segments.back()->getPosition();

    // Create new segments behind the head
    for (int i = 0; i < amountOfSegments; ++i) {
        
        auto newSegment = std::make_unique<sf::CircleShape>(sizeOfSegments);
        newSegment->setFillColor(sf::Color::Green);

        lastSegmentPosition.x -= sizeOfSegments * std::cos(angle);
        lastSegmentPosition.y -= sizeOfSegments * std::sin(angle);
        newSegment->setPosition(lastSegmentPosition);

        segments.push_back(std::move(newSegment));
    }
}

void Snake::move() {
    if (segments.empty()) return;

    // Head speed
    float xOffset = speed * std::cos(angle);
    float yOffset = speed * std::sin(angle);

    // Move the head
    sf::Vector2f previousPosition = segments[0]->getPosition();
    segments[0]->move(xOffset, yOffset);

    // Move the body, segment by segment
    for (size_t i = 1; i < segments.size(); ++i) {
        sf::Vector2f tempPosition = segments[i]->getPosition();
        segments[i]->setPosition(previousPosition);
        previousPosition = tempPosition;
    }
}

sf::FloatRect Snake::getGlobalBounds() const {
    if (segments.empty()) {
        return sf::FloatRect(); // Empty rectangle if no segment
    }

    // Get bounds of the first segment
    float left = segments[0]->getGlobalBounds().left;
    float top = segments[0]->getGlobalBounds().top;
    float right = left + segments[0]->getGlobalBounds().width;
    float bottom = top + segments[0]->getGlobalBounds().height;

    for (const auto& segment : segments) {
        const sf::FloatRect& rect = segment->getGlobalBounds();
        left = std::min(left, rect.left);
        top = std::min(top, rect.top);
        right = std::max(right, rect.left + rect.width);
        bottom = std::max(bottom, rect.top + rect.height);
    }

    return sf::FloatRect(left, top, right - left, bottom - top);
}

void Snake::makeStop() {
    isStopped = true;
    StopTimer = DEFAULT_STOP_TIMER;
    speed = 0;
}

bool Snake::isSnakeStopped() {
    return isStopped;
}

float Snake::getSpeed() {
    return this->speed;
}

void Snake::setSpeedPrevious(float speedPrevious) {
    this->speedPrevious = speedPrevious;
    this->isStopped = false;
}

float Snake::getSpeedPrevious() {
    return this->speedPrevious;
}

void Snake::updateTimers(float elapsedTime) {
    StopTimer -= elapsedTime;
    if (StopTimer <= 0 && isStopped == true) {
        isStopped = false;
        speed = speedPrevious;
    }
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& segment : segments) {
        sf::RenderStates segmentStates = states;  // Copy current states

        segmentStates.transform.translate(segment->getPosition()); // Translate based on segment position

        target.draw(*segment, segmentStates);  // Draw the segment with its individual states
    }
}
