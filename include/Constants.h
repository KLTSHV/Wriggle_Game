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
const std::string POWERUP_STOP_SNAKES_LABEL = "../assets/stop_snakes_label.png";
const std::string POWERUP_SPEED_UP_LABEL = "../assets/speed_up_label.png";
const std::string POWERUP_SHRINK_LABEL = "../assets/shrink_label.png";
const std::string POWERUP_ERASE_LABEL = "../assets/erase_label.png";
const std::string POWERUP_INVINCIBILITY_LABEL = "../assets/invincibility_label.png";
const std::string BACKGROUND_AUDIO = "../assets/BackgroundAudio.mp3";

const int FRAME_RATE_LIMIT = 60;


// Константы ChangeSkin
const unsigned int CHANGESKIN_TITLE_FONT_SIZE = 40;
const unsigned int CHANGESKIN_SKIN_FONT_SIZE = 20;
const unsigned int CHANGESKIN_SELECTED_FONT_SIZE = 18;
const unsigned int CHANGESKIN_BACK_BUTTON_FONT_SIZE = 30;
const sf::Color CHANGESKIN_TITLE_COLOR = sf::Color::White;
const sf::Color CHANGESKIN_SELECTED_COLOR = sf::Color::White;
const sf::Color CHANGESKIN_BACK_BUTTON_COLOR = sf::Color::White;
constexpr float CHANGESKIN_LABEL_DISPLAY_WIDTH = 150.f;
constexpr float CHANGESKIN_LABEL_DISPLAY_HEIGHT= 150.f;
constexpr float CHANGESKIN_LABEL_SPACING= 50.f;
constexpr float CHANGESKIN_SKIN_PLACING_Y = 200.f;
constexpr float CHANGESKIN_SELECTED_OUTLINE_THICKNESS = 3.f;
const sf::Color CHANGESKIN_SELECTED_OUTLINE_COLOR = sf::Color::White;
const sf::Vector2f CHANGESKIN_TITLE_POSITION = {200, 50};
const sf::Vector2f CHANGESKIN_BACK_BUTTON_POSITION = {50, 500};
const std::vector<std::string> CHANGESKIN_SKIN_PATHS = {"../assets/skin_2.png", "../assets/skin_3.png", };


// Константы Game

constexpr int DIFFICULTY_LEVEL_UP_INTERVAL = 20;
constexpr int MAX_DIFFICULTY_LEVEL = 10;
constexpr int BORDER_DIFFICULTY_LEVEL = 5;
constexpr float DIFFICULTY_SNAKE_SPAWN_DURATION_DECREASE = 0.4f;
constexpr float DIFFICULTY_POWERUP_SPAWN_DURATION_DECREASE = 1.0f;
constexpr float BORDER_DIFFICULTY_SNAKE_SPAWN_DURATION_DECREASE = 0.1f;
constexpr float BORDER_DIFFICULTY_POWERUP_SPAWN_DURATION_DECREASE = 0.5f;

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
constexpr float PROGRESS_INCREMENT = PROGRESS_BAR_WIDTH / DIFFICULTY_LEVEL_UP_INTERVAL;
const sf::Color PROGRESS_BAR_FILL_COLOR = sf::Color::Red;
const sf::Color PROGRESS_BAR_OUTLINE_COLOR = sf::Color::White;

constexpr float POWERUP_LABEL_OFFSET_X = 40.f;
constexpr float POWERUP_LABEL_OFFSET_Y = 5.f;
constexpr float POWERUP_BAR_WIDTH = 200.f;
constexpr float POWERUP_BAR_HEIGHT = 15.f;
constexpr int POWERUP_BAR_POSITION_X = 10;
constexpr int POWERUP_BAR_POSITION_Y = 50;
constexpr float POWER_PROGRESS_BAR_OUTLINE_THICKNESS = 2.f;
// Отступ от правого верхнего угла
constexpr float POWERUP_BAR_OFFSET_X = 210.f;  
constexpr float POWERUP_BAR_OFFSET_Y = 10.f;   

const sf::Color POWERUP_BAR_FILL_COLOR   = sf::Color(255, 165, 0); 
const sf::Color POWERUP_BAR_OUTLINE_COLOR = sf::Color::White;
constexpr float POWERUP_BAR_OUTLINE_THICKNESS = 2.f;

constexpr float SNAKE_SPAWN_DURATION_INITIAL = 2.8f;
constexpr float POWER_UP_SPAWN_DURATION_INITIAL = 9.0f;
constexpr float DIAGONAL_SPEED_FACTOR = 1.0f / 1.414213562f;
constexpr float WALL_MIN_LENGTH = 100.0f;
constexpr float WALL_MAX_LENGTH = 300.0f;
constexpr float WALL_MIN_OFFSET = 50.0f;
constexpr float WALL_THICKNESS = 20.0f;
constexpr int WALL_AMOUNT_MIN = 4;
constexpr int WALL_AMOUNT_MAX = 8;

constexpr float SNAKE_SPEED_MIN = 50.0f;
constexpr float SNAKE_SPEED_MAX = 350.0f;
constexpr float SNAKE_SEGMENT_SIZE_MIN = 8.0f;
constexpr float SNAKE_SEGMENT_SIZE_MAX = 20.0f;
constexpr int SNAKE_SEGMENT_AMOUNT_MIN = 5;
constexpr int SNAKE_SEGMENT_AMOUNT_MAX = 25;


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
constexpr float INITIAL_SPEED = 100.0f;
constexpr float DASH_DISTANCE = 70.0f; 
constexpr float SPEED_UP_SPEED = 180.0f;
constexpr float SPEED_UP_DURATION = 8.0f;
constexpr float INVINCIBILITY_DURATION = 5.0f;
constexpr float SHRINK_SCALE_X = 0.02f;
constexpr float SHRINK_SCALE_Y = 0.02f;
constexpr float SHRINK_DURATION = 7.0f;
constexpr float NORMAL_SCALE_X = 0.04f;
constexpr float NORMAL_SCALE_Y = 0.04f;
constexpr float DASH_COOLDOWN = 5.0f;

// Константы PowerUp
const std::string POWERUP_TEXTURE_PATH = "../assets/powerup.png";
const float SCALE_X = 0.01f;
const float SCALE_Y = 0.01f;

// Константы Snake
const float PI = 3.14159f;
const float DEFAULT_SIZE_OF_SEGMENTS = 10.0f;
const float DEFAULT_STOP_TIMER = 6.0f;
const float SNAKE_FADE_RATE = 100.f;


// Константы Statistics
const std::string STAT_FILE_PATH = "../statistics.txt";
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
