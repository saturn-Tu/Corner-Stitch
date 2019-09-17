#ifndef _UTIL_H
#define _UTIL_H

#include <vector>

class Point {
public:
    Point(){}
    Point(int x, int y):x(x), y(y){}
    int x, y;
    bool operator==(const Point& a);
};

class Rectangle {
public:
    Rectangle(std::vector <Point> outline){
        leftBottom = outline[0];
        rightTop = outline[0]; 
        for ( int n = 1; n < outline.size(); n++ ) {
            if ( leftBottom.x > outline[n].x || leftBottom.y > outline[n].y )
                leftBottom = outline[n];    
            if ( rightTop.x < outline[n].x || rightTop.y < outline[n].y ) 
                rightTop = outline[n];    
        }
    }
    Point leftBottom, rightTop;
    bool InRectangle(Point target);

};

class Tile: public Rectangle {
public:
    bool type; //0:space, 1:solid
    Tile *bl, *lb, *rt, *tr;
};

#endif
