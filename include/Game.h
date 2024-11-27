#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "Player.h"  // Include the full definition
#include "Snake.h"
#include "Wall.h"
#include "Menu.h"
#include "PowerUp.h" // Ensure this is included before using std::unique_ptr<PowerUp>


// Forward declaration of Player
class Player;
class Snake;
class PowerUp;
class Menu;
class Wall;

class Game {
public:
    Game();
    void run();
    
private:
    void processEvents();
    void update();
    void render();
    
    void resetGame();
    void spawnSnake();
    void spawnPowerUp();
    void handleCollisions();
    void activatePowerUp(PowerUp& powerUp);
    void handlePlayerMovement();
    void adjustDifficultyLevel();
    void spawnWall();
    void handleWallCollisions();

    sf::RenderWindow window;
    std::unique_ptr<Player> player;
    std::unique_ptr<Menu> menu;
    std::vector<std::unique_ptr<Snake> > snakes;
    std::vector<std::unique_ptr<Wall> > walls;
    std::vector<std::unique_ptr<PowerUp> > powerUps;
    std::vector<float> speedOfStoppedSnakes;
    
    bool erasingSnakesProcess = false;

    sf::Clock gameClock;
    float dashTimer;
    float snakeSpawnTimer;
    float powerUpSpawnTimer;
    float wallSpawnTimer;
    bool isGameRunning;
    float difficultyLevelTimer;
    int difficultyLevel;
    float wallSpawnDuration = 5.0f;
    bool isSnakesStopped = false;
    float snakeSpawnDuration;
    float powerUpSpawnDuration;
};

#endif // GAME_H
