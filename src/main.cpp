#include "raylib.h"
#include "Snake.h"
#include "Food.h"
#include <cstdlib>
#include <ctime>
#include "Constant.h"

enum class GameState {
	MENU,
	PLAYING,
	TWOPLAYER,
	GAMEOVER
};

int main() {

	int selectedButton = 0;

	int score = 0;
	int score2 = 0;

	bool isTwoPlayer = false;
	bool shouldQuit = false;

	InitWindow(screenWidth, screenHeight, "Snake Game");
	SetExitKey(KEY_NULL);
	SetTargetFPS(60);

	Snake snake;
	Snake snake2(10, 15, Direction::RIGHT);

	float moveTimer = 0.0f;
	float moveInterval = 0.1f;

	GameState state = GameState::MENU;
	
	srand(time(nullptr));
	Food food(snake);

	while (!WindowShouldClose() && !shouldQuit) {
		if (state == GameState::MENU) {
			if(IsKeyPressed(KEY_DOWN)) selectedButton = (selectedButton + 1) % 3;
			if(IsKeyPressed(KEY_UP)) selectedButton = (selectedButton + 2) % 3;
			
			if (IsKeyPressed(KEY_ENTER)) {
				if (selectedButton == 0) { state = GameState::PLAYING; isTwoPlayer = false; }
				if (selectedButton == 1) { state = GameState::TWOPLAYER; isTwoPlayer = true; }
				if (selectedButton == 2) shouldQuit = true;
			}

			Rectangle playButton = {300, 200, 200, 50};
			Rectangle twoPlayerButton = {300, 260, 200, 50};
			Rectangle quitButton = {300, 320, 200, 50};

			Vector2 mouse = GetMousePosition();
			if (CheckCollisionPointRec(mouse, playButton)) selectedButton = 0;
			if (CheckCollisionPointRec(mouse, twoPlayerButton)) selectedButton = 1;
			if (CheckCollisionPointRec(mouse, quitButton)) selectedButton = 2;

			if (IsMouseButtonPressed(0)) {
				if (selectedButton == 0) { state = GameState::PLAYING; isTwoPlayer = false; }	
				if (selectedButton == 1) { state = GameState::TWOPLAYER; isTwoPlayer = true; }
				if (selectedButton == 2) shouldQuit = true;
			}

			Color playColor = (selectedButton == 0) ? GREEN : DARKGREEN;
			Color twoPlayerColor = (selectedButton == 1) ? GREEN : DARKGREEN;	
			Color quitColor = (selectedButton == 2) ? RED : Color{139, 0, 0, 255};

			BeginDrawing();
				ClearBackground(BLACK);
				DrawText("SNAKE GAME", 300, 150, 30, GREEN);
				
				DrawRectangleRec(playButton, playColor);
				DrawText("PLAY", 370, 215, 20, BLACK);
			
				DrawRectangleRec(twoPlayerButton, twoPlayerColor);	
				DrawText("VERSUS MODE", 320, 275, 20, BLACK);

				DrawRectangleRec(quitButton, quitColor);		
				DrawText("QUIT", 370, 335, 20, BLACK);
			EndDrawing();
		}
		
		else if (state == GameState::PLAYING) {
			if (IsKeyPressed(KEY_UP) && snake.lastMovedDir != Direction::DOWN)
				snake.inputBuffer.push(Direction::UP);
			if (IsKeyPressed(KEY_DOWN) && snake.lastMovedDir != Direction::UP)
				snake.inputBuffer.push(Direction::DOWN);
			if (IsKeyPressed(KEY_LEFT) && snake.lastMovedDir != Direction::RIGHT)		
				snake.inputBuffer.push(Direction::LEFT);
			if (IsKeyPressed(KEY_RIGHT) && snake.lastMovedDir != Direction::LEFT)	
				snake.inputBuffer.push(Direction::RIGHT);
			
			moveTimer += GetFrameTime();
			if (moveTimer >= moveInterval) {
				moveTimer = 0.0f;
				snake.move();
			}

			if (snake.body.front().x == food.position.x && snake.body.front().y == food.position.y) {
				snake.grow();
				food.respawn(snake);
				score++;
			}
	
			if (snake.checkSelfCollision() || snake.checkWallCollision()) {
				state = GameState::GAMEOVER;
			}
		
			BeginDrawing();
				ClearBackground(BLACK);
				for (int x = 2; x < cols; x++) {
 					for (int y = 2; y < rows; y++) {
				        Color cellColor = ((x + y) % 2 == 0) ? Color{20, 20, 20, 255} : Color{30, 30, 30, 255};
				        DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, cellColor);
				   	}		
				}
			
				for (int i = 0; i < snake.body.size(); i++) {
					Color color = (i == 0) ? GREEN : DARKGREEN;
					DrawRectangle(
						snake.body[i].x * cellSize,
						snake.body[i].y * cellSize,
						cellSize,
						cellSize,
						color			
					);
				}
			
				DrawRectangle(
					food.position.x * cellSize,
			   		food.position.y * cellSize,
					cellSize,
					cellSize,
					WHITE		
				);
	
				DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
			EndDrawing();
		}

		else if (state == GameState::TWOPLAYER) {
			if (IsKeyPressed(KEY_UP) && snake.lastMovedDir != Direction::DOWN)
				snake.inputBuffer.push(Direction::UP);
			if (IsKeyPressed(KEY_DOWN) && snake.lastMovedDir != Direction::UP)
				snake.inputBuffer.push(Direction::DOWN);
			if (IsKeyPressed(KEY_LEFT) && snake.lastMovedDir != Direction::RIGHT)		
				snake.inputBuffer.push(Direction::LEFT);
			if (IsKeyPressed(KEY_RIGHT) && snake.lastMovedDir != Direction::LEFT)	
				snake.inputBuffer.push(Direction::RIGHT);

			if (IsKeyPressed(KEY_W) && snake2.lastMovedDir != Direction::DOWN)
				snake2.inputBuffer.push(Direction::UP);
			if (IsKeyPressed(KEY_S) && snake2.lastMovedDir != Direction::UP)
				snake2.inputBuffer.push(Direction::DOWN);
			if (IsKeyPressed(KEY_A) && snake2.lastMovedDir != Direction::RIGHT)		
				snake2.inputBuffer.push(Direction::LEFT);
			if (IsKeyPressed(KEY_D) && snake2.lastMovedDir != Direction::LEFT)	
				snake2.inputBuffer.push(Direction::RIGHT);
			
			moveTimer += GetFrameTime();
			if (moveTimer >= moveInterval) {
				moveTimer = 0.0f;
				snake.move();
				snake2.move();
			}

			if (snake.body.front().x == food.position.x && snake.body.front().y == food.position.y) {
				snake.grow();
				food.respawn(snake, &snake2);
				score++;
			}

			if (snake2.body.front().x == food.position.x && snake2.body.front().y == food.position.y) {
				snake2.grow();
				food.respawn(snake, &snake2);
				score2++;
			}
	
			if (snake.checkSelfCollision() || snake.checkWallCollision() || snake.checkOtherCollision(snake2)) {
				state = GameState::GAMEOVER;
				snake.isDead = true;
			}
			if (snake2.checkSelfCollision() || snake2.checkWallCollision() || snake2.checkOtherCollision(snake)) {
				state = GameState::GAMEOVER;
				snake2.isDead = true;
			}
		
			BeginDrawing();
				ClearBackground(BLACK);
				for (int x = 2; x < cols; x++) {
 					for (int y = 2; y < rows; y++) {
				        Color cellColor = ((x + y) % 2 == 0) ? Color{20, 20, 20, 255} : Color{30, 30, 30, 255};
				        DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, cellColor);
				   	}		
				}
			
				for (int i = 0; i < snake.body.size(); i++) {
					Color color = (i == 0) ? GREEN : DARKGREEN;
					DrawRectangle(
						snake.body[i].x * cellSize,
						snake.body[i].y * cellSize,
						cellSize,
						cellSize,
						color			
					);
				}

				for (int i = 0; i < snake2.body.size(); i++) {
					Color color = (i == 0) ? RED : Color{139, 0, 0, 255};
					DrawRectangle(
						snake2.body[i].x * cellSize,
						snake2.body[i].y * cellSize,
						cellSize,
						cellSize,
						color			
					);
				}
			
				DrawRectangle(
					food.position.x * cellSize,
			   		food.position.y * cellSize,
					cellSize,
					cellSize,
					WHITE		
				);
	
				DrawText(TextFormat("Score Green: %d", score), 10, 10, 20, GREEN);
				DrawText(TextFormat("Score Red: %d", score2), screenWidth - 200, 10, 20, RED);
			EndDrawing();
		}
		
		else if (state == GameState::GAMEOVER) {
			BeginDrawing();
				ClearBackground(BLACK);
           			DrawText("GAME OVER", 320, 240, 20, RED);

					if (isTwoPlayer) {
           				if (snake.isDead && !snake2.isDead)
                			DrawText("RED WINS!", 340, 270, 20, RED);
            			else if (snake2.isDead && !snake.isDead)
                			DrawText("GREEN WINS!", 340, 270, 20, GREEN);
           				else
          					DrawText("DRAW!", 360, 270, 20, WHITE);

						DrawText(TextFormat("Score Green: %d", score), 340, 300, 20, GREEN);
						DrawText(TextFormat("Score Red: %d", score2), 340, 325, 20, RED);
					} else {
						DrawText(TextFormat("Score: %d", score), 340, 300, 20, WHITE);
					}
					
			        DrawText("Press ESC to go to MENU", 270, 360, 20, GRAY);
			EndDrawing();

			if (IsKeyPressed(KEY_ESCAPE)) {
				snake = Snake();
				snake2 = Snake(10, 15, Direction::RIGHT);
				food.respawn(snake, &snake2);
				score = 0;
				score2 = 0;
				snake.isDead = false;
				snake2.isDead = false;
				moveTimer = 0.0f;
				isTwoPlayer = false;
				state = GameState::MENU;
			}
		}

	}

	CloseWindow();
	return 0;
}

