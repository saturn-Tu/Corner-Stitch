#include "util.h"

bool Point::operator==(const Point& a) {
    return this->x == a.x && this->y == a.y;   
}

bool Rectangle::operator==(const Rectangle& a) {
    return this->leftBottom == a.leftBottom && this->rightTop == a.rightTop;
}

bool Rectangle::InRectangle(Point target) {
    return target.x >= this->leftBottom.x && target.y >= this->leftBottom.y && target.x <= this->rightTop.x && target.y <= this->rightTop.y;    
}

int Rectangle::GetArea() {
    return (rightTop.x-leftBottom.x)*(rightTop.y-leftBottom.y);
}

string Tile::ReturnOutlineString() {
    string s;
    if (this->type == 0)
        s.append("SPACE ");
    else
        s.append("TILE ");
    s.append(to_string(this->leftBottom.x));
    s.append(" ");
    s.append(to_string(this->leftBottom.y));
    s.append(" ");
    s.append(to_string(this->leftBottom.x));
    s.append(" ");
    s.append(to_string(this->rightTop.y));
    s.append(" ");
    s.append(to_string(this->rightTop.x));
    s.append(" ");
    s.append(to_string(this->rightTop.y));
    s.append(" ");
    s.append(to_string(this->rightTop.x));
    s.append(" ");
    s.append(to_string(this->leftBottom.y));
    s.append(" ");
    s.append(to_string(this->leftBottom.x));
    s.append(" ");
    s.append(to_string(this->leftBottom.y));
    s.append("\n");
    return s;
}

bool Tile::InTile(Point target, bool downward) {
    if ( !this->lb && !this->bl ) // touch left corner
        return target.x >= this->leftBottom.x && target.y >= this->leftBottom.y && target.x <= this->rightTop.x && target.y <= this->rightTop.y;    
    else if ( !this->lb ) // touch botton edge
        return target.x > this->leftBottom.x && target.y >= this->leftBottom.y && target.x <= this->rightTop.x && target.y <= this->rightTop.y;    
    else if ( !this->bl ) { // touch left edge
        if (downward)
            return target.x >= this->leftBottom.x && target.y >= this->leftBottom.y && target.x <= this->rightTop.x && target.y < this->rightTop.y;    
        else
            return target.x >= this->leftBottom.x && target.y > this->leftBottom.y && target.x <= this->rightTop.x && target.y <= this->rightTop.y;    
    }
    else { // normal
        if (downward)
            return target.x > this->leftBottom.x && target.y >= this->leftBottom.y && target.x <= this->rightTop.x && target.y < this->rightTop.y;    
        else
            return target.x > this->leftBottom.x && target.y > this->leftBottom.y && target.x <= this->rightTop.x && target.y <= this->rightTop.y;    
    }
}

ostream& operator<<(ostream &s, Point p){
    s << "("<< p.x << ", " << p.y << ")"; 
    return s; 
}

ostream& operator<<(ostream &s, Rectangle r) { 
    s << r.leftBottom << ", " << r.rightTop << endl; 
    return s; 
} 
