# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -fsanitize=address -g -Iinclude

# Платформо-зависимые настройки
ifeq ($(OS), Windows_NT)
    LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
    EXECUTABLE_EXT = .exe
    RM = del /Q /F
    MKDIR = if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
else
    LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
    EXECUTABLE_EXT =
    RM = rm -rf
    MKDIR = mkdir -p
endif

# Папки
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Файлы
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
EXECUTABLE = arcade_game$(EXECUTABLE_EXT)

# Условие для библиотеки на macOS
ifeq ($(shell uname), Darwin)
    DYLD_EXPORT = export DYLD_LIBRARY_PATH=/opt/homebrew/lib:$$DYLD_LIBRARY_PATH &&
else
    DYLD_EXPORT =
endif

# Основное правило
all: $(EXECUTABLE)

# Создание исполняемого файла
$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

# Компиляция объектных файлов
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Создание директории для объектных файлов
$(OBJ_DIR):
	$(MKDIR)

# Очистка
clean:
	$(RM) $(OBJ_DIR)\* $(EXECUTABLE)

# Запуск игры
run: $(EXECUTABLE)
	$(DYLD_EXPORT) ./$(EXECUTABLE)

# Условные цели
.PHONY: all clean run
