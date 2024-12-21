Wriggle is an arcade game where players control a character to navigate through obstacles avoiding snakes using the SFML library.

**Installation:**

Prerequisites. Make sure you have the following installed:
-A C++17-compatible compiler (e.g., g++ or clang++).
-SFML 2.5 or higher.
-AddressSanitizer (optional, used for debugging).

Steps

1.Clone the repository:

git clone https://github.com/username/wriggle.git
cd wriggle

2.Install SFML if not already installed. For macOS:

brew install sfml

For Linux:

sudo apt-get install libsfml-dev

3.Build the game:

make

4.Run the game:

make run

**Project Structure**

src/: Contains .cpp files with the implementation of the game logic.
include/: Contains .h files with declarations and definitions.
obj/: Directory for compiled object files (created during the build process).
Makefile: Automates the build process.

**Build and Run**

To build: Run make in the project root directory.

To run the game: Use make run. On macOS, this will automatically set the DYLD_LIBRARY_PATH for SFML.

To clean up: Remove compiled files with:

make clean

**Dependencies**

SFML (Simple and Fast Multimedia Library) for graphics, window, system, and audio.
AddressSanitizer (optional, for debugging memory issues).

