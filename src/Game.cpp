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
      snakeSpawnTimer(0), powerUpSpawnTimer(0), 
      isGameRunning(false), difficultyLevelTimer(0.0f), difficultyLevel(0), gameClock(), wallSpawnTimer(0) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    menu = std::make_unique<Menu>();
    startTheGame = menu->showWelcomeScreen(window, *this);
    player = std::make_unique<Player>(); 
    std::cout << "Error here 1" << std::endl;
    player->setPosition(PLAYER_POSITION_X, PLAYER_POSITION_Y);
    std::cout << "Error here 2" << std::endl;
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
    // Шкала прогресса
    progressBar.setSize(sf::Vector2f(PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT)); // Уменьшенный размер шкалы
    progressBar.setFillColor(sf::Color::Red);
    progressBar.setPosition(PROGRESSBAR_POSITION_X, PROGRESSBAR_POSITION_Y); // Положение на экране

    // Обводка шкалы прогресса
    progressBarOutline.setSize(sf::Vector2f(PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT));
    progressBarOutline.setOutlineThickness(PROGRESS_BAR_OUTLINE_THICKNESS); // Толщина обводки
    progressBarOutline.setOutlineColor(sf::Color::White); // Цвет обводки
    progressBarOutline.setFillColor(sf::Color::Transparent); // Прозрачная внутренняя часть
    progressBarOutline.setPosition(progressBar.getPosition()); // Совпадение с положением шкалы

    elapsedGameTime = 0.f;
    progressBarFill = 0.f;

    snakeSpawnDuration = SNAKE_SPAWN_DURATION_INITIAL;
    powerUpSpawnDuration = POWER_UP_SPAWN_DURATION_INITIAL;
   }
void Game::run() {
    while (window.isOpen()) {
        std::cout << elapsedGameTime << std::endl;

        if (isGameRunning && startTheGame) {
            std::cout << "DoUpdates" << std::endl;
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
        if (isGameRunning && startTheGame) {
            if (event.type == sf::Event::KeyPressed) {
                float speed = player->returnSpeed();
                float diagonalFactor = DIAGONAL_SPEED_FACTOR; // Нормализация диагональной скорости
                sf::Vector2f moveOffset(0.f, 0.f);
                if (event.key.code == sf::Keyboard::W) {
                    moveOffset.y = -speed;
                } if (event.key.code == sf::Keyboard::S) {
                    moveOffset.y = speed;
                } if (event.key.code == sf::Keyboard::A) {
                    moveOffset.x = -speed;
                } if (event.key.code == sf::Keyboard::D) {
                    moveOffset.x = speed;
                }
                // Проверка для диагонального движения
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    moveOffset.x = speed * diagonalFactor;
                    moveOffset.y = -speed * diagonalFactor;
                } if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    moveOffset.x = -speed * diagonalFactor;
                    moveOffset.y = -speed * diagonalFactor;
                }if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    moveOffset.x = speed * diagonalFactor;
                    moveOffset.y = speed * diagonalFactor;
                }if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    moveOffset.x = -speed * diagonalFactor;
                    moveOffset.y = speed * diagonalFactor;
                }
                // Проверка на столкновение перед движением
                sf::Vector2f newPosition = player->sprite.getPosition() + moveOffset;
                sf::FloatRect newBounds = player->getGlobalBounds();                
                newBounds.left += moveOffset.x;
                newBounds.top += moveOffset.y;
            
                bool collision = false;
                for (const auto& wall : walls) {
                    if (newBounds.intersects(wall->getGlobalBounds())) {
                        collision = true;
                        break;
                    }
                }

                // Если нет столкновений, игрок двигается
                if (!collision) {
                    player->move(window, moveOffset.x, moveOffset.y, walls);
                }
            }
        } 
    }
}



void Game::update() {
    float elapsedTime = gameClock.restart().asSeconds();
    
    dashTimer += elapsedTime;
    snakeSpawnTimer += elapsedTime;
    powerUpSpawnTimer += elapsedTime;
    difficultyLevelTimer += elapsedTime;
    player->updateTimers(elapsedTime);
    if(!erasingSnakesProcess && !snakes.empty()){
        for(const auto& snake : snakes){
            if(snake){
            snake->updateTimers(elapsedTime);
            isSnakesStopped = snake->isSnakeStopped();
            }
        }
    }
    elapsedGameTime += elapsedTime;
    std::cout << "Update Timer" << std::endl;
    timerText.setString("Time: " + std::to_string(static_cast<int>(elapsedGameTime)) + "s");

    // Обновление прогресса
    progressBarFill += elapsedTime * (PROGRESS_INCREMENT);
    if (progressBarFill >= PROGRESS_BAR_WIDTH) {
        progressBarFill = 0.f; // Сброс шкалы
    }
    progressBar.setSize(sf::Vector2f(progressBarFill, progressBar.getSize().y));


    // wallSpawnTimer += elapsedTime;
    // if (wallSpawnTimer >= wallSpawnDuration) {
    //     spawnWall();
    //     wallSpawnTimer = 0.0f;
    // }
    
    if(!erasingSnakesProcess){
    if (snakeSpawnTimer >= snakeSpawnDuration && !isSnakesStopped) {
        spawnSnake();
        std::cout << "Snake Spawned!" << std::endl;
        snakeSpawnTimer = 0;
        speedOfStoppedSnakes.clear();
    }}
    if(!powerUps.empty()){
        powerUpSpawnTimer = 0;
    }
    if (powerUpSpawnTimer >= powerUpSpawnDuration) {
        std::cout << isSnakesStopped << std::endl;
        spawnPowerUp();
        powerUpSpawnTimer = 0;
    }
    if(!snakes.empty()){
        for (auto& snake : snakes) {
            if(snake){
            snake->move();
            }  
        }
    }
    if(difficultyLevelTimer >= DIFFICULTY_LEVEL_UP_INTERVAL){
        this->adjustDifficultyLevel();
        difficultyLevelTimer = 0;
        walls.clear();
        int amountOfWall = rand()%5 + 2;
        for(int i = 0; i < amountOfWall; i++){
            spawnWall();
        }

    }

    handleCollisions();
    handleWallCollisions();
}

void Game::adjustDifficultyLevel(){
    if (difficultyLevel<=5){
        snakeSpawnDuration-=0.4f;
        difficultyLevel++;
        powerUpSpawnDuration+=1.0f;
    } else if (difficultyLevel > 5){
        snakeSpawnDuration-=0.1f;
        difficultyLevel++;
        powerUpSpawnDuration+=0.5f;
    }
}

void Game::render() {
    window.clear();

    // Отрисовка игрока, змей, бонусов и стен
    window.draw(*player);

    for (const auto& snake : snakes) {
        window.draw(*snake);
    }

    for (const auto& powerUp : powerUps) {
        window.draw(*powerUp);
    }

    // Отрисовка стен
    for (const auto& wall : walls) {
        window.draw(*wall);
    }
    window.draw(timerText);
    window.draw(progressBar);
    window.draw(progressBarOutline);

    window.display();
}


void Game::spawnSnake() {
    if(!erasingSnakesProcess){
    auto newSnake = std::make_unique<Snake>();

    int side = rand() % 4;
    float x = 0, y = 0;
    float angle = 0;

    switch (side) {
        case 0: // Cверху
            x = rand() % window.getSize().x;
            y = 0;
            angle = static_cast<float>(rand() % 180 + 90); // угол от 90 до 270, вниз
            break;
        case 1: // Снизу
            x = rand() % window.getSize().x;
            y = window.getSize().y;
            angle = static_cast<float>(rand() % 180); // угол от 0 до 180, вверх
            break;
        case 2: // Слева
            x = 0;
            y = rand() % window.getSize().y;
            angle = static_cast<float>(rand() % 180 + 180); // угол от 180 до 360, вправо
            break;
        case 3: // Справа
            x = window.getSize().x;
            y = rand() % window.getSize().y;
            angle = static_cast<float>(rand() % 180 + 270); // угол от 270 до 450, влево
            break;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distSpeed(0.05f, 0.5f);
    std::uniform_real_distribution<float> distSize(8.0f, 20.0f);
    
    float speed = distSpeed(gen);
    float size = distSize(gen);

    newSnake->setPosition(x, y);
    newSnake->setSpeed(speed);
    newSnake->setSpeedPrevious(speed);
    newSnake->setAngle(angle * 3.14159f / 180.0f);  // Преобразуем угол в радианы
    newSnake->setAmountOfSegments((rand()%20)+5); // Случайное количество сегментов
    newSnake->setSizeOfSegments(size);             // Размер сегментов
    newSnake->grow();
    
    

    snakes.push_back(std::move(newSnake));}
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
    if(!snakes.empty()){
    for (auto it = snakes.begin(); it != snakes.end();) {
        if (player->getGlobalBounds().intersects((*it)->getGlobalBounds())) { 
            std::cout << "Collision detected with snake!" << std::endl;

            if (!player->isInvincible()) {
                std::cout << "Player is not invincible, resetting game..." << std::endl;
                resetGame(); //Завершить игру если на игроке нет бафа неуязвимости
            }


        } else {
            ++it;
        }
    }}

    // Столкновение с бонусами
    for (auto it = powerUps.begin(); it != powerUps.end();) {
        if (player->getGlobalBounds().intersects((*it)->getGlobalBounds())) {  
            std::cout << "Power-up collected!" << std::endl;
            activatePowerUp(*(*it));  
            it = powerUps.erase(it);  
        } else {
            ++it;  
        }
    }
}

void Game::spawnWall() {
    auto newWall = std::make_unique<Wall>();

    // Случайное положение, длина и ориентация
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(50.0f, window.getSize().x - 50.0f);
    std::uniform_real_distribution<float> distY(50.0f, window.getSize().y - 50.0f);
    std::uniform_real_distribution<float> distLength(WALL_MIN_LENGTH, WALL_MAX_LENGTH);
    std::uniform_int_distribution<int> orientationDist(0, 1);

    float x = distX(gen);
    float y = distY(gen);
    float length = distLength(gen);
    bool isVertical = orientationDist(gen) == 1;

    newWall->setPosition(x, y);
    newWall->setSize(length, isVertical);

    walls.push_back(std::move(newWall));
}

void Game::handleWallCollisions() {
    sf::FloatRect playerBounds = player->getGlobalBounds();

    for (const auto& wall : walls) {
        sf::FloatRect wallBounds = wall->getGlobalBounds();

        // Проверяем пересечение игрока со стеной
        if (playerBounds.intersects(wallBounds)) {
            sf::Vector2f playerPosition = player->sprite.getPosition();

            // Определяем направление, из которого произошёл контакт, и корректируем позицию игрока
            if (playerBounds.left < wallBounds.left) {
                playerPosition.x = wallBounds.left - playerBounds.width;
            } else if (playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width) {
                playerPosition.x = wallBounds.left + wallBounds.width;
            }

            if (playerBounds.top < wallBounds.top) {
                playerPosition.y = wallBounds.top - playerBounds.height;
            } else if (playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height) {
                playerPosition.y = wallBounds.top + wallBounds.height;
            }

            // Устанавливаем скорректированную позицию игрока
            player->setPosition(playerPosition.x, playerPosition.y);
        }
    }
}






void Game::activatePowerUp(PowerUp& powerUp) {
    std::cout << "Power-up activated!" << std::endl;
    int effect = rand() % 5;  
    if (effect == 0) {
        player->makeSpeededUp();
        std::cout << "Start speeding-up" << std::endl;
    } else if (effect == 1) {
        player->makeInvincible();
        std::cout << "Start Invincibility" << std::endl;
    } else if (effect == 2) {
        std::cout << "Erasing all Snakes!" << std::endl;
        erasingSnakesProcess = true;
        if(!snakes.empty()){
        snakes.clear();
        }
        erasingSnakesProcess = false;
        std::cout << "Completed erasing snakes" << std::endl;
    } else if (effect == 3) {
        if(!snakes.empty()){
        for(const auto& snake : snakes){
            snake->makeStop();
            std::cout << "STOP all snakes!" << std::endl;
        }
        isSnakesStopped = true;
        }
    } else if (effect == 4){
        std::cout << "Shrink player" << std::endl;
        player->shrink();
    }
}

void Game::saveStatistics(const std::string& fileName) {
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

    // Update statistics
    gameStats.gamesPlayed += 1;
    gameStats.totalTime += static_cast<int>(elapsedGameTime);
    gameStats.highScore = gameStats.highScore ? gameStats.highScore < static_cast<int>(elapsedGameTime) : static_cast<int>(elapsedGameTime);
    // Assume highScore logic is updated elsewhere

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


void Game::resetGame() {
    backgroundMusic.stop();
    // Очистка объектов игры
    snakes.clear();
    powerUps.clear();
    player->reset();
    isGameRunning = true;
    this->saveStatistics("statistics.txt");
    menu -> showWelcomeScreen(window, *this);

    std::cout << "Game Over" << std::endl;
}