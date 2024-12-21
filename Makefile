# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -g -Iinclude

# Платформозависимые настройки
ifeq ($(OS), Windows_NT)
    # Windows
    LDFLAGS = -Lpath_to_sfml/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
    SFML_BIN = path_to_sfml/bin
    EXECUTABLE = arcade_game.exe
else
    # macOS/Linux
    UNAME_S := $(shell uname)
    ifeq ($(UNAME_S), Darwin)
        # macOS
        LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
        DYLD_EXPORT = export DYLD_LIBRARY_PATH=/opt/homebrew/lib:$$DYLD_LIBRARY_PATH && 
    else
        # Linux
        LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
    endif
    EXECUTABLE = arcade_game
endif

# Папки
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Файлы
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Основное правило
all: $(EXECUTABLE)

# Создание исполняемого файла
$(EXECUTABLE): $(OBJECTS)
ifeq ($(OS), Windows_NT)
	@$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)
	@cp $(SFML_BIN)/*.dll .
else
	@$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)
endif

# Компиляция объектных файлов
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Создание директории для объектных файлов
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Очистка
clean:
	@rm -rf $(OBJ_DIR) $(EXECUTABLE)
ifeq ($(OS), Windows_NT)
	@rm -f *.dll
endif

# Запуск игры
run: $(EXECUTABLE)
ifeq ($(OS), Windows_NT)
	@./$(EXECUTABLE)
else ifeq ($(UNAME_S), Darwin)
	@$(DYLD_EXPORT) ./$(EXECUTABLE)
else
	@./$(EXECUTABLE)
endif

# Условные цели
.PHONY: all clean run
