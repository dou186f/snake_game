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

	float moveTimer1 = 0.0f;
	float moveTimer2 = 0.0f;
	float moveInterval1 = 0.1f;
	float moveInterval2 = 0.1f;

	GameState state = GameState::MENU;
	
	srand(time(nullptr));
	Food food(snake);

	while (!WindowShouldClose() && !shouldQuit) {

		// MENU PAGE
		if (state == GameState::MENU) {
			if(IsKeyPressed(KEY_DOWN)) selectedButton = (selectedButton + 1) % 3;
			if(IsKeyPressed(KEY_UP)) selectedButton = (selectedButton + 2) % 3;
			
			if (IsKeyPressed(KEY_ENTER)) {
				if (selectedButton == 0) { state = GameState::PLAYING; isTwoPlayer = false; }
				if (selectedButton == 1) { state = GameState::TWOPLAYER; isTwoPlayer = true; }
				if (selectedButton == 2) shouldQuit = true;
			}

			BeginDrawing();
				ClearBackground(BLACK);
				
				Color playColor = (selectedButton == 0) ? GREEN : DARKGREEN;
				Color versusColor = (selectedButton == 1) ? GREEN : DARKGREEN;	
				Color quitColor = (selectedButton == 2) ? RED : Color{139, 0, 0, 255};
				
				DrawText("SNAKE GAME", 300, 165, 30, GREEN);
				
				DrawText("PLAY", 300, 235, 20, playColor);
				DrawText("VERSUS MODE", 300, 295, 20, versusColor);
				DrawText("QUIT", 300, 355, 20, quitColor);

				DrawText(">", 280, 235 + selectedButton * 60, 20, WHITE);
			EndDrawing();
		}
		
		// SINGLE PLAYER
		else if (state == GameState::PLAYING) {
			if (IsKeyPressed(KEY_UP) && snake.lastMovedDir != Direction::DOWN)
				snake.inputBuffer.push(Direction::UP);
			if (IsKeyPressed(KEY_DOWN) && snake.lastMovedDir != Direction::UP)
				snake.inputBuffer.push(Direction::DOWN);
			if (IsKeyPressed(KEY_LEFT) && snake.lastMovedDir != Direction::RIGHT)		
				snake.inputBuffer.push(Direction::LEFT);
			if (IsKeyPressed(KEY_RIGHT) && snake.lastMovedDir != Direction::LEFT)	
				snake.inputBuffer.push(Direction::RIGHT);
			
			moveTimer1 += GetFrameTime();
			if (moveTimer1 >= moveInterval1) {
				moveTimer1 = 0.0f;
				snake.move();
			}

			if (snake.body.front().x == food.position.x && snake.body.front().y == food.position.y) {
				snake.grow();
				food.respawn(snake);
				score++;
				if (moveInterval1 > 0.05f) moveInterval1 -= 0.001f;
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

		// TWO PLAYER
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
			
			moveTimer1 += GetFrameTime();
			moveTimer2 += GetFrameTime();
			if (moveTimer1 >= moveInterval1) {
				moveTimer1 = 0.0f;
				snake.move();
			}
			if (moveTimer2 >= moveInterval2) {
				moveTimer2 = 0.0f;
				snake2.move();
			}

			if (snake.body.front().x == food.position.x && snake.body.front().y == food.position.y) {
				snake.grow();
				food.respawn(snake, &snake2);
				score++;
				if (moveInterval1 > 0.05f) moveInterval1 -= 0.001f;
			}

			if (snake2.body.front().x == food.position.x && snake2.body.front().y == food.position.y) {
				snake2.grow();
				food.respawn(snake, &snake2);
				score2++;
				if (moveInterval2 > 0.05f) moveInterval2 -= 0.001f;
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
		
		// GAME OVER PAGE
		else if (state == GameState::GAMEOVER) {
			BeginDrawing();
				ClearBackground(BLACK);
           			DrawText("GAME OVER", 320, 240, 20, RED);

					if (isTwoPlayer) {
           					if (snake.isDead && !snake2.isDead) {
                					DrawText("RED WINS!", 360, 270, 20, RED);
						}
						else if (snake2.isDead && !snake.isDead) {
                					DrawText("GREEN WINS!", 360, 270, 20, GREEN);
						}
           					else {
							if (score > score2) {
								DrawText("GREEN WINS!", 360, 270, 20, GREEN);
							}
							else if (score2 > score) {
								DrawText("RED WINS!", 360, 270, 20, RED);
							}
							else {
          							DrawText("DRAW!", 360, 270, 20, WHITE);
							}
						}

						DrawText(TextFormat("Score Green: %d", score), 360, 300, 20, GREEN);
						DrawText(TextFormat("Score Red: %d", score2), 360, 325, 20, RED);
					} 
					else {
						DrawText(TextFormat("Score: %d", score), 340, 300, 20, WHITE);
					}
					
			        DrawText("Press ESC to go to MENU", 270, 360, 20, GRAY);
				DrawText("Press R to RESTART", 270, 400, 20, GRAY);
			EndDrawing();
			
			if (IsKeyPressed(KEY_ESCAPE)) {
				snake = Snake();
				snake2 = Snake(10, 15, Direction::RIGHT);
				food.respawn(snake, &snake2);
				score = 0;
				score2 = 0;
				snake.isDead = false;
				snake2.isDead = false;
				moveTimer1 = 0.0f;
				moveInterval1 = 0.1f;
				moveTimer2 = 0.0f;
				moveInterval2 = 0.1f;
				isTwoPlayer = false;
				state = GameState::MENU;
			}

			if (IsKeyPressed(KEY_R)) {
				GameState restartState = isTwoPlayer ? GameState::TWOPLAYER : GameState::PLAYING;
				snake = Snake();
				snake2 = Snake(10, 15, Direction::RIGHT);
				food.respawn(snake, &snake2);
				score = 0;
				score2 = 0;
				snake.isDead = false;
				snake2.isDead = false;
				moveTimer1 = 0.0f;
				moveInterval1 = 0.1f;
				moveTimer2 = 0.0f;
				moveInterval2 = 0.1f;
				state = restartState;
			}
		}
	}

	CloseWindow();
	return 0;
}

