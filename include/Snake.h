#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Snake : public sf::Drawable {
public:
    Snake();
    void setPosition(float x, float y); 
    void setAngle(float angle);        
    void setSpeed(float speed);
    void setSpeedPrevious(float speedPrevious);         
    void setAmountOfSegments(int amount);
    void setSizeOfSegments(float size);
    float getSpeed(); 
    float getSpeedPrevious();
    bool isSnakeStopped();
    void grow();
    void setScale(float x, float y);                       
    void move();                       
    void makeStop();
    void updateTimers(float elapsedTime);
    sf::FloatRect getGlobalBounds() const; 
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    
    ~Snake() = default; //Дописать, мб из-за этого ошибка

    int amountOfSegments;
    float sizeOfSegments;
    std::vector<std::unique_ptr<sf::CircleShape> > segments;
    

private:
 
    float speed;   
    float speedPrevious;
    float angle;   // Угол движения в градусах
    sf::Vector2f velocity;
    bool isStopped = false;
    float StopTimer = 0;

};

#endif // SNAKE_H
