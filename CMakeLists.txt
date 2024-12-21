cmake_minimum_required(VERSION 3.15)

project(Wriggle LANGUAGES CXX)

# Укажите стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включить отладочные символы
set(CMAKE_BUILD_TYPE Debug)

# Определение архитектуры
if(APPLE)
    # Универсальная сборка для Mac
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for Mac")
endif()

# Добавить поддержку AddressSanitizer (опционально)
option(ENABLE_SANITIZER "Enable AddressSanitizer for debugging" OFF)

if(ENABLE_SANITIZER)
    add_compile_options(-fsanitize=address -g)
    add_link_options(-fsanitize=address)
endif()

# Папки с исходными файлами и заголовками
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
set(OBJ_DIR ${CMAKE_BINARY_DIR}/obj)

# Подключение всех исходных файлов
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

# Создаём исполняемый файл
add_executable(Wriggle ${SOURCES})

# Указать, где искать заголовочные файлы
target_include_directories(Wriggle PRIVATE ${INC_DIR})

# Подключение SFML через FetchContent
include(FetchContent)
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)

FetchContent_MakeAvailable(sfml)

# Связывание с библиотеками SFML
target_link_libraries(Wriggle PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

# Настройки для Windows
if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

# Настройки для Unix/Linux
if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(Wriggle PRIVATE X11)
endif()

# Вывод информации о сборке
message(STATUS "Building for: ${CMAKE_SYSTEM_NAME}")
if(APPLE)
    message(STATUS "Architecture: ${CMAKE_OSX_ARCHITECTURES}")
endif()

# Условие для библиотек на MacOS
if(APPLE)
    set(DYLD_LIBRARY_PATH "/opt/homebrew/lib")
    message(STATUS "DYLD_LIBRARY_PATH set to: ${DYLD_LIBRARY_PATH}")
endif()

# Добавление правил для запуска
add_custom_target(run
    COMMAND Wriggle
    DEPENDS Wriggle
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
