# Snake Game

A Snake game with single-player and local two-player versus modes, built in C++17 using raylib.

## Features

- **Single Player** - Eat food, grow, and increase your score as movement speeds up
- **Versus Mode** - Two players on one keyboard, with collisions between snakes
- **Input Buffering** - Queued turns with reverse-direction protection
- **Board Completion** - Filling the board ends the game with a win screen
- **Menus** - Keyboard navigation, restart, and return to the menu

## Controls

| Action | Key |
|---|---|
| Player 1 movement | Arrow keys |
| Player 2 movement | W / A / S / D |
| Menu selection | Up / Down, then Enter |
| Restart after a round | R |
| Return to menu after a round | Esc |

In versus mode, the surviving player wins. If both lose together, or the board is completed, the higher score wins; equal scores produce a draw.

## Build & Run

Requires CMake 3.21+ and a C++17 compiler. Raylib 5.0 is downloaded automatically during the first configuration.

Run from the project root.

### Windows (MinGW-w64)

Have MinGW-w64's `gcc`, `g++`, and `mingw32-make` on your PATH, before any other GCC installation.

```powershell
cmake -S . -B build-mingw -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build-mingw
.\build-mingw\snake_game.exe
```

### Linux / macOS

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/snake_game
```

Linux requires the [raylib system dependencies](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux). On macOS, use the Xcode command-line tools.

## Tech Stack

- **Language:** C++17
- **Graphics:** raylib 5.0
- **Build:** CMake
