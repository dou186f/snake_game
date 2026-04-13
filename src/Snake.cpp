#include "Snake.h"
#include "Constant.h"
Snake::Snake() {
    body.push_back({20, 15});
    body.push_back({19, 15});
    body.push_back({18, 15});
    dir = Direction::RIGHT;
    lastMovedDir = Direction::RIGHT;
    isDead = false;
}

Snake::Snake(int startX, int startY, Direction startDir) {
    body.push_back({startX, startY});
    body.push_back({startX - 1, startY});
    body.push_back({startX - 2, startY});
    dir = startDir;
    lastMovedDir = startDir;
	isDead = false;
}

void Snake::move() {
	Vector2i newHead = body.front();
	
	if (!inputBuffer.empty()) {
		dir = inputBuffer.front();
		inputBuffer.pop();
	}

	switch (dir) {
		case Direction::UP:	newHead.y -= 1; break;
		case Direction::DOWN:	newHead.y += 1; break;
		case Direction::LEFT: 	newHead.x -= 1; break;
		case Direction::RIGHT:	newHead.x += 1; break;
	}

	body.push_front(newHead);

	if (shouldGrow) {
		shouldGrow = false;
	} else {
		body.pop_back();
	}

	lastMovedDir = dir;
}

void Snake::grow() {
	shouldGrow = true;
}

bool Snake::checkSelfCollision() {
	Vector2i head = body.front();
	for (int i = 1; i < body.size(); i++) {
		if (body[i].x == head.x && body[i].y == head.y) {
			return true;
		}
	}
	return false;
}

bool Snake::checkOtherCollision(Snake& otherSnake) {
	Vector2i head = body.front();
	for (int i = 0; i < otherSnake.body.size(); i++) {
		if (otherSnake.body[i].x == head.x && otherSnake.body[i].y == head.y) {
			return true;
		}
	}
	return false;
}

bool Snake::checkWallCollision() {
	Vector2i head = body.front();
	return head.x < 2 || head.x >= cols || head.y < 2 || head.y >= rows;
}
