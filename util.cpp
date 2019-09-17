#include "util.h"

bool Point::operator==(const Point& a) {
    return this->x == a.x && this->y == a.y;   
}

bool Rectangle::InRectangle(Point target) {
    return target.x > this->leftBottom.x && target.y > this->leftBottom.y && target.x <= this->rightTop.x && target.y <= this->rightTop.y;    
}

