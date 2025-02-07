
// #include "../include/Game.h"
// #include <iostream>
// #include <cmath>
// #include <random>
// #include <fstream>
// #include "../include/PowerUp.h"  
// #include "../include/Player.h"
// #include "../include/Constants.h"

// Game::Game() 
//     : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arcade Game"), 
//       snakeSpawnTimer(0), 
//       powerUpSpawnTimer(0), 
//       isGameRunning(false), 
//       difficultyLevelTimer(0.0f), 
//       difficultyLevel(0), 
//       gameClock(), 
//       wallSpawnTimer(0)
// {
//     std::srand(static_cast<unsigned int>(std::time(nullptr)));
//     menu = std::make_unique<Menu>();
//     startTheGame = menu->showWelcomeScreen(window, *this);

//     player = std::make_unique<Player>(); 
//     player->setPosition(PLAYER_POSITION_X, PLAYER_POSITION_Y);

//     font.loadFromFile("../assets/arial.ttf");  
//     timerText.setFont(font);
//     timerText.setCharacterSize(FONT_SIZE);
//     timerText.setFillColor(sf::Color::White);
//     timerText.setPosition(TIMERTEXT_POSITION_X, TIMERTEXT_POSITION_Y);

//     if (!backgroundMusic.openFromFile("../assets/BackgroundAudio.mp3")) {
//         std::cout << "Error: Failed to load background music!" << std::endl;
//     } else {
//         backgroundMusic.setLoop(true);  
//         backgroundMusic.play();         
//     }

//     // --- Изначальный (красный) прогресс-бар ---
//     progressBar.setSize(sf::Vector2f(PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT));
//     progressBar.setFillColor(sf::Color::Red);
//     progressBar.setPosition(PROGRESSBAR_POSITION_X, PROGRESSBAR_POSITION_Y);

//     progressBarOutline.setSize(sf::Vector2f(PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT));
//     progressBarOutline.setOutlineThickness(PROGRESS_BAR_OUTLINE_THICKNESS);
//     progressBarOutline.setOutlineColor(sf::Color::White);
//     progressBarOutline.setFillColor(sf::Color::Transparent);
//     progressBarOutline.setPosition(progressBar.getPosition());
    
//     // --- Новая полоса для бонусов (оранжевая), по умолчанию неактивна ---
//     powerProgressBar.setSize(sf::Vector2f(POWERUP_BAR_WIDTH, POWERUP_BAR_HEIGHT));
//     powerProgressBar.setFillColor(POWERUP_BAR_FILL_COLOR);
//     // Ставим в правый верх с небольшим отступом:
//     powerProgressBar.setPosition(
//         WINDOW_WIDTH - POWERUP_BAR_OFFSET_X,
//         POWERUP_BAR_OFFSET_Y
//     );
    
//     powerProgressBarOutline.setSize(sf::Vector2f(POWERUP_BAR_WIDTH, POWERUP_BAR_HEIGHT));
//     powerProgressBarOutline.setOutlineThickness(POWERUP_BAR_OUTLINE_THICKNESS);
//     powerProgressBarOutline.setOutlineColor(POWERUP_BAR_OUTLINE_COLOR);
//     powerProgressBarOutline.setFillColor(sf::Color::Transparent);
//     powerProgressBarOutline.setPosition(powerProgressBar.getPosition());

//     elapsedGameTime = 0.f;
//     progressBarFill = 0.f;

//     // Для старого powerProgressBarFill можно удалить
//     // или инициализировать нулём (больше не используется)
//     // powerProgressBarFill = 0.f; 

//     snakeSpawnDuration = SNAKE_SPAWN_DURATION_INITIAL;
//     powerUpSpawnDuration = POWER_UP_SPAWN_DURATION_INITIAL;
// }

// void Game::run() {
//     window.setFramerateLimit(60);
//     while (window.isOpen()) {
//         if (isGameRunning && startTheGame) {
//             update();
//             render();
//             processEvents();
//         } 
//     }
// }

// void Game::processEvents() {
//     sf::Event event;
//     while (window.pollEvent(event)) {
//         if (event.type == sf::Event::Closed) {
//             window.close();
//         }
//         // Обработка дэша
//         if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LShift) {
//             int dashDirection = -1;
//             bool up    = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
//             bool down  = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
//             bool left  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
//             bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
            
//             if (up && !down && !left && !right)
//                 dashDirection = 0; // Up
//             else if (up && right)
//                 dashDirection = 1; // Up-Right
//             else if (right && !up && !down)
//                 dashDirection = 2; // Right
//             else if (down && right)
//                 dashDirection = 3; // Down-Right
//             else if (down && !up && !left && !right)
//                 dashDirection = 4; // Down
//             else if (down && left)
//                 dashDirection = 5; // Down-Left
//             else if (left && !up && !down)
//                 dashDirection = 6; // Left
//             else if (up && left)
//                 dashDirection = 7; // Up-Left
            
//             if (dashDirection == -1)
//                 dashDirection = 0; // Значение по умолчанию (Up)
            
//             player->dash(dashDirection, walls);
//         }
//     }
// }

// void Game::update() {
//     float deltaTime = gameClock.restart().asSeconds();

//     dashTimer += deltaTime;
//     snakeSpawnTimer += deltaTime;
//     powerUpSpawnTimer += deltaTime;
//     difficultyLevelTimer += deltaTime;

//     player->updateTimers(deltaTime);

//     // Обновляем змей (если они не удалены)
//     if (!erasingSnakesProcess && !snakes.empty()) {
//         for (auto& snake : snakes) {
//             if (snake) {
//                 snake->update(deltaTime);
//                 isSnakesStopped = snake->isStopped();
//             }
//         }
//     }

//     elapsedGameTime += deltaTime;
//     timerText.setString("Time: " + std::to_string(static_cast<int>(elapsedGameTime)) + "s");

//     // -- Красный прогресс-бар (как у вас было) --
//     progressBarFill += deltaTime * PROGRESS_INCREMENT;
//     if (progressBarFill >= PROGRESS_BAR_WIDTH) {
//         progressBarFill = 0.f;
//     }
//     progressBar.setSize(sf::Vector2f(progressBarFill, progressBar.getSize().y));

//     // -- Логика для оранжевого бара: «убывание» оставшегося времени бонуса --
//     if (powerUpBarActive) {
//         powerUpTimeRemaining -= deltaTime;
//         if (powerUpTimeRemaining <= 0.f) {
//             // Бонус истёк
//             powerUpTimeRemaining = 0.f;
//             powerUpBarActive = false;
//             // При необходимости здесь можно принудительно «отключить» эффект
//             // если он не отключается сам внутри Player::updateTimers(...)
//         } else {
//             // Пересчёт ширины пропорционально оставшемуся времени
//             float ratio = powerUpTimeRemaining / powerUpTimeMax;
//             float newWidth = ratio * POWERUP_BAR_WIDTH;
//             powerProgressBar.setSize(sf::Vector2f(newWidth, POWERUP_BAR_HEIGHT));
//         }
//     }

//     // Движение игрока
//     handlePlayerMovement(deltaTime);

//     // Спавн змей (если не остановлены)
//     if (!erasingSnakesProcess) {
//         if (snakeSpawnTimer >= snakeSpawnDuration && !isSnakesStopped) {
//             spawnSnake();
//             snakeSpawnTimer = 0;
//             speedOfStoppedSnakes.clear();
//         }
//     }

//     // Спавн бонусов
//     if (!powerUps.empty()) {
//         // Если бонус уже есть на поле, сбросим таймер
//         powerUpSpawnTimer = 0;
//     }
//     if (powerUpSpawnTimer >= powerUpSpawnDuration) {
//         spawnPowerUp();
//         powerUpSpawnTimer = 0;
//     }

//     // Обновляем змей (ещё раз — при необходимости)
//     if (!snakes.empty()) {
//         for (auto& snake : snakes) {
//             if (snake){
//                 snake->update(deltaTime);
//             }
//         }
//     }

//     // Повышение сложности
//     if (difficultyLevelTimer >= DIFFICULTY_LEVEL_UP_INTERVAL) {
//         adjustDifficultyLevel();
//         difficultyLevelTimer = 0;
//         walls.clear();
//         int amountOfWall = rand() % 5 + 2;
//         for (int i = 0; i < amountOfWall; i++) {
//             spawnWall();
//         }
//     }

//     // Проверяем столкновения
//     handleCollisions();
//     handleWallCollisions();
// }

// void Game::adjustDifficultyLevel() {
//     if (difficultyLevel <= 5) {
//         snakeSpawnDuration -= 0.4f;
//         difficultyLevel++;
//         powerUpSpawnDuration += 1.0f;
//     } else {
//         snakeSpawnDuration -= 0.1f;
//         difficultyLevel++;
//         powerUpSpawnDuration += 0.5f;
//     }
// }

// void Game::render() {
//     window.clear();

//     // Рисуем игрока
//     window.draw(*player);

//     // Рисуем змей
//     for (const auto& snake : snakes) {
//         window.draw(*snake);
//     }

//     // Рисуем стены
//     for (const auto& wall : walls) {
//         window.draw(*wall);
//     }

//     // Рисуем бонусы поверх стен
//     for (const auto& powerUp : powerUps) {
//         window.draw(*powerUp);
//     }

//     // Текст таймера
//     window.draw(timerText);

//     // Старый (красный) прогресс-бар
//     window.draw(progressBar);
//     window.draw(progressBarOutline);

//     // Если бонус активен — рисуем оранжевый бар
//     if (powerUpBarActive) {
//         window.draw(powerProgressBarOutline);
//         window.draw(powerProgressBar);
//     }

//     window.display();
// }

// void Game::spawnSnake() {
//     if (!erasingSnakesProcess) {
//         int side = rand() % 4;
//         float x = 0, y = 0;
//         float angle = 0;

//         switch (side) {
//             case 0: // Верх
//                 x = rand() % window.getSize().x;
//                 y = 0;
//                 angle = 45.f + static_cast<float>(rand()) / RAND_MAX * (135.f - 45.f);
//                 break;
//             case 1: // Низ
//                 x = rand() % window.getSize().x;
//                 y = window.getSize().y;
//                 angle = 225.f + static_cast<float>(rand()) / RAND_MAX * (315.f - 225.f);
//                 break;
//             case 2: // Лево
//                 x = 0;
//                 y = rand() % window.getSize().y;
//                 angle = -45.f + static_cast<float>(rand()) / RAND_MAX * 90.f;
//                 if (angle < 0.f) angle += 360.f;
//                 break;
//             case 3: // Право
//                 x = window.getSize().x;
//                 y = rand() % window.getSize().y;
//                 angle = 135.f + static_cast<float>(rand()) / RAND_MAX * (225.f - 135.f);
//                 break;
//         }

//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_real_distribution<float> distSpeed(SNAKE_SPEED_MIN, SNAKE_SPEED_MAX);
//         std::uniform_real_distribution<float> distSize(SNAKE_SEGMENT_SIZE_MIN, SNAKE_SEGMENT_SIZE_MAX);
        
//         float speed = distSpeed(gen);
//         float size = distSize(gen);
//         int numSegments = (rand() % (SNAKE_SEGMENT_AMOUNT_MAX - SNAKE_SEGMENT_AMOUNT_MIN)) + SNAKE_SEGMENT_AMOUNT_MIN;
//         angle = angle * PI / 180.0f;

//         auto newSnake = std::make_unique<Snake>(speed, angle, numSegments, size);
//         newSnake->setPosition(sf::Vector2f(x, y));
        
//         snakes.push_back(std::move(newSnake));
//     }
// }

// void Game::spawnPowerUp() {
//     auto newPowerUp = std::make_unique<PowerUp>();
//     int x = rand() % window.getSize().x;
//     int y = rand() % window.getSize().y;

//     x = std::clamp(x, 30, static_cast<int>(window.getSize().x - 30));
//     y = std::clamp(y, 30, static_cast<int>(window.getSize().y - 30));

//     newPowerUp->setPosition(x, y);
//     powerUps.push_back(std::move(newPowerUp));
// }

// void Game::handleCollisions() {
//     // Столкновения со змеями
//     if (!snakes.empty()) {
//         for (auto it = snakes.begin(); it != snakes.end();) {
//             bool collided = false;
//             const auto &snakeSegments = (*it)->getSegments();

//             for (const auto &seg : snakeSegments) {
//                 if (player->getGlobalBounds().intersects(seg.getGlobalBounds())) {
//                     if (!player->isInvincible()) {
//                         resetGame();
//                         return; 
//                     }
//                     collided = true;
//                     break;
//                 }
//             }

//             if (!collided) {
//                 ++it;
//             } else {
//                 // Если хотим удалять змей при коллизии в режиме неуязвимости
//                 // it = snakes.erase(it);
//                 // иначе просто:
//                 ++it;
//             }
//         }
//     }

//     // Столкновения с бонусами
//     for (auto it = powerUps.begin(); it != powerUps.end();) {
//         if (player->getGlobalBounds().intersects((*it)->getGlobalBounds())) {
//             activatePowerUp(*(*it));
//             it = powerUps.erase(it);
//         } else {
//             ++it;
//         }
//     }
// }

// void Game::spawnWall() {
//     const int maxAttempts = 10;
//     int attempt = 0;
//     bool validPosition = false;
//     std::unique_ptr<Wall> newWall = std::make_unique<Wall>();

//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_real_distribution<float> distX(50.0f, window.getSize().x - 50.0f);
//     std::uniform_real_distribution<float> distY(50.0f, window.getSize().y - 50.0f);
//     std::uniform_real_distribution<float> distLength(WALL_MIN_LENGTH, WALL_MAX_LENGTH);
//     std::uniform_int_distribution<int> orientationDist(0, 1);

//     while (attempt < maxAttempts && !validPosition) {
//         float x = distX(gen);
//         float y = distY(gen);
//         float length = distLength(gen);
//         bool isVertical = (orientationDist(gen) == 1);

//         newWall->setPosition(x, y);
//         newWall->setSize(length, isVertical);

//         if (!player->getGlobalBounds().intersects(newWall->getGlobalBounds())) {
//             validPosition = true;
//         }
//         attempt++;
//     }
//     walls.push_back(std::move(newWall));
// }

// void Game::handleWallCollisions() {
//     sf::FloatRect playerBounds = player->getGlobalBounds();
//     sf::Vector2f pos = player->sprite.getPosition();
    
//     for (const auto& wall : walls) {
//         sf::FloatRect wallBounds = wall->getGlobalBounds();
        
//         if (playerBounds.intersects(wallBounds)) {
//             float intersectLeft   = std::max(playerBounds.left, wallBounds.left);
//             float intersectRight  = std::min(playerBounds.left + playerBounds.width, wallBounds.left + wallBounds.width);
//             float intersectTop    = std::max(playerBounds.top, wallBounds.top);
//             float intersectBottom = std::min(playerBounds.top + playerBounds.height, wallBounds.top + wallBounds.height);
            
//             float intersectWidth  = intersectRight - intersectLeft;
//             float intersectHeight = intersectBottom - intersectTop;
            
//             if (intersectWidth < intersectHeight) {
//                 if (playerBounds.left < wallBounds.left)
//                     pos.x -= intersectWidth;
//                 else
//                     pos.x += intersectWidth;
//             } else {
//                 if (playerBounds.top < wallBounds.top)
//                     pos.y -= intersectHeight;
//                 else
//                     pos.y += intersectHeight;
//             }
//             player->setPosition(pos.x, pos.y);
//             playerBounds = player->getGlobalBounds();
//         }
//     }
// }

// void Game::activatePowerUp(PowerUp& powerUp) {
//     std::cout << "Power-up activated!" << std::endl;
//     int effect =rand() % 5;

//     switch (effect) {
//         case 0: // Ускорение
//             player->makeSpeededUp(); // Внутри Player устанавливается таймер
//             // Включаем отображение прогресс-бара
//             powerUpBarActive = true;
//             powerUpTimeMax = SPEED_UP_DURATION;
//             powerUpTimeRemaining = SPEED_UP_DURATION;
//             break;
//         case 1: // Неуязвимость
//             player->makeInvincible();
//             powerUpBarActive = true;
//             powerUpTimeMax = INVINCIBILITY_DURATION;
//             powerUpTimeRemaining = INVINCIBILITY_DURATION;
//             break;
//         case 2: // Стирание всех змей (полоса не нужна)
//             erasingSnakesProcess = true;
//             if (!snakes.empty()) {
//                 snakes.clear();
//             }
//             erasingSnakesProcess = false;
//             std::cout << "All snakes erased!" << std::endl;
//             // Не активируем powerUpBarActive
//             break;
//         case 3: // Остановка змей
//             if (!snakes.empty()) {
//                 for (auto& snake : snakes) {
//                     snake->makeStop();
//                 }
//                 isSnakesStopped = true;
//             }
//             // Включаем полосу на DEFAULT_STOP_TIMER
//             powerUpBarActive = true;
//             powerUpTimeMax = DEFAULT_STOP_TIMER/2;
//             powerUpTimeRemaining = DEFAULT_STOP_TIMER/2;
//             break;
//         case 4: // Уменьшение игрока
//             player->shrink();
//             powerUpBarActive = true;
//             powerUpTimeMax = SHRINK_DURATION;
//             powerUpTimeRemaining = SHRINK_DURATION;
//             break;
//         default:
//             break;
//     }

//     // Раз у нас полоса включается заново, сразу выставим её ширину на максимум
//     if (powerUpBarActive) {
//         powerProgressBar.setSize(sf::Vector2f(POWERUP_BAR_WIDTH, POWERUP_BAR_HEIGHT));
//     }
// }

// void Game::saveStatistics(const std::string& fileName) {
//     Game::stats gameStats{0, 0, 0};
//     std::ifstream inputFile(fileName);

//     if (inputFile.is_open()) {
//         std::string line;
//         while (std::getline(inputFile, line)) {
//             if (line.find("Games Played:") != std::string::npos) {
//                 gameStats.gamesPlayed = std::stoi(line.substr(line.find(':') + 1));
//             } else if (line.find("High Score:") != std::string::npos) {
//                 gameStats.highScore = std::stoi(line.substr(line.find(':') + 1));
//             } else if (line.find("Total Time:") != std::string::npos) {
//                 gameStats.totalTime = std::stoi(line.substr(line.find(':') + 1));
//             }
//         }
//         inputFile.close();
//     }

//     gameStats.gamesPlayed += 1;
//     gameStats.totalTime += static_cast<int>(elapsedGameTime);

//     // Примерно так можно обновить High Score (не совсем корректно в оригинале)
//     // если текущее время больше, чем предыдущий рекорд, и т.д.
//     if (static_cast<int>(elapsedGameTime) > gameStats.highScore) {
//         gameStats.highScore = static_cast<int>(elapsedGameTime);
//     }

//     std::ofstream outputFile(fileName);
//     if (outputFile.is_open()) {
//         outputFile << "Games Played:" << gameStats.gamesPlayed << '\n';
//         outputFile << "High Score:" << gameStats.highScore << '\n';
//         outputFile << "Total Time:" << gameStats.totalTime << '\n';
//         outputFile.close();
//     } else {
//         std::cerr << "Unable to open file for writing.\n";
//     }
// }

// void Game::handlePlayerMovement(float deltaTime) {
//     float speed = player->returnSpeed();
//     sf::Vector2f moveOffset(0.f, 0.f);

//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
//         moveOffset.y -= speed;
//     }
//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
//         moveOffset.y += speed;
//     }
//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
//         moveOffset.x -= speed;
//     }
//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
//         moveOffset.x += speed;
//     }

//     if (moveOffset.x != 0.f && moveOffset.y != 0.f) {
//         moveOffset *= DIAGONAL_SPEED_FACTOR;
//     }

//     player->move(moveOffset.x, moveOffset.y, walls, deltaTime);
// }

// void Game::resetGame() {
//     backgroundMusic.stop();
//     snakes.clear();
//     powerUps.clear();
//     walls.clear();
//     player->reset();
//     isSnakesStopped = false;
//     isGameRunning = false;

//     progressBarFill = 0.f;
//     progressBar.setSize(sf::Vector2f(PROGRESS_BAR_WIDTH, progressBar.getSize().y));

//     // Сбросим и второй бар
//     powerUpBarActive = false;
//     powerUpTimeRemaining = 0.f;

//     this->saveStatistics("statistics.txt");
    
//     bool newGame = menu->showWelcomeScreen(window, *this);
//     if (newGame) {
//         snakeSpawnTimer = 0;
//         powerUpSpawnTimer = 0;
//         dashTimer = 0;
//         difficultyLevelTimer = 0;
//         elapsedGameTime = 0;
//         gameClock.restart();
//         isGameRunning = true;
//         backgroundMusic.play();
//         difficultyLevel = 0;
//     } else {
//         window.close();
//     }
    
//     std::cout << "Game Over" << std::endl;
// }

#include "../include/Game.h"
#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include "../include/PowerUp.h"
#include "../include/Player.h"
#include "../include/Constants.h"

Game::Game() 
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arcade Game"), 
      snakeSpawnTimer(0),
      powerUpSpawnTimer(0),
      isGameRunning(false),
      difficultyLevelTimer(0.0f),
      difficultyLevel(0),
      gameClock(),
      wallSpawnTimer(0)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    menu = std::make_unique<Menu>();
    startTheGame = menu->showWelcomeScreen(window, *this);

    player = std::make_unique<Player>();
    player->setPosition(PLAYER_POSITION_X, PLAYER_POSITION_Y);

    font.loadFromFile("../assets/arial.ttf");
    timerText.setFont(font);
    timerText.setCharacterSize(FONT_SIZE);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(TIMERTEXT_POSITION_X, TIMERTEXT_POSITION_Y);

    if (!backgroundMusic.openFromFile("../assets/BackgroundAudio.mp3")) {
        std::cout << "Error: Failed to load background music!" << std::endl;
    } else {
        backgroundMusic.setLoop(true);
        backgroundMusic.play();
    }

    // --- "Красный" прогресс-бар (ваш) ---
    progressBar.setSize(sf::Vector2f(PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT));
    progressBar.setFillColor(sf::Color::Red);
    progressBar.setPosition(PROGRESSBAR_POSITION_X, PROGRESSBAR_POSITION_Y);

    progressBarOutline.setSize(sf::Vector2f(PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT));
    progressBarOutline.setOutlineThickness(PROGRESS_BAR_OUTLINE_THICKNESS);
    progressBarOutline.setOutlineColor(sf::Color::White);
    progressBarOutline.setFillColor(sf::Color::Transparent);
    progressBarOutline.setPosition(progressBar.getPosition());

    // --- Оранжевая полоса для "длительных" усилителей ---
    powerProgressBar.setSize(sf::Vector2f(POWERUP_BAR_WIDTH, POWERUP_BAR_HEIGHT));
    powerProgressBar.setFillColor(POWERUP_BAR_FILL_COLOR);
    // Правый верхний угол с отступом
    powerProgressBar.setPosition(WINDOW_WIDTH - POWERUP_BAR_OFFSET_X, POWERUP_BAR_OFFSET_Y);

    powerProgressBarOutline.setSize(sf::Vector2f(POWERUP_BAR_WIDTH, POWERUP_BAR_HEIGHT));
    powerProgressBarOutline.setOutlineThickness(POWERUP_BAR_OUTLINE_THICKNESS);
    powerProgressBarOutline.setOutlineColor(POWERUP_BAR_OUTLINE_COLOR);
    powerProgressBarOutline.setFillColor(sf::Color::Transparent);
    powerProgressBarOutline.setPosition(powerProgressBar.getPosition());

    // Иконка (пока не загружена), будет отображаться у оранжевой полоски
    powerUpIconActive = false;
    // Можно задать чуть левее полосы:
    powerUpIconSprite.setPosition(
        powerProgressBar.getPosition().x - 40.f,
        powerProgressBar.getPosition().y - 5.f
    );
    // Подберите координаты, чтобы иконка выглядела красиво.

    elapsedGameTime = 0.f;
    progressBarFill = 0.f;

    snakeSpawnDuration  = SNAKE_SPAWN_DURATION_INITIAL;
    powerUpSpawnDuration = POWER_UP_SPAWN_DURATION_INITIAL;
}

void Game::run() {
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        if (isGameRunning && startTheGame) {
            update();
            render();
            processEvents();
        }
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        // Обработка dash
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LShift) {
            int dashDirection = -1;
            bool up    = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
            bool down  = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
            bool left  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

            if (up && !down && !left && !right)
                dashDirection = 0; // Up
            else if (up && right)
                dashDirection = 1; // Up-Right
            else if (right && !up && !down)
                dashDirection = 2; // Right
            else if (down && right)
                dashDirection = 3; // Down-Right
            else if (down && !up && !left && !right)
                dashDirection = 4; // Down
            else if (down && left)
                dashDirection = 5; // Down-Left
            else if (left && !up && !down)
                dashDirection = 6; // Left
            else if (up && left)
                dashDirection = 7; // Up-Left

            if (dashDirection == -1)
                dashDirection = 0; // По умолчанию — Up

            player->dash(dashDirection, walls);
        }
    }
}

void Game::update() {
    float deltaTime = gameClock.restart().asSeconds();

    dashTimer += deltaTime;
    snakeSpawnTimer += deltaTime;
    powerUpSpawnTimer += deltaTime;
    difficultyLevelTimer += deltaTime;

    player->updateTimers(deltaTime);

    // Обновляем змей, если они не удалены
    if (!erasingSnakesProcess && !snakes.empty()) {
        for (auto &snake : snakes) {
            if (snake) {
                snake->update(deltaTime);
                isSnakesStopped = snake->isStopped();
            }
        }
    }

    elapsedGameTime += deltaTime;
    timerText.setString("Time: " + std::to_string(static_cast<int>(elapsedGameTime)) + "s");

    // -- Красный прогресс-бар --
    progressBarFill += deltaTime * PROGRESS_INCREMENT;
    if (progressBarFill >= PROGRESS_BAR_WIDTH) {
        progressBarFill = 0.f;
    }
    progressBar.setSize(sf::Vector2f(progressBarFill, progressBar.getSize().y));

    // -- Логика оранжевой полосы для "длительных" бонусов --
    if (powerUpBarActive) {
        powerUpTimeRemaining -= deltaTime;
        if (powerUpTimeRemaining <= 0.f) {
            // Снятие эффекта
            powerUpTimeRemaining = 0.f;
            powerUpBarActive = false;

            // Если это была остановка змей — возобновляем
            if (isSnakesStopped) {
                for (auto& snake : snakes) {
                    snake->makeResume();  // Метод, который восстанавливает движение
                }
                isSnakesStopped = false;
            }
        } else {
            float ratio = powerUpTimeRemaining / powerUpTimeMax;
            float newWidth = ratio * POWERUP_BAR_WIDTH;
            powerProgressBar.setSize(sf::Vector2f(newWidth, POWERUP_BAR_HEIGHT));
        }
    }

    // -- Логика иконки, показываемой рядом с полосой --
    // Если иконка активна, уменьшаем время
    if (powerUpIconActive) {
        powerUpIconTimeRemaining -= deltaTime;
        if (powerUpIconTimeRemaining <= 0.f) {
            // Прекращаем отображение иконки
            powerUpIconActive = false;
        }
    }

    // Движение игрока
    handlePlayerMovement(deltaTime);

    // Спавн змей
    if (!erasingSnakesProcess) {
        if (snakeSpawnTimer >= snakeSpawnDuration && !isSnakesStopped) {
            spawnSnake();
            snakeSpawnTimer = 0;
            speedOfStoppedSnakes.clear();
        }
    }

    // Спавн бонусов
    if (!powerUps.empty()) {
        powerUpSpawnTimer = 0;
    }
    if (powerUpSpawnTimer >= powerUpSpawnDuration) {
        spawnPowerUp();
        powerUpSpawnTimer = 0;
    }

    // Сложность
    if (difficultyLevelTimer >= DIFFICULTY_LEVEL_UP_INTERVAL) {
        adjustDifficultyLevel();
        difficultyLevelTimer = 0;
        walls.clear();
        int amountOfWall = rand() % 5 + 2;
        for (int i = 0; i < amountOfWall; i++) {
            spawnWall();
        }
    }

    // Проверяем столкновения
    handleCollisions();
    handleWallCollisions();
}

void Game::adjustDifficultyLevel() {
    if (difficultyLevel <= 5) {
        snakeSpawnDuration -= 0.4f;
        difficultyLevel++;
        std::cout << difficultyLevel << std::endl;
        powerUpSpawnDuration += 1.0f;
    } else {
        snakeSpawnDuration -= 0.1f;
        difficultyLevel++;
        std::cout << difficultyLevel << std::endl;
        powerUpSpawnDuration += 0.5f;
    }
}

void Game::render() {
    window.clear();

    // Рисуем игрока
    window.draw(*player);

    // Рисуем змей
    for (const auto &snake : snakes) {
        window.draw(*snake);
    }

    // Рисуем стены
    for (const auto &wall : walls) {
        window.draw(*wall);
    }

    // Рисуем бонусы
    for (const auto &powerUp : powerUps) {
        window.draw(*powerUp);
    }

    // Текст таймера
    window.draw(timerText);

    // Красный прогресс-бар
    window.draw(progressBar);
    window.draw(progressBarOutline);

    // Если активен "длительный" бонус — рисуем полосу
    if (powerUpBarActive) {
        window.draw(powerProgressBarOutline);
        window.draw(powerProgressBar);
    }

    // Если иконка активна — рисуем
    if (powerUpIconActive) {
        window.draw(powerUpIconSprite);
    }

    window.display();
}

void Game::spawnSnake() {
    if (!erasingSnakesProcess) {
        int side = rand() % 4;
        float x = 0, y = 0;
        float angle = 0;

        switch (side) {
            case 0: // Верх
                x = rand() % window.getSize().x;
                y = 0;
                angle = 45.f + static_cast<float>(rand()) / RAND_MAX * (135.f - 45.f);
                break;
            case 1: // Низ
                x = rand() % window.getSize().x;
                y = window.getSize().y;
                angle = 225.f + static_cast<float>(rand()) / RAND_MAX * (315.f - 225.f);
                break;
            case 2: // Лево
                x = 0;
                y = rand() % window.getSize().y;
                angle = -45.f + static_cast<float>(rand()) / RAND_MAX * 90.f;
                if (angle < 0.f) angle += 360.f;
                break;
            case 3: // Право
                x = window.getSize().x;
                y = rand() % window.getSize().y;
                angle = 135.f + static_cast<float>(rand()) / RAND_MAX * (225.f - 135.f);
                break;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> distSpeed(SNAKE_SPEED_MIN, SNAKE_SPEED_MAX);
        std::uniform_real_distribution<float> distSize(SNAKE_SEGMENT_SIZE_MIN, SNAKE_SEGMENT_SIZE_MAX);

        float speed = distSpeed(gen);
        float size  = distSize(gen);
        int   numSegments = (rand() % (SNAKE_SEGMENT_AMOUNT_MAX - SNAKE_SEGMENT_AMOUNT_MIN))
                            + SNAKE_SEGMENT_AMOUNT_MIN;
        angle = angle * PI / 180.f;

        auto newSnake = std::make_unique<Snake>(speed, angle, numSegments, size);
        newSnake->setPosition(sf::Vector2f(x, y));

        snakes.push_back(std::move(newSnake));
    }
}

void Game::spawnPowerUp() {
    auto newPowerUp = std::make_unique<PowerUp>();
    int x = rand() % window.getSize().x;
    int y = rand() % window.getSize().y;

    x = std::clamp(x, 30, static_cast<int>(window.getSize().x - 30));
    y = std::clamp(y, 30, static_cast<int>(window.getSize().y - 30));

    newPowerUp->setPosition(x, y);
    powerUps.push_back(std::move(newPowerUp));
}

void Game::handleCollisions() {
    // Столкновения со змеями
    if (!snakes.empty()) {
        for (auto it = snakes.begin(); it != snakes.end();) {
            bool collided = false;
            const auto &snakeSegments = (*it)->getSegments();

            for (const auto &seg : snakeSegments) {
                if (player->getGlobalBounds().intersects(seg.getGlobalBounds())) {
                    // Проверка неуязвимости игрока
                    if (!player->isInvincible()) {
                        resetGame();
                        return;
                    }
                    collided = true;
                    break;
                }
            }

            if (!collided) {
                ++it;
            } else {
                // Можем убрать змею, если хотим
                // it = snakes.erase(it);
                // а можем оставить
                ++it;
            }
        }
    }

    // Столкновения с бонусами
    for (auto it = powerUps.begin(); it != powerUps.end();) {
        if (player->getGlobalBounds().intersects((*it)->getGlobalBounds())) {
            activatePowerUp(*(*it));
            it = powerUps.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::spawnWall() {
    const int maxAttempts = 10;
    int attempt = 0;
    bool validPosition = false;
    std::unique_ptr<Wall> newWall = std::make_unique<Wall>();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(50.0f, window.getSize().x - 50.0f);
    std::uniform_real_distribution<float> distY(50.0f, window.getSize().y - 50.0f);
    std::uniform_real_distribution<float> distLength(WALL_MIN_LENGTH, WALL_MAX_LENGTH);
    std::uniform_int_distribution<int> orientationDist(0, 1);

    while (attempt < maxAttempts && !validPosition) {
        float x = distX(gen);
        float y = distY(gen);
        float length = distLength(gen);
        bool isVertical = (orientationDist(gen) == 1);

        newWall->setPosition(x, y);
        newWall->setSize(length, isVertical);

        if (!player->getGlobalBounds().intersects(newWall->getGlobalBounds())) {
            validPosition = true;
        }
        attempt++;
    }
    walls.push_back(std::move(newWall));
}

void Game::handleWallCollisions() {
    sf::FloatRect playerBounds = player->getGlobalBounds();
    sf::Vector2f pos = player->sprite.getPosition();

    for (const auto& wall : walls) {
        sf::FloatRect wallBounds = wall->getGlobalBounds();

        if (playerBounds.intersects(wallBounds)) {
            float intersectLeft   = std::max(playerBounds.left, wallBounds.left);
            float intersectRight  = std::min(playerBounds.left + playerBounds.width, wallBounds.left + wallBounds.width);
            float intersectTop    = std::max(playerBounds.top, wallBounds.top);
            float intersectBottom = std::min(playerBounds.top + playerBounds.height, wallBounds.top + wallBounds.height);

            float intersectWidth  = intersectRight - intersectLeft;
            float intersectHeight = intersectBottom - intersectTop;

            if (intersectWidth < intersectHeight) {
                if (playerBounds.left < wallBounds.left)
                    pos.x -= intersectWidth;
                else
                    pos.x += intersectWidth;
            } else {
                if (playerBounds.top < wallBounds.top)
                    pos.y -= intersectHeight;
                else
                    pos.y += intersectHeight;
            }
            player->setPosition(pos.x, pos.y);
            playerBounds = player->getGlobalBounds();
        }
    }
}

void Game::activatePowerUp(PowerUp &powerUp) {
    std::cout << "Power-up activated!" << std::endl;

    // Предположим, что effect = rand() % 5
    // Или у вас уже где-то рассчитан effect:
    int effect = rand() % 5; // пример для теста

    // ------------------------------------
    // 1) Прогресс-бар (длительность) -> powerUpBarActive
    // 2) Иконка -> powerUpIconActive
    // ------------------------------------
    // По умолчанию отключаем полосу
    powerUpBarActive = false;
    powerUpTimeRemaining = 0.f;
    powerUpTimeMax       = 0.f;

    // Но иконку всегда включаем (на небольшое время, 
    // или столько же, сколько длится бонус)
    powerUpIconActive = true;
    powerUpIconTimeRemaining = 2.f;  // если это бонус без длительности
                                     // (стереть змей), иконка мигнёт 2с

    // В зависимости от effect загружаем соответствующий файл
    switch (effect) {
        case 0: // Ускорение
            player->makeSpeededUp();
            // Запускаем бар
            powerUpBarActive      = true;
            powerUpTimeMax        = SPEED_UP_DURATION;   // 8.0f, к примеру
            powerUpTimeRemaining  = SPEED_UP_DURATION;
            // И иконку показываем ровно столько же:
            powerUpIconTimeRemaining = SPEED_UP_DURATION;

            // Загружаем текстуру
            if (!powerUpIconTexture.loadFromFile(POWERUP_SPEED_UP_LABEL)) {
                std::cout << "Error loading SPEED_UP_LABEL texture\n";
            }
            break;

        case 1: // Неуязвимость
            player->makeInvincible();
            powerUpBarActive      = true;
            powerUpTimeMax        = INVINCIBILITY_DURATION;  // 5.0f
            powerUpTimeRemaining  = INVINCIBILITY_DURATION;
            powerUpIconTimeRemaining = INVINCIBILITY_DURATION;

            if (!powerUpIconTexture.loadFromFile(POWERUP_INVINCIBILITY_LABEL)) {
                std::cout << "Error loading INVINCIBILITY_LABEL\n";
            }
            break;

        case 2: // Стирание всех змей (моментальный)
            erasingSnakesProcess = true;
            if (!snakes.empty()) {
                snakes.clear();
            }
            erasingSnakesProcess = false;
            // Полосу не активируем (нет длительности)
            // Но иконку покажем 2 секунды
            if (!powerUpIconTexture.loadFromFile(POWERUP_ERASE_LABEL)) {
                std::cout << "Error loading ERASE_LABEL\n";
            }
            std::cout << "All snakes erased!\n";
            break;

        case 3: // Остановка змей
            if (!snakes.empty()) {
                for (auto &snake : snakes) {
                    snake->makeStop();
                }
                isSnakesStopped = true;
            }
            // Длительный эффект => бар
            powerUpBarActive      = true;
            powerUpTimeMax        = DEFAULT_STOP_TIMER; // к примеру 6.0f
            powerUpTimeRemaining  = DEFAULT_STOP_TIMER;
            powerUpIconTimeRemaining = DEFAULT_STOP_TIMER;

            if (!powerUpIconTexture.loadFromFile(POWERUP_STOP_SNAKES_LABEL)) {
                std::cout << "Error loading STOP_SNAKES_LABEL\n";
            }
            break;

        case 4: // Уменьшение игрока
            player->shrink();
            // Длительный эффект => бар
            powerUpBarActive      = true;
            powerUpTimeMax        = SHRINK_DURATION;  // 7.0f
            powerUpTimeRemaining  = SHRINK_DURATION;
            powerUpIconTimeRemaining = SHRINK_DURATION;

            if (!powerUpIconTexture.loadFromFile(POWERUP_SHRINK_LABEL)) {
                std::cout << "Error loading SHRINK_LABEL\n";
            }
            break;

        default:
            // На всякий случай
            if (!powerUpIconTexture.loadFromFile(POWERUP_ERASE_LABEL)) {
                std::cout << "Error loading ERASE_LABEL\n";
            }
            break;
    }

    // Обновили текстуру иконки — привяжем её к sprite
    powerUpIconSprite.setTexture(powerUpIconTexture);

    // Подберите масштаб, если нужно:
    powerUpIconSprite.setScale(0.06f, 0.06f); 
    // Или оставьте как есть, если устраивает размер

    // Если запустили полосу, сразу выставим ширину на максимум
    if (powerUpBarActive) {
        powerProgressBar.setSize(sf::Vector2f(POWERUP_BAR_WIDTH, POWERUP_BAR_HEIGHT));
    }
}

void Game::saveStatistics(const std::string &fileName) {
    // Если у вас структура Game::stats, пишите: Game::stats gameStats{...}
    // или вынесите структуру "stats" вне класса
    // Ниже - пример, если struct stats вне класса Game
    struct stats {
        int gamesPlayed;
        int highScore;
        int totalTime;
    };
    stats gameStats{0, 0, 0};

    std::ifstream inputFile(fileName);
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            if (line.find("Games Played:") != std::string::npos) {
                gameStats.gamesPlayed = std::stoi(line.substr(line.find(':') + 1));
            } else if (line.find("High Score:") != std::string::npos) {
                gameStats.highScore = std::stoi(line.substr(line.find(':') + 1));
            } else if (line.find("Total Time:") != std::string::npos) {
                gameStats.totalTime = std::stoi(line.substr(line.find(':') + 1));
            }
        }
        inputFile.close();
    }

    gameStats.gamesPlayed += 1;
    gameStats.totalTime += static_cast<int>(elapsedGameTime);

    // Пример обновления рекорда
    if (static_cast<int>(elapsedGameTime) > gameStats.highScore) {
        gameStats.highScore = static_cast<int>(elapsedGameTime);
    }

    std::ofstream outputFile(fileName);
    if (outputFile.is_open()) {
        outputFile << "Games Played:" << gameStats.gamesPlayed << '\n';
        outputFile << "High Score:" << gameStats.highScore << '\n';
        outputFile << "Total Time:" << gameStats.totalTime << '\n';
        outputFile.close();
    } else {
        std::cerr << "Unable to open file for writing.\n";
    }
}

void Game::handlePlayerMovement(float deltaTime) {
    float speed = player->returnSpeed();
    sf::Vector2f moveOffset(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        moveOffset.y -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        moveOffset.y += speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        moveOffset.x -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        moveOffset.x += speed;
    }

    if (moveOffset.x != 0.f && moveOffset.y != 0.f) {
        moveOffset *= DIAGONAL_SPEED_FACTOR; 
    }

    player->move(moveOffset.x, moveOffset.y, walls, deltaTime);
}

void Game::resetGame() {
    backgroundMusic.stop();
    snakes.clear();
    powerUps.clear();
    walls.clear();
    player->reset();
    isSnakesStopped = false;
    isGameRunning = false;
    difficultyLevelTimer = 0;
    difficultyLevel = 0;
    progressBarFill = 0.f;
    progressBar.setSize(sf::Vector2f(PROGRESS_BAR_WIDTH, progressBar.getSize().y));
    snakeSpawnDuration  = SNAKE_SPAWN_DURATION_INITIAL;
    powerUpSpawnDuration = POWER_UP_SPAWN_DURATION_INITIAL;

    // Сбросим оранжевый бар
    powerUpBarActive      = false;
    powerUpTimeRemaining  = 0.f;

    // Сбросим иконку
    powerUpIconActive       = false;
    powerUpIconTimeRemaining = 0.f;

    this->saveStatistics("statistics.txt");

    bool newGame = menu->showWelcomeScreen(window, *this);
    if (newGame) {
        snakeSpawnTimer      = 0;
        powerUpSpawnTimer    = 0;
        difficultyLevelTimer = 0;
        elapsedGameTime      = 0;
        snakeSpawnDuration  = SNAKE_SPAWN_DURATION_INITIAL;
    powerUpSpawnDuration = POWER_UP_SPAWN_DURATION_INITIAL;
        difficultyLevel = 0;
        std::cout << "DL = 0 , reset" << std::endl;
        gameClock.restart();
        isGameRunning = true;
        backgroundMusic.play();
    } else {
        window.close();
    }

    std::cout << "Game Over" << std::endl;
}