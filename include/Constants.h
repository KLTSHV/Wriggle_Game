#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>
#include <iostream>
#include <cmath>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

const std::string FONT_PATH = "../assets/arial.ttf";

// Константы ChangeSkin
const unsigned int CHANGESKIN_TITLE_FONT_SIZE = 40;
const unsigned int CHANGESKIN_BACK_BUTTON_FONT_SIZE = 30;
const sf::Color CHANGESKIN_TITLE_COLOR = sf::Color::White;
const sf::Color CHANGESKIN_BACK_BUTTON_COLOR = sf::Color::White;
const sf::Vector2f CHANGESKIN_TITLE_POSITION = {200, 50};
const sf::Vector2f CHANGESKIN_BACK_BUTTON_POSITION = {50, 500};
const std::vector<std::string> CHANGESKIN_SKIN_PATHS = {"../assets/powerup.png", "../assets/player.png"};

// Константы Game
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int FONT_SIZE = 24;
constexpr int PLAYER_POSITION_X = 400;
constexpr int PLAYER_POSITION_Y = 300;
constexpr int TIMERTEXT_POSITION_X = 10;
constexpr int TIMERTEXT_POSITION_Y = 10;
constexpr int PROGRESSBAR_POSITION_X = 10;
constexpr int PROGRESSBAR_POSITION_Y = 50;
constexpr float PROGRESS_BAR_WIDTH = 200.f;
constexpr float PROGRESS_BAR_HEIGHT = 15.f;
constexpr float PROGRESS_BAR_OUTLINE_THICKNESS = 2.f;
constexpr float PROGRESS_INCREMENT = PROGRESS_BAR_WIDTH / 30.f;
constexpr float SNAKE_SPAWN_DURATION_INITIAL = 3.0f;
constexpr float POWER_UP_SPAWN_DURATION_INITIAL = 9.0f;
constexpr float DIAGONAL_SPEED_FACTOR = 1.0f / 1.414213562f;
constexpr float WALL_MIN_LENGTH = 100.0f;
constexpr float WALL_MAX_LENGTH = 300.0f;
constexpr float WALL_MIN_OFFSET = 50.0f;
constexpr int DIFFICULTY_LEVEL_UP_INTERVAL = 30;
constexpr int MAX_DIFFICULTY_LEVEL = 5;
constexpr float SNAKE_SPEED_MIN = 0.05f;
constexpr float SNAKE_SPEED_MAX = 0.5f;
constexpr float SNAKE_SEGMENT_SIZE_MIN = 8.0f;
constexpr float SNAKE_SEGMENT_SIZE_MAX = 20.0f;

// Константы Menu
constexpr int TITLE_FONT_SIZE = 50;
constexpr int OPTION_FONT_SIZE = 30;
inline const sf::Color TEXT_COLOR = sf::Color::White;
constexpr float TITLE_X = 250.0f;
constexpr float TITLE_Y = 100.0f;
constexpr float START_X = 250.0f;
constexpr float START_Y = 250.0f;
constexpr float CHANGE_SKIN_X = 250.0f;
constexpr float CHANGE_SKIN_Y = 300.0f;
constexpr float STATISTICS_X = 250.0f;
constexpr float STATISTICS_Y = 350.0f;
constexpr float EXIT_X = 250.0f;
constexpr float EXIT_Y = 400.0f;

// Константы Player
constexpr float INITIAL_SPEED = 8.0f;
constexpr float DASH_DISTANCE = 10.0f; 
constexpr float SPEED_UP_SPEED = 15.0f;
constexpr float SPEED_UP_DURATION = 7.0f;
constexpr float INVINCIBILITY_DURATION = 5.0f;
constexpr float SHRINK_SCALE_X = 0.01f;
constexpr float SHRINK_SCALE_Y = 0.01f;
constexpr float SHRINK_DURATION = 7.0f;
constexpr float NORMAL_SCALE_X = 0.02f;
constexpr float NORMAL_SCALE_Y = 0.02f;
constexpr float DASH_COOLDOWN = 6.0f;

// Константы PowerUp
const std::string POWERUP_TEXTURE_PATH = "assets/powerup.png";
const float SCALE_X = 0.01f;
const float SCALE_Y = 0.01f;

// Константы Snake
const float PI = 3.14159f;
const float DEFAULT_SIZE_OF_SEGMENTS = 10.0f;
const float DEFAULT_STOP_TIMER = 6.0f;

// Константы Statistics
const std::string STAT_FILE_PATH = "statistics.txt";
const int STAT_TITLE_FONT_SIZE = 40;
const int STAT_BUTTON_FONT_SIZE = 30;
const int STAT_TEXT_FONT_SIZE = 30;
const sf::Color STAT_TEXT_COLOR = sf::Color::White;
const sf::Vector2f STAT_TITLE_POSITION(200, 50);
const sf::Vector2f STAT_BACK_BUTTON_POSITION(50, 500);
const sf::Vector2f STAT_TEXT_POSITION(200, 150);
const int STAT_TEXT_Y_INCREMENT = 50;
const int DEFAULT_HIGH_SCORE = 1234;
const int DEFAULT_GAMES_PLAYED = 56;
const int DEFAULT_TOTAL_TIME = 12345;

#endif // CONSTANTS_H
