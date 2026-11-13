#include "Snake.h"

Snake::Snake() : Snake(20, 15, Direction::RIGHT) {}

Snake::Snake(int startX, int startY, Direction startDir)
    : dir(startDir), lastMovedDir(startDir) {
    Vector2i offset{1, 0};
    if (startDir == Direction::LEFT) offset = {-1, 0};
    if (startDir == Direction::UP) offset = {0, -1};
    if (startDir == Direction::DOWN) offset = {0, 1};
    for (int i = 0; i < 3; ++i) {
        body.push_back({startX - offset.x * i, startY - offset.y * i});
    }
}

void Snake::queueDirection(Direction direction) {
    Direction previous = inputBuffer.empty() ? lastMovedDir : inputBuffer.back();
    bool opposite =
        (previous == Direction::UP && direction == Direction::DOWN) ||
        (previous == Direction::DOWN && direction == Direction::UP) ||
        (previous == Direction::LEFT && direction == Direction::RIGHT) ||
        (previous == Direction::RIGHT && direction == Direction::LEFT);
    if (direction != previous && !opposite) {
        inputBuffer.push(direction);
    }
}

void Snake::move() {
    if (!inputBuffer.empty()) {
        dir = inputBuffer.front();
        inputBuffer.pop();
    }

    Vector2i head = body.front();
    switch (dir) {
        case Direction::UP: --head.y; break;
        case Direction::DOWN: ++head.y; break;
        case Direction::LEFT: --head.x; break;
        case Direction::RIGHT: ++head.x; break;
    }

    previousTail = body.back();
    body.push_front(head);
    body.pop_back();
    lastMovedDir = dir;
}

void Snake::grow() {
    body.push_back(previousTail);
}

bool Snake::checkSelfCollision() const {
    const Vector2i head = body.front();
    for (std::size_t i = 1; i < body.size(); ++i) {
        if (body[i].x == head.x && body[i].y == head.y) return true;
    }
    return false;
}

bool Snake::checkOtherCollision(const Snake& otherSnake) const {
    const Vector2i head = body.front();
    for (const auto& segment : otherSnake.body) {
        if (segment.x == head.x && segment.y == head.y) return true;
    }
    return false;
}

bool Snake::checkWallCollision() const {
    const Vector2i head = body.front();
    return head.x < 2 || head.x >= cols || head.y < 2 || head.y >= rows;
}
