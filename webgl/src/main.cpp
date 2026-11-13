#include "raylib.h"
#include "Snake.h"
#include "Food.h"
#include "Constant.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

enum class GameState { MENU, PLAYING, GAMEOVER };

struct Game {
    Snake snake;
    Snake snake2{10, 15, Direction::RIGHT};
    Food food{snake};
    GameState state = GameState::MENU;
    int selectedButton = 0;
    int score = 0;
    int score2 = 0;
    bool isTwoPlayer = false;
    bool shouldQuit = false;
    bool boardCompleted = false;
    float moveTimer1 = 0.0f;
    float moveTimer2 = 0.0f;
    float moveInterval1 = 0.1f;
    float moveInterval2 = 0.1f;

    void reset(bool twoPlayer) {
        snake = Snake();
        snake2 = Snake(10, 15, Direction::RIGHT);
        score = score2 = 0;
        moveTimer1 = moveTimer2 = 0.0f;
        moveInterval1 = moveInterval2 = 0.1f;
        isTwoPlayer = twoPlayer;
        boardCompleted = false;
        food.respawn(snake, isTwoPlayer ? &snake2 : nullptr);
        state = GameState::PLAYING;
    }

    void handleInput() {
        if (IsKeyPressed(KEY_UP)) snake.queueDirection(Direction::UP);
        if (IsKeyPressed(KEY_DOWN)) snake.queueDirection(Direction::DOWN);
        if (IsKeyPressed(KEY_LEFT)) snake.queueDirection(Direction::LEFT);
        if (IsKeyPressed(KEY_RIGHT)) snake.queueDirection(Direction::RIGHT);
        if (isTwoPlayer) {
            if (IsKeyPressed(KEY_W)) snake2.queueDirection(Direction::UP);
            if (IsKeyPressed(KEY_S)) snake2.queueDirection(Direction::DOWN);
            if (IsKeyPressed(KEY_A)) snake2.queueDirection(Direction::LEFT);
            if (IsKeyPressed(KEY_D)) snake2.queueDirection(Direction::RIGHT);
        }
    }

    void advance(float frameTime) {
        float remaining = frameTime;
        while (remaining > 0.0f && state == GameState::PLAYING) {
            float nextMove = std::max(0.0f, moveInterval1 - moveTimer1);
            if (isTwoPlayer) {
                nextMove = std::min(nextMove, std::max(0.0f, moveInterval2 - moveTimer2));
            }
            float elapsed = std::min(remaining, nextMove);
            moveTimer1 += elapsed;
            if (isTwoPlayer) moveTimer2 += elapsed;
            remaining -= elapsed;

            bool moved1 = moveTimer1 + 0.000001f >= moveInterval1;
            bool moved2 = isTwoPlayer && moveTimer2 + 0.000001f >= moveInterval2;
            if (!moved1 && !moved2) break;

            if (moved1) {
                moveTimer1 = std::max(0.0f, moveTimer1 - moveInterval1);
                snake.move();
            }
            if (moved2) {
                moveTimer2 = std::max(0.0f, moveTimer2 - moveInterval2);
                snake2.move();
            }

            bool ate1 = moved1 && snake.body.front().x == food.position.x && snake.body.front().y == food.position.y;
            bool ate2 = moved2 && snake2.body.front().x == food.position.x && snake2.body.front().y == food.position.y;
            if (ate1) snake.grow();
            if (ate2) snake2.grow();

            snake.isDead = snake.checkSelfCollision() || snake.checkWallCollision();
            if (isTwoPlayer) {
                snake.isDead = snake.isDead || snake.checkOtherCollision(snake2);
                snake2.isDead = snake2.checkSelfCollision() || snake2.checkWallCollision() || snake2.checkOtherCollision(snake);
            }
            if (snake.isDead || (isTwoPlayer && snake2.isDead)) {
                state = GameState::GAMEOVER;
                break;
            }

            if (ate1) {
                ++score;
                moveInterval1 = std::max(0.05f, moveInterval1 - 0.001f);
            }
            if (ate2) {
                ++score2;
                moveInterval2 = std::max(0.05f, moveInterval2 - 0.001f);
            }
            if ((ate1 || ate2) && !food.respawn(snake, isTwoPlayer ? &snake2 : nullptr)) {
                boardCompleted = true;
                state = GameState::GAMEOVER;
            }
        }
    }

    void drawSnake(const Snake& player, Color headColor, Color bodyColor) const {
        for (std::size_t i = 0; i < player.body.size(); ++i) {
            DrawRectangle(player.body[i].x * cellSize, player.body[i].y * cellSize,
                          cellSize, cellSize, i == 0 ? headColor : bodyColor);
        }
    }

    void drawBoard() const {
        for (int x = 2; x < cols; ++x) {
            for (int y = 2; y < rows; ++y) {
                Color color = (x + y) % 2 == 0 ? Color{20, 20, 20, 255} : Color{30, 30, 30, 255};
                DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, color);
            }
        }
        drawSnake(snake, GREEN, DARKGREEN);
        if (isTwoPlayer) drawSnake(snake2, RED, Color{139, 0, 0, 255});
        DrawRectangle(food.position.x * cellSize, food.position.y * cellSize, cellSize, cellSize, WHITE);
        if (isTwoPlayer) {
            DrawText(TextFormat("Score Green: %d", score), 10, 10, 20, GREEN);
            DrawText(TextFormat("Score Red: %d", score2), screenWidth - 200, 10, 20, RED);
        } else {
            DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
        }
    }

    void drawMenu() const {
        DrawText("SNAKE GAME", 300, 165, 30, GREEN);
        DrawText("PLAY", 300, 235, 20, selectedButton == 0 ? GREEN : DARKGREEN);
        DrawText("VERSUS MODE", 300, 295, 20, selectedButton == 1 ? GREEN : DARKGREEN);
        DrawText("QUIT", 300, 355, 20, selectedButton == 2 ? RED : Color{139, 0, 0, 255});
        DrawText(">", 280, 235 + selectedButton * 60, 20, WHITE);
    }

    void drawResult() const {
        if (boardCompleted) {
            DrawText(isTwoPlayer ? "BOARD COMPLETE!" : "YOU WIN!", 280, 210, 30, GREEN);
            DrawText("You finished the game!", 270, 250, 20, WHITE);
        } else {
            DrawText("GAME OVER", 320, 240, 20, RED);
        }
        if (isTwoPlayer) {
            if (snake.isDead && !snake2.isDead) {
                DrawText("RED WINS!", 330, 280, 20, RED);
            } else if (snake2.isDead && !snake.isDead) {
                DrawText("GREEN WINS!", 330, 280, 20, GREEN);
            } else if (score > score2) {
                DrawText("GREEN WINS!", 330, 280, 20, GREEN);
            } else if (score2 > score) {
                DrawText("RED WINS!", 330, 280, 20, RED);
            } else {
                DrawText("DRAW!", 360, 280, 20, WHITE);
            }
            DrawText(TextFormat("Score Green: %d", score), 320, 310, 20, GREEN);
            DrawText(TextFormat("Score Red: %d", score2), 320, 335, 20, RED);
        } else {
            DrawText(TextFormat("Score: %d", score), 340, 300, 20, WHITE);
        }
        DrawText("Press ESC to go to MENU", 270, 380, 20, GRAY);
        DrawText("Press R to RESTART", 270, 420, 20, GRAY);
    }

    void frame() {
        if (state == GameState::MENU) {
            if (IsKeyPressed(KEY_DOWN)) selectedButton = (selectedButton + 1) % 3;
            if (IsKeyPressed(KEY_UP)) selectedButton = (selectedButton + 2) % 3;
            if (IsKeyPressed(KEY_ENTER)) {
                if (selectedButton == 2) shouldQuit = true;
                else reset(selectedButton == 1);
            }
        } else if (state == GameState::PLAYING) {
            handleInput();
            advance(GetFrameTime());
        } else {
            if (IsKeyPressed(KEY_ESCAPE)) state = GameState::MENU;
            else if (IsKeyPressed(KEY_R)) reset(isTwoPlayer);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        if (state == GameState::MENU) drawMenu();
        else if (state == GameState::PLAYING) drawBoard();
        else drawResult();
        EndDrawing();
    }
};

#ifdef __EMSCRIPTEN__
void webFrame(void* context) {
    Game& game = *static_cast<Game*>(context);
    if (WindowShouldClose() || game.shouldQuit) {
        emscripten_cancel_main_loop();
        CloseWindow();
        return;
    }
    game.frame();
}
#endif

int main() {
    InitWindow(screenWidth, screenHeight, "Snake Game");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    static Game game;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(webFrame, &game, 0, true);
#else
    while (!WindowShouldClose() && !game.shouldQuit) game.frame();
    CloseWindow();
#endif
    return 0;
}
