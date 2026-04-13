#pragma once

#include "Snake.h"
#include "Constant.h"

class Food {
public:
	Vector2i position;
	
	Food(Snake& snake);
	void respawn(Snake& snake, Snake* snake2 = nullptr);
};
