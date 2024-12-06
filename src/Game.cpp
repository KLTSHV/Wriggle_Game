#include "/Users/egorkoltysev/Desktop/PROG/Wriggle/include/Game.h"
#include <iostream>
#include <cmath>
#include <random>
#include "/Users/egorkoltysev/Desktop/PROG/Wriggle/include/PowerUp.h"  
#include "/Users/egorkoltysev/Desktop/PROG/Wriggle/include/Player.h"


Game::Game() 
    : window(sf::VideoMode(800, 600), "Arcade Game"), 
      snakeSpawnTimer(0), powerUpSpawnTimer(0), 
      isGameRunning(false), difficultyLevelTimer(0.0f), difficultyLevel(0), gameClock(), wallSpawnTimer(0) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    player = std::make_unique<Player>(); 
    menu = std::make_unique<Menu>();
    std::cout << "Error here 1" << std::endl;
    player->setPosition(400, 300);
    std::cout << "Error here 2" << std::endl;

    font.loadFromFile("assets/arial.ttf");  

    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(10, 10);

    if (!backgroundMusic.openFromFile("assets/BackgroundAudio.mp3")) {
        std::cout << "Error: Failed to load background music!" << std::endl;
    } else {
        backgroundMusic.setLoop(true);  
        backgroundMusic.play();         
    }
    // Шкала прогресса
    progressBar.setSize(sf::Vector2f(200.f, 15.f)); // Уменьшенный размер шкалы
    progressBar.setFillColor(sf::Color::Red);
    progressBar.setPosition(10, 50); // Положение на экране

    // Обводка шкалы прогресса
    progressBarOutline.setSize(sf::Vector2f(200.f, 15.f));
    progressBarOutline.setOutlineThickness(2.f); // Толщина обводки
    progressBarOutline.setOutlineColor(sf::Color::White); // Цвет обводки
    progressBarOutline.setFillColor(sf::Color::Transparent); // Прозрачная внутренняя часть
    progressBarOutline.setPosition(progressBar.getPosition()); // Совпадение с положением шкалы

    elapsedGameTime = 0.f;
    progressBarFill = 0.f;

   snakeSpawnDuration = 3.0f;
    powerUpSpawnDuration = 9.0f;
    menu->showWelcomeScreen(window);
    isGameRunning = true;
}


void Game::run() {
    while (window.isOpen()) {
        processEvents();
        if (isGameRunning) {
            update();
            render();
        }
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (isGameRunning) {
            if (event.type == sf::Event::KeyPressed) {
                float speed = player->returnSpeed();
                float diagonalFactor = 1.0f / std::sqrt(2.0f); // Нормализация диагональной скорости
                
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
    timerText.setString("Time: " + std::to_string(static_cast<int>(elapsedGameTime)) + "s");

    // Обновление прогресса
    progressBarFill += elapsedTime * (200.f / 30.f);
    if (progressBarFill >= 200.f) {
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
    if(difficultyLevelTimer >= 30){
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
    std::uniform_real_distribution<float> distLength(100.0f, 300.0f);
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

void Game::resetGame() {
    backgroundMusic.stop();
    // Очистка объектов игры
    snakes.clear();
    powerUps.clear();
    player->reset();
    isGameRunning = true;
    std::cout << "Game Over" << std::endl;
}