#pragma once

#include <deque>
#include <queue>
#include "Constant.h"

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
    void queueDirection(Direction direction);
    void move();
    void grow();
    bool checkSelfCollision() const;
    bool checkWallCollision() const;
    bool checkOtherCollision(const Snake& otherSnake) const;

private:
    Vector2i previousTail{};
};
