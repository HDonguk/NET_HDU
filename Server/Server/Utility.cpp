// Utility.cpp
#include "Utility.h"
#include <cmath>

bool checkCollision(float x1, float y1, float x2, float y2, float radius1, float radius2) {
    float distance = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    return distance <= (radius1 + radius2);
}
