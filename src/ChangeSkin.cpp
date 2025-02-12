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

    // Кнопка "Back" с исходным цветом
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

    // Устанавливаем позицию и начальный масштаб для каждого спрайта
    for (int i = 0; i < numSkins; ++i) {
        skinSprites[i].setPosition(startX + i * (skinDisplayWidth + spacing), startY);
        skinSprites[i].setScale(DEFAULT_SKIN_SCALE, DEFAULT_SKIN_SCALE);
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
    
    // коэффициент интерполяции
    const float animationSpeed = 0.1f;

    // Флаг для отслеживания наведения для кнопки Back
    bool wasHoveredBack = false;
    
    // Лямбда для обновления анимации интерактивного текста
    auto updateInteractiveText = [&](sf::Text &text, bool &wasHovered, const sf::Color &defaultColor) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool isHovered = text.getGlobalBounds().contains(mousePos);
        float targetScale = isHovered ? 1.1f : 1.0f;
        sf::Color targetColor = isHovered ? HOVER_TEXT_COLOR : defaultColor;
        
        // Плавное изменение масштаба
        sf::Vector2f currentScale = text.getScale();
        float newScale = currentScale.x + (targetScale - currentScale.x) * animationSpeed;
        text.setScale(newScale, newScale);
        
        // Плавное изменение цвета
        sf::Color currentColor = text.getFillColor();
        auto lerp = [&](sf::Uint8 current, sf::Uint8 target) -> sf::Uint8 {
            float c = static_cast<float>(current);
            float t = static_cast<float>(target);
            c += (t - c) * animationSpeed;
            return static_cast<sf::Uint8>(c);
        };
        sf::Color newColor(
            lerp(currentColor.r, targetColor.r),
            lerp(currentColor.g, targetColor.g),
            lerp(currentColor.b, targetColor.b),
            255
        );
        text.setFillColor(newColor);
        wasHovered = isHovered;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);
                // Обработка клика по кнопке Back
                if (backButton.getGlobalBounds().contains(mousePosF)) {
                    return true;  // Возвращаемся в предыдущее меню
                }
                // Обработка клика по скинам
                for (size_t i = 0; i < skinSprites.size(); ++i) {
                    if (skinSprites[i].getGlobalBounds().contains(mousePosF)) {
                        selectedSkin = static_cast<int>(i);
                        std::cout << "Skin clicked: " << skinPaths[i] << std::endl;
                    }
                }
            }
        }
        
        // Обновляем анимацию для кнопки Back
        updateInteractiveText(backButton, wasHoveredBack, CHANGESKIN_BACK_BUTTON_COLOR);
        
        // Получаем позицию мыши для обновления скинов
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        // Вектор для хранения информации, над каким скином находится курсор
        std::vector<bool> skinHovered(skinSprites.size(), false);
        for (size_t i = 0; i < skinSprites.size(); ++i) {
            bool isHovered = skinSprites[i].getGlobalBounds().contains(mousePos);
            skinHovered[i] = isHovered;
            // Целевой масштаб для скина – базовый масштаб или увеличенный при наведении
            float targetScale = isHovered ? DEFAULT_SKIN_SCALE * 1.1f : DEFAULT_SKIN_SCALE;
            sf::Vector2f currentScale = skinSprites[i].getScale();
            float newScale = currentScale.x + (targetScale - currentScale.x) * animationSpeed;
            skinSprites[i].setScale(newScale, newScale);
            
            // Обновляем позицию подписи, чтобы она располагалась ниже изменённого спрайта
            sf::FloatRect spriteBounds = skinSprites[i].getGlobalBounds();
            skinLabels[i].setPosition(spriteBounds.left, spriteBounds.top + spriteBounds.height + 5);
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
                selectedText.setPosition(bounds.left, bounds.top - 25);
                window.draw(selectedText);
            }
            // Если скин не выбран, но курсор наведён — рисуем лавандовую обводку
            else if (skinHovered[i]) {
                sf::FloatRect bounds = skinSprites[i].getGlobalBounds();
                sf::RectangleShape hoverOutline(sf::Vector2f(bounds.width, bounds.height));
                hoverOutline.setPosition(bounds.left, bounds.top);
                hoverOutline.setFillColor(sf::Color::Transparent);
                hoverOutline.setOutlineThickness(2.f); // Толщина обводки при наведении
                hoverOutline.setOutlineColor(HOVER_TEXT_COLOR);
                window.draw(hoverOutline);
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