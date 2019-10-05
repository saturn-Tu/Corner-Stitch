#include "util.h"

bool Point::operator==(const Point& a) {
    return this->x == a.x && this->y == a.y;   
}

bool Rectangle::InRectangle(Point target) {
    return target.x >= this->leftBottom.x && target.y >= this->leftBottom.y && target.x <= this->rightTop.x && target.y <= this->rightTop.y;    
}

ostream& operator<<(ostream &s, Point p){
    s << "("<< p.x << ", " << p.y << ")"; 
    return s; 
}

ostream& operator<<(ostream &s, Rectangle r) { 
    s << r.leftBottom << ", " << r.rightTop << endl; 
    return s; 
} 
