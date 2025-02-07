#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "Player.h"
#include "Snake.h"
#include "Wall.h"
#include "Menu.h"
#include "PowerUp.h"

class Game {
public:
    Game();
    void run();
    void setGameRunning(bool value) {
        isGameRunning = value;
    }
    bool startTheGame = false;
    sf::Clock gameClock;
    std::unique_ptr<Menu> menu; 
    sf::RenderWindow window;
    void resetGame();
    bool isGameRunning = false;

private:
    void processEvents();
    void update();
    void render();
    void handlePlayerInput(sf::Keyboard::Key key);
    void saveStatistics(const std::string& fileName);
    void spawnSnake();
    void spawnPowerUp();
    void handleCollisions();
    void activatePowerUp(PowerUp& powerUp);
    void handlePlayerMovement(float deltaTime);
    void adjustDifficultyLevel();
    void spawnWall();
    void handleWallCollisions();
    
    // Музыка, игрок, змеии, стены, бонусы...
    sf::Music backgroundMusic;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Snake>> snakes;
    std::vector<std::unique_ptr<Wall>> walls;
    std::vector<std::unique_ptr<PowerUp>> powerUps;
    std::vector<float> speedOfStoppedSnakes;
    bool erasingSnakesProcess = false;

    // Шрифт и текст таймера
    sf::Font font;
    sf::Text timerText;
    float elapsedGameTime;

    // --- 1) "Красный" прогресс-бар (у вас уже был) ---
    sf::RectangleShape progressBar;
    float progressBarFill;
    sf::RectangleShape progressBarOutline;

    // --- 2) "Оранжевый" прогресс-бар для усилений ---
    sf::RectangleShape powerProgressBar;
    sf::RectangleShape powerProgressBarOutline;
    bool  powerUpBarActive      = false;  // Отображается ли сейчас полоса
    float powerUpTimeRemaining  = 0.f;    // Сколько секунд осталось бонусу
    float powerUpTimeMax        = 0.f;    // Максимальное время этого бонуса

    // --- 3) Иконка усиления, выводимая возле оранжевой полосы ---
    sf::Texture powerUpIconTexture;
    sf::Sprite  powerUpIconSprite;
    bool  powerUpIconActive      = false; // Показать ли иконку сейчас
    float powerUpIconTimeRemaining = 0.f; // Сколько секунд показывать иконку
    // Если у бонуса есть продолжительность (barActive), 
    // иконка показывается столько же, сколько длится бонус. 
    // Если бонус моментальный (erase), показываем иконку на короткий промежуток

    // Таймеры
    float dashTimer;
    float snakeSpawnTimer;
    float powerUpSpawnTimer;
    float wallSpawnTimer;
    float difficultyLevelTimer;
    int   difficultyLevel;
    float wallSpawnDuration = 5.0f;
    bool  isSnakesStopped   = false;
    float snakeSpawnDuration;
    float powerUpSpawnDuration;
    float POWER_PROGRESS_INCREMENT;
};

#endif // GAME_H