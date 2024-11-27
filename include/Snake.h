#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Snake : public sf::Drawable {
public:
    Snake();
    void setPosition(float x, float y); // Set the position of the head
    void setAngle(float angle);        // Set the movement angle
    void setSpeed(float speed);
    void setSpeedPrevious(float speedPrevious);         // Set the movement speed
    void setAmountOfSegments(int amount);
    void setSizeOfSegments(float size);
    float getSpeed(); 
    float getSpeedPrevious();
    bool isSnakeStopped();
    void grow();
    void setScale(float x, float y);                       // Increase the length of the snake
    void move();                       // Handle movement
    void makeStop();
    void updateTimers(float elapsedTime);
    sf::FloatRect getGlobalBounds() const; // Get the bounds of the snake
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // Destructor (not strictly necessary for unique_ptr, but good practice if you want to explicitly clear resources)
    ~Snake() = default;

    int amountOfSegments;
    float sizeOfSegments;
    std::vector<std::unique_ptr<sf::CircleShape> > segments;
    

private:
 // Store unique pointers to the segments of the snake
    float speed;   // Movement speed
    float speedPrevious;
    float angle;   // Movement angle in degrees
    sf::Vector2f velocity; // Precomputed velocity for straight movement
    bool isStopped = false;
    float StopTimer = 0;

};

#endif // SNAKE_H
