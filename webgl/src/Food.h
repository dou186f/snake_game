#pragma once

#include "Snake.h"

class Food {
public:
    Vector2i position{};

    explicit Food(const Snake& snake);
    bool respawn(const Snake& snake, const Snake* snake2 = nullptr);
};
