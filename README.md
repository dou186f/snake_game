# Snake Game

A Snake game with single-player and local two-player versus modes, built in C++ using raylib.

## Features

- **Single Player** — Classic snake gameplay with score tracking
- **Versus Mode** — Two players on the same keyboard, collision detection between snakes
- **Input Buffering** — Queued directional inputs prevent missed keypresses at high speed
- **Menu & Game Over screens** — Keyboard and mouse navigation

## Controls

| Action | Player 1 | Player 2 |
|---|---|---|
| Move | Arrow Keys | W A S D |

## Build

Requires CMake 3.16+ and raylib 5.0 source placed locally.

1. Download raylib 5.0 source from [raylib releases](https://github.com/raysan5/raylib/releases/tag/5.0) and place the folder as `raylib-5.0/` in the project root.
2. Build:

```bash
mkdir build && cd build
cmake ..
make
./snake_game
```

## Tech Stack

- **Language:** C++
- **Graphics:** raylib 5.0
- **Build:** CMake

## Author

**Doğu Erbaş**  
doguerbass@gmail.com
