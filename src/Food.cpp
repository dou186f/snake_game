#include "Food.h"
#include <cstdlib>
#include "Constant.h"

Food::Food(Snake& snake) {
	respawn(snake);
}

void Food::respawn(Snake& snake, Snake* snake2) {

	while(true) {
		Vector2i newPos = {rand() % (cols - 2) + 2, rand() % (rows - 2) + 2};

		bool onSnake = false;

		for (auto& segment : snake.body) {
			if (segment.x == newPos.x && segment.y == newPos.y) {
				onSnake = true;
				break;
			}
		}

        if (!onSnake && snake2 != nullptr) {
            for (auto& segment : snake2->body) {
                if (segment.x == newPos.x && segment.y == newPos.y) {
                    onSnake = true;
                    break;
                }
            }
        }

        if (!onSnake) {
            position = newPos;
            return;
        }		
	}
}
