#ifndef _UTIL_H
#define _UTIL_H

#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Point {
public:
    Point(){}
    Point(int x, int y):x(x), y(y){}
    int x, y;
    bool operator==(const Point& a);
    friend ostream& operator<<(ostream &s, Point p);
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
    Rectangle(Point lb, Point rt): leftBottom(lb), rightTop(rt){}
    Rectangle(int x1, int y1, int x2, int y2) {
        this->leftBottom.x = x1;
        this->leftBottom.y = y1;
        this->rightTop.x = x2;
        this->rightTop.y = y2;
    }
    Point leftBottom, rightTop;
    bool InRectangle(Point target);
    friend ostream &operator<<(ostream &s, Rectangle r); 
};

class Tile: public Rectangle {
public:
    Tile(Point lb, Point rt, bool type):Rectangle(lb, rt), type(type){
        this->bl = 0;
        this->lb = 0;
        this->rt = 0;
        this->tr = 0;
    }
    Tile(int x1, int y1, int x2, int y2, bool type):Rectangle(x1,y1,x2,y2), type(type){
        this->bl = 0;
        this->lb = 0;
        this->rt = 0;
        this->tr = 0;
    }
    bool type; //0:space, 1:solid
    Tile *bl, *lb, *rt, *tr;
    bool InTile(Point target, bool downward);
    string ReturnOutlineString();
};

#endif
