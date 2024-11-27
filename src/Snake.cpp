#include "/Users/egorkoltysev/Desktop/PROG/Wriggle/include/Snake.h"
#include <cmath>
#include <memory>

Snake::Snake() 
    : speed(0), 
      angle(0), 
      velocity(0, 0), 
      amountOfSegments(0), 
      sizeOfSegments(10.0f) {
    // Add the head segment as a unique_ptr
    segments.push_back(std::make_unique<sf::CircleShape>(sizeOfSegments));
    segments.back()->setFillColor(sf::Color::Green);
}

void Snake::setPosition(float x, float y) {
    if (!segments.empty()) {
        segments[0]->setPosition(x, y);  // Use -> to dereference the unique_ptr
    }
}

void Snake::setAngle(float angle) {
    this->angle = angle;
    velocity.x = speed * std::cos(angle * 3.14159f / 180.0f);
    velocity.y = speed * std::sin(angle * 3.14159f / 180.0f);
}

void Snake::setSpeed(float speed) {
    this->speed = speed;
    velocity.x = speed * std::cos(angle * 3.14159f / 180.0f);
    velocity.y = speed * std::sin(angle * 3.14159f / 180.0f);
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

    // Create new segments behind the last one
    for (int i = 0; i < amountOfSegments; ++i) {
        // Create a new unique_ptr for each segment
        auto newSegment = std::make_unique<sf::CircleShape>(sizeOfSegments);
        newSegment->setFillColor(sf::Color::Green);

        // Adjust the position of the new segment based on the direction of movement
        lastSegmentPosition.x -= sizeOfSegments * std::cos(angle);
        lastSegmentPosition.y -= sizeOfSegments * std::sin(angle);
        newSegment->setPosition(lastSegmentPosition);

        // Push the new segment to the vector
        segments.push_back(std::move(newSegment));
    }
}

void Snake::move() {
    if (segments.empty()) return;

    // Скорость головы
    float xOffset = speed * std::cos(angle);
    float yOffset = speed * std::sin(angle);

    // перемещаю голову
    sf::Vector2f previousPosition = segments[0]->getPosition();
    segments[0]->move(xOffset, yOffset);

    // Движение змейки, сегмента за сегментом
    for (size_t i = 1; i < segments.size(); ++i) {
        sf::Vector2f tempPosition = segments[i]->getPosition();
        segments[i]->setPosition(previousPosition);
        previousPosition = tempPosition;
    }
}

sf::FloatRect Snake::getGlobalBounds() const {
    if (segments.empty()) {
        return sf::FloatRect(); // Если нет сегмента то пустой квадрат
    }

    // Границы первого сегмента
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

void Snake::makeStop(){

    isStopped = true;
    StopTimer = 6.0f;
    speed = 0;
    
}

bool Snake::isSnakeStopped(){
    return isStopped;
}

float Snake::getSpeed(){
    return this->speed;
}
void Snake::setSpeedPrevious(float speedPrevious){
    this->speedPrevious = speedPrevious;
    this->isStopped = false;
}
float Snake::getSpeedPrevious(){
    return this->speedPrevious;
}
void Snake::updateTimers(float elapsedTime){
    StopTimer-=elapsedTime;
    if (StopTimer <= 0 && isStopped == true){
        isStopped = false;
        speed = speedPrevious;
    }
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& segment : segments) {
        sf::RenderStates segmentStates = states;  // копия текущих состояний

        segmentStates.transform.translate(segment->getPosition()); // перемещение по позиции сегмента

        target.draw(*segment, segmentStates);  // Отрисовывка сегмента с индивидуальными состояниями
    }
}
