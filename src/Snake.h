#pragma once

#include <deque>
#include "Constant.h"
#include <queue>

struct Vector2i {
	int x, y;
};

enum class Direction {
	UP, DOWN, LEFT, RIGHT	
};

class Snake {
public:	
	std::deque<Vector2i> body;
	std::queue<Direction> inputBuffer;
	Direction dir;
	Direction lastMovedDir;
	bool isDead = false;

	Snake();
	Snake(int startX, int startY, Direction startDir);
	void move();
	void grow();
	bool checkSelfCollision();
	bool checkWallCollision();
	bool checkOtherCollision(Snake& otherSnake);
private:
	bool shouldGrow = false;
};
