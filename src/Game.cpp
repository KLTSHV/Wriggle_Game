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
        // Обработка дэша при нажатии LShift
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
                dashDirection = 0; // Значение по умолчанию (Up)
            
            // Передаём также walls для проверки столкновений
            player->dash(dashDirection, walls);
        }
    }
}




void Game::update() {
    // Получаем время между кадрами (delta time)
    float deltaTime = gameClock.restart().asSeconds();

    // Обновление таймеров и прочего
    dashTimer += deltaTime;
    snakeSpawnTimer += deltaTime;
    powerUpSpawnTimer += deltaTime;
    difficultyLevelTimer += deltaTime;
    player->updateTimers(deltaTime);
    if (!erasingSnakesProcess && !snakes.empty()){
        for (const auto& snake : snakes){
            if (snake) {
                snake->update(deltaTime);
                isSnakesStopped = snake->isStopped();
            }
        }
    }
    elapsedGameTime += deltaTime;
    timerText.setString("Time: " + std::to_string(static_cast<int>(elapsedGameTime)) + "s");

    // Обновление прогресс-бара
    progressBarFill += deltaTime * PROGRESS_INCREMENT;
    if (progressBarFill >= PROGRESS_BAR_WIDTH) {
        progressBarFill = 0.f;
    }
    progressBar.setSize(sf::Vector2f(progressBarFill, progressBar.getSize().y));

    // Обработка ввода для движения игрока (см. следующий пункт)
    handlePlayerMovement(deltaTime);

    // Остальные обновления (спавн змей, бонусов, столкновения и т.д.)
    if (!erasingSnakesProcess){
        if (snakeSpawnTimer >= snakeSpawnDuration && !isSnakesStopped) {
            spawnSnake();
            std::cout << "Snake Spawned!" << std::endl;
            snakeSpawnTimer = 0;
            speedOfStoppedSnakes.clear();
        }
    }
    if (!powerUps.empty()){
        powerUpSpawnTimer = 0;
    }
    if (powerUpSpawnTimer >= powerUpSpawnDuration) {
        spawnPowerUp();
        powerUpSpawnTimer = 0;
    }
    if (!snakes.empty()){
        for (auto& snake : snakes) {
            if (snake){
                snake->update(deltaTime);
            }
        }
    }
    if (difficultyLevelTimer >= DIFFICULTY_LEVEL_UP_INTERVAL){
        adjustDifficultyLevel();
        difficultyLevelTimer = 0;
        walls.clear();
        int amountOfWall = rand() % 5 + 2;
        for (int i = 0; i < amountOfWall; i++){
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

    window.draw(*player);
    for (const auto& snake : snakes)
        window.draw(*snake);
    for (const auto& wall : walls)
        window.draw(*wall);
    // Рисуем бонусы поверх стен
    for (const auto& powerUp : powerUps)
        window.draw(*powerUp);

    window.draw(timerText);
    window.draw(progressBar);
    window.draw(progressBarOutline);

    window.display();
}


void Game::spawnSnake() {
    if(!erasingSnakesProcess){
    

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
    std::uniform_real_distribution<float> distSpeed(SNAKE_SPEED_MIN, SNAKE_SPEED_MAX);
    std::uniform_real_distribution<float> distSize(8.0f, 20.0f);
    
    float speed = distSpeed(gen);
    float size = distSize(gen); // Размер сегментов
    int numSegments = (rand()%20)+5;
    angle = angle * 3.14159f / 180.0f;  // Преобразуем угол в радианы

    auto newSnake = std::make_unique<Snake>(speed, angle, numSegments, size);
    newSnake->setPosition(sf::Vector2f(x, y));
    std::cout << "Snake Spawned" << std::endl;
    
    

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
    if (!snakes.empty()) {
        for (auto it = snakes.begin(); it != snakes.end(); ) {
            if (player->getGlobalBounds().intersects((*it)->getGlobalBounds())) { 
                std::cout << "Collision detected with snake!" << std::endl;
                if (!player->isInvincible()) {
                    std::cout << "Player is not invincible, resetting game..." << std::endl;
                    resetGame();
                    return; // Немедленно выходим, чтобы не продолжать итерацию по очищенному контейнеру
                }
            } else {
                ++it;
            }
        }
    }

    // Столкновение с бонусами 
    for (auto it = powerUps.begin(); it != powerUps.end(); ) {
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
        bool isVertical = orientationDist(gen) == 1;

        newWall->setPosition(x, y);
        newWall->setSize(length, isVertical);

        // Если стена не пересекается с игроком, позиция допустима:
        if (!player->getGlobalBounds().intersects(newWall->getGlobalBounds())) {
            validPosition = true;
        }
        attempt++;
    }
    // Добавляем стену (если положение не найдено, стена может пересекаться с игроком)
    walls.push_back(std::move(newWall));
}

void Game::handleWallCollisions() {
    sf::FloatRect playerBounds = player->getGlobalBounds();
    sf::Vector2f pos = player->sprite.getPosition();
    
    for (const auto& wall : walls) {
        sf::FloatRect wallBounds = wall->getGlobalBounds();
        
        if (playerBounds.intersects(wallBounds)) {
            // Вычисляем перекрывающийся прямоугольник
            float intersectLeft   = std::max(playerBounds.left, wallBounds.left);
            float intersectRight  = std::min(playerBounds.left + playerBounds.width, wallBounds.left + wallBounds.width);
            float intersectTop    = std::max(playerBounds.top, wallBounds.top);
            float intersectBottom = std::min(playerBounds.top + playerBounds.height, wallBounds.top + wallBounds.height);
            
            float intersectWidth  = intersectRight - intersectLeft;
            float intersectHeight = intersectBottom - intersectTop;
            
            // Разрешаем столкновение по оси с меньшим проникновением
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
            playerBounds = player->getGlobalBounds(); // обновляем границы после коррекции
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
void Game::handlePlayerMovement(float deltaTime) {
    float speed = player->returnSpeed(); // Убедитесь, что INITIAL_SPEED имеет разумное значение
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

    // Если одновременно нажаты две клавиши, нормализуем скорость (например, умножая на коэффициент)
    if (moveOffset.x != 0.f && moveOffset.y != 0.f) {
        moveOffset *= DIAGONAL_SPEED_FACTOR;  // Обычно DIAGONAL_SPEED_FACTOR равен 0.7071
    }

    // Передвигаем игрока с учётом delta time
    player->move(moveOffset.x, moveOffset.y, walls, deltaTime);
}




void Game::resetGame() {
    backgroundMusic.stop();
    snakes.clear();
    powerUps.clear();
    walls.clear();
    player->reset();
    isGameRunning = false; // Останавливаем игровой процесс перед перезапуском

    // Сброс прогресс-бара
    progressBarFill = 0.0f;
    progressBar.setSize(sf::Vector2f(PROGRESS_BAR_WIDTH, progressBar.getSize().y));

    this->saveStatistics("statistics.txt");
    
    // Показываем главное меню и ждём выбора пользователя
    bool newGame = menu->showWelcomeScreen(window, *this);
    if (newGame) {
         // Сброс игровых переменных
         snakeSpawnTimer = 0;
         powerUpSpawnTimer = 0;
         difficultyLevelTimer = 0;
         elapsedGameTime = 0;
         gameClock.restart();
         isGameRunning = true;
         backgroundMusic.play();
    } else {
         window.close();
    }
    
    std::cout << "Game Over" << std::endl;
}
