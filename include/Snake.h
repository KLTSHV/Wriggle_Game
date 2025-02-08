

#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "../include/Constants.h" 

class Snake : public sf::Drawable {
public:
    // Конструктор:
    //   speed       — скорость головы (в диапазоне SNAKE_SPEED_MIN–SNAKE_SPEED_MAX),
    //   angle       — направление движения (в радианах),
    //   numSegments — общее число сегментов (голова + последующие),
    //   segmentSize — размер сегмента.
    Snake(float speed, float angle, int numSegments, float segmentSize);

    // Устанавливает положение головы змеи.
    // Обычно вызывается сразу после создания, чтобы поместить голову на краю карты.
    void setPosition(const sf::Vector2f &pos);

    // Обновляет движение змеи:
    // – перемещает голову (если не остановлена),
    // – каждые segmentSpawnInterval секунд добавляет новый сегмент (если их меньше, чем totalSegments),
    // – обновляет плавное следование уже появившихся сегментов.
    void update(float deltaTime);

    // Возвращает уменьшённый хитбокс для столкновений.
    // Используется хитбокс головы, уменьшенный на коэффициент shrinkFactor.
    sf::FloatRect getGlobalBounds() const;

    // Возвращает true, если змея полностью исчезла.
    bool isExpired() const;

    bool isStopped() const;
    void makeStop();
    void makeResume();
    void updateTimers(float elapsedTime);

    const std::vector<sf::CircleShape>& getSegments() const;

protected:

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    float speed;           // Скорость головы.
    float angle;           // Направление движения (в радианах).
    sf::Vector2f velocity; // Вектор скорости = speed * [cos(angle), sin(angle)].
    float segmentSize;     // Радиус сегмента.
    float desiredSpacing;  // Идеальное расстояние между сегментами (например, 2 * segmentSize).

    std::vector<sf::CircleShape> segments; // Контейнер сегментов (голова — первый элемент).

    int totalSegments;       // Общее желаемое число сегментов (передаётся в конструкторе).
    float spawnTimer;        // Таймер для появления нового сегмента.
    float segmentSpawnInterval; // Интервал появления сегмента = desiredSpacing / speed.

    // Параметры затухания (если змея уходит за пределы экрана):
    bool fading;       // Флаг, включен ли эффект затухания.
    float fadeAlpha;   // Текущая непрозрачность (255 = полностью видимый, 0 = прозрачный).
    float fadeRate;    // Скорость уменьшения alpha (например, 100.f единиц/сек).
    bool expired;      // True, если змея полностью исчезла.

    // Флаг остановки змеи.
    bool stopped;
    float stoppedTimer;
};

#endif // SNAKE_H
