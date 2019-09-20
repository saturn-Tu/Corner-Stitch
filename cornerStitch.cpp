#include "cornerStitch.h"

Tile* CornerStitchPlane::PointFinding(Point target) {
    bool direction = 0; //0: vertical 1:horizontal
    Tile *now_rec = start_tile;
    while( !now_rec->InRectangle(target) ) {
        if ( direction ) {  
            if ( target.x > now_rec->rightTop.x )
                now_rec = now_rec->tr;
            else if ( target.x < now_rec->leftBottom.x )
                now_rec = now_rec->bl;
            else
                direction = !direction;
        }
        else {
            if ( target.y > now_rec->rightTop.y )
                now_rec = now_rec->rt;
            else if ( target.y < now_rec->leftBottom.y )
                now_rec = now_rec->lb;
            else
                direction = !direction;
        }
    }
    return now_rec;
}

bool CornerStitchPlane::AreaSearch(Rectangle area) {
    Point leftTop(area.leftBottom.x, area.rightTop.y);
    Tile *now_rec = this->PointFinding(leftTop);
    while( !now_rec && now_rec->leftBottom.y >= area.leftBottom.y ) {
        while ( now_rec->rightTop.x < area.leftBottom.x ) {
            now_rec = now_rec->tr;    
        }
        if ( now_rec->type || now_rec->rightTop.x < area.rightTop.x ) return 0;
        now_rec = now_rec->lb;
    }
    return 1;
}

bool CornerStitchPlane::TileCreate(Rectangle tile) {
    if ( !AreaSearch(tile) )  return 0;
    // Split top space tile
    Tile *now_rec = this->PointFinding(tile.rightTop);
}

void CornerStitchPlane::SplitTile_H(Tile& ori, Rectangle& tile) {
    if ( tile.rightTop.y == ori.rightTop.y ) return;
    Rectangle *new_tile = new Tile(tile.leftBottom.x, tile.rightTop.y, tile.rightTop.x, ori.rightTop.y, 0);
    Rectangle *now_tile = ori.bl;
    //while ( !now_tile && now_tile->rightTop.y <= new_tile->rightTop.y ) {
           
    //}
}
