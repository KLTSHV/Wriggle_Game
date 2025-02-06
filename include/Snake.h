// #ifndef SNAKE_H
// #define SNAKE_H

// #include <SFML/Graphics.hpp>
// #include <vector>
// #include <memory>

// class Snake : public sf::Drawable {
// public:
//     Snake();
//     void setPosition(float x, float y); 
//     void setAngle(float angle);        
//     void setSpeed(float speed);
//     void setSpeedPrevious(float speedPrevious);         
//     void setAmountOfSegments(int amount);
//     void setSizeOfSegments(float size);
//     float getSpeed(); 
//     float getSpeedPrevious();
//     bool isSnakeStopped();
//     void grow();
//     void setScale(float x, float y);                       
//     void move(float deltaTime);                       
//     void makeStop();
//     void updateTimers(float elapsedTime);
//     sf::FloatRect getGlobalBounds() const; 
//     virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
//     ~Snake() = default; //Дописать, мб из-за этого ошибка
//     int amountOfSegments;
//     float sizeOfSegments;
//     std::vector<std::unique_ptr<sf::CircleShape> > segments;
// private:
//     float speed;   
//     float speedPrevious;
//     float angle;   // Угол движения в градусах
//     sf::Vector2f velocity;
//     bool isStopped = false;
//     float StopTimer = 0;
// };

// #endif // SNAKE_H

#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "../include/Constants.h"  // Убедитесь, что здесь определены WINDOW_WIDTH, WINDOW_HEIGHT, SNAKE_SPEED_MIN/MAX, PI и пр.

// Класс Snake наследуется от sf::Drawable для возможности вызова window.draw(*snake)
class Snake : public sf::Drawable {
public:
    // Конструктор принимает:
    //   speed       — скорость движения головы,
    //   angle       — направление движения (в радианах),
    //   numSegments — общее число сегментов (включая голову),
    //   segmentSize — размер (радиус) сегмента.
    Snake(float speed, float angle, int numSegments, float segmentSize);

    // Устанавливает позицию головы и выстраивает тело змеи позади неё
    void setPosition(const sf::Vector2f& pos);

    // Обновляет положение змеи (движение головы и плавное следование тела)
    // deltaTime — время, прошедшее с прошлого кадра
    void update(float deltaTime);

    // Возвращает объединённые границы всех сегментов (например, для столкновений)
    sf::FloatRect getGlobalBounds() const;

    // Возвращает true, если змея полностью исчезла (например, после затухания)
    bool isExpired() const;

    // Останавливает змею (например, при активации бонуса)
    void makeStop();

    // Возвращает true, если змея остановлена
    bool isStopped() const;

protected:
    // Переопределяем метод отрисовки (вызывается при window.draw(*snake))
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    float speed;          // Скорость движения головы
    float angle;          // Направление движения (в радианах)
    sf::Vector2f velocity; // Вектор скорости (speed * [cos(angle), sin(angle)])
    float segmentSize;     // Размер (радиус) сегмента
    float desiredSpacing;  // Идеальное расстояние между сегментами (например, 2 * segmentSize)
    std::vector<sf::CircleShape> segments; // Сегменты змеи (голова — первый элемент)

    // Параметры для эффекта затухания, когда змея уходит за пределы экрана
    bool fading;      // Флаг, включен ли процесс затухания
    float fadeAlpha;  // Текущая непрозрачность (255 = полностью видимо, 0 = полностью прозрачно)
    float fadeRate;   // Скорость уменьшения alpha (единиц в секунду)
    bool expired;     // True, если змея полностью исчезла

    // Флаг, показывающий, что змея остановлена
    bool stopped;
};

#endif // SNAKE_H
