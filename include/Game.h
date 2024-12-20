#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Player.h"  
#include "Snake.h"
#include "Wall.h"
#include "Menu.h"
#include "PowerUp.h" 

class Player;
class Snake;
class PowerUp;
class Menu;
class Wall;

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
    void handlePlayerMovement();
    void adjustDifficultyLevel();
    void spawnWall();
    void handleWallCollisions();
    
    sf::Music backgroundMusic;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Snake> > snakes;
    std::vector<std::unique_ptr<Wall> > walls;
    std::vector<std::unique_ptr<PowerUp> > powerUps;
    std::vector<float> speedOfStoppedSnakes;
    struct stats{
        int gamesPlayed;
        int highScore;
        int totalTime; 
    };
    
    bool erasingSnakesProcess = false;

    sf::Font font;
    sf::Text timerText;
    float elapsedGameTime;
    sf::RectangleShape progressBar;
    float progressBarFill;
    sf::RectangleShape progressBarOutline;

    float dashTimer;
    float snakeSpawnTimer;
    float powerUpSpawnTimer;
    float wallSpawnTimer;

    float difficultyLevelTimer;
    int difficultyLevel;
    float wallSpawnDuration = 5.0f;
    bool isSnakesStopped = false;
    float snakeSpawnDuration;
    float powerUpSpawnDuration;
    
};

#endif // GAME_H
