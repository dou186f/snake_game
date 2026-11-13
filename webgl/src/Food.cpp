#include "Food.h"
#include <cstdlib>
#include <vector>

Food::Food(const Snake& snake) {
    respawn(snake);
}

bool Food::respawn(const Snake& snake, const Snake* snake2) {
    std::vector<Vector2i> freeCells;
    for (int x = 2; x < cols; ++x) {
        for (int y = 2; y < rows; ++y) {
            bool occupied = false;
            for (const auto& segment : snake.body) {
                if (segment.x == x && segment.y == y) {
                    occupied = true;
                    break;
                }
            }
            if (!occupied && snake2 != nullptr) {
                for (const auto& segment : snake2->body) {
                    if (segment.x == x && segment.y == y) {
                        occupied = true;
                        break;
                    }
                }
            }
            if (!occupied) freeCells.push_back({x, y});
        }
    }

    if (freeCells.empty()) return false;
    position = freeCells[static_cast<std::size_t>(std::rand()) % freeCells.size()];
    return true;
}
