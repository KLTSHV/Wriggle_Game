#include "../include/ChangeSkin.h"
#include "../include/Constants.h" 
#include <iostream>
#include <algorithm>

bool ChangeSkin::show(sf::RenderWindow &window) {
    // Загрузка ресурсов (скинов и шрифта)
    if (!loadResources()) {
        std::cerr << "Failed to load resources for ChangeSkin!" << std::endl;
        return false;
    }
    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "Error loading font from " << FONT_PATH << std::endl;
        return false;
    }
    
    // Заголовок меню "Change Skin"
    sf::Text title("Change Skin", font, CHANGESKIN_TITLE_FONT_SIZE);
    title.setFillColor(CHANGESKIN_TITLE_COLOR);
    title.setPosition(CHANGESKIN_TITLE_POSITION);

    // Кнопка "Back"
    sf::Text backButton("Back", font, CHANGESKIN_BACK_BUTTON_FONT_SIZE);
    backButton.setFillColor(CHANGESKIN_BACK_BUTTON_COLOR);
    backButton.setPosition(CHANGESKIN_BACK_BUTTON_POSITION);

    // Настраиваем расположение скинов
    const float skinDisplayWidth = CHANGESKIN_LABEL_DISPLAY_WIDTH;
    const float skinDisplayHeight = CHANGESKIN_LABEL_DISPLAY_HEIGHT;
    const float spacing = CHANGESKIN_LABEL_SPACING;
    int numSkins = static_cast<int>(skinSprites.size());
    float totalWidth = numSkins * skinDisplayWidth + (numSkins - 1) * spacing;
    float startX = (window.getSize().x - totalWidth) / 2;
    float startY = CHANGESKIN_SKIN_PLACING_Y;

    // Устанавливаем позицию для каждого спрайта
    for (int i = 0; i < numSkins; ++i) {
        skinSprites[i].setPosition(startX + i * (skinDisplayWidth + spacing), startY);
    }
    
    // Для каждого скина создаём подпись
    std::vector<sf::Text> skinLabels;
    for (int i = 0; i < numSkins; ++i) {
        sf::Text label("Skin " + std::to_string(i + 1), font, CHANGESKIN_SKIN_FONT_SIZE);
        label.setFillColor(CHANGESKIN_TITLE_COLOR);
        sf::FloatRect spriteBounds = skinSprites[i].getGlobalBounds();
        // Размещаем подпись чуть ниже спрайта
        label.setPosition(spriteBounds.left, spriteBounds.top + spriteBounds.height + 5);
        skinLabels.push_back(label);
    }

    // Основной цикл обработки событий меню
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Закрытие окна
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            // Обработка клика мышью
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                // Если клик по кнопке "Back" — выходим из меню
                if (backButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    return true;  // Возвращаемся в предыдущее меню
                }
                // Проверяем клики по каждому скину
                for (size_t i = 0; i < skinSprites.size(); ++i) {
                    if (skinSprites[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        selectedSkin = static_cast<int>(i);
                        std::cout << "Skin clicked: " << skinPaths[i] << std::endl;
                    }
                }
            }
        }
        
        window.clear();

        // Рисуем заголовок и кнопку Back
        window.draw(title);
        window.draw(backButton);

        // Рисуем каждый скин и его подпись
        for (size_t i = 0; i < skinSprites.size(); ++i) {
            // Если скин выбран, рисуем белую рамку и текст "Selected"
            if (static_cast<int>(i) == selectedSkin) {
                sf::FloatRect bounds = skinSprites[i].getGlobalBounds();
                sf::RectangleShape outline(sf::Vector2f(bounds.width, bounds.height));
                outline.setPosition(bounds.left, bounds.top);
                outline.setFillColor(sf::Color::Transparent);
                outline.setOutlineThickness(CHANGESKIN_SELECTED_OUTLINE_THICKNESS);
                outline.setOutlineColor(sf::Color::White);
                window.draw(outline);
                
                sf::Text selectedText("Selected", font, CHANGESKIN_SELECTED_FONT_SIZE);
                selectedText.setFillColor(CHANGESKIN_SELECTED_COLOR);
                // Размещаем текст чуть выше спрайта
                selectedText.setPosition(bounds.left, bounds.top - 25);
                window.draw(selectedText);
            }
            window.draw(skinSprites[i]);
            window.draw(skinLabels[i]);
        }
        
        window.display();
    }
    
    return false;
}

bool ChangeSkin::loadResources() {
    // Здесь можно добавить загрузку других ресурсов, если необходимо.
    return loadSkins();
}

bool ChangeSkin::loadSkins() {
    // Очищаем вектора, чтобы не было дублирования при повторном входе в меню
    skinPaths.clear();
    skinTextures.clear();
    skinSprites.clear();

    // Загружаем пути из константы (например, CHANGESKIN_SKIN_PATHS)
    skinPaths = CHANGESKIN_SKIN_PATHS;
    if (skinPaths.empty()) {
        std::cerr << "No skins found to load!" << std::endl;
        return false;
    }
    selectedSkin = 0;

    // Резервируем память, чтобы избежать переаллокаций
    skinTextures.reserve(skinPaths.size());
    skinSprites.reserve(skinPaths.size());

    for (const auto &path : skinPaths) {
        sf::Texture texture;
        if (!texture.loadFromFile(path)) {
            std::cerr << "Error loading texture from: " << path << std::endl;
            continue;  // Пропускаем неудачные загрузки
        }
        skinTextures.push_back(std::move(texture));

        sf::Sprite sprite;
        sprite.setTexture(skinTextures.back());
        // Масштабирование: приводим спрайт к фиксированному размеру
        float spriteWidth = sprite.getLocalBounds().width;
        float spriteHeight = sprite.getLocalBounds().height;
        if (spriteWidth == 0 || spriteHeight == 0) {
            std::cerr << "Invalid sprite bounds for texture: " << path << std::endl;
            continue;
        }
        float scaleX = CHANGESKIN_LABEL_DISPLAY_WIDTH / spriteWidth;
        float scaleY = CHANGESKIN_LABEL_DISPLAY_HEIGHT / spriteHeight;
        float scaleFactor = std::min(scaleX, scaleY);
        sprite.setScale(scaleFactor, scaleFactor);

        skinSprites.push_back(sprite);
    }
    if (skinSprites.empty()) {
        std::cerr << "No valid skin sprites loaded!" << std::endl;
        return false;
    }
    return true;
}