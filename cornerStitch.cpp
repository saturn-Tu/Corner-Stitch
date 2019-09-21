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
    this->SplitTile_H(*now_rec, tile);
}

void CornerStitchPlane::SplitTile_H(Tile& ori, Rectangle& tile) {
    if ( tile.rightTop.y == ori.rightTop.y ) return;
    Tile *new_tile = new Tile(ori.leftBottom.x, tile.rightTop.y, ori.rightTop.x, ori.rightTop.y, 0);
    new_tile->lb = &ori;
    // update left tile of origin tile
    bool left_flag = 0;
    Tile *now_tile = ori.bl;
    while ( now_tile && now_tile->rightTop.y <= ori.rightTop.y ) {
        if ( now_tile->rightTop.y > new_tile->leftBottom.y ) {
            if ( !left_flag ) {
                left_flag = 1;
                new_tile->bl = now_tile;
            }
            now_tile->tr = new_tile;    
        }       
        now_tile = now_tile->rt;
    }
    // update right tile of origin tile
    bool right_flag = 0;
    now_tile = ori.tr;
    while ( now_tile && now_tile->leftBottom.y >= new_tile->leftBottom.y ) {
        if ( now_tile->rightTop.y >= new_tile->leftBottom.y ) {
            if ( !right_flag ) {
                right_flag = 1;
                new_tile->tr = now_tile;
            }
            now_tile->bl = new_tile;    
        }       
        now_tile = now_tile->lb;
    }
    // update upper tile of origin tile
    bool top_flag = 0;
    now_tile = ori.rt;
    while ( now_tile && now_tile->leftBottom.x >= new_tile->leftBottom.x ) {
        if ( now_tile->leftBottom.x >= new_tile->leftBottom.x ) {
            if ( !top_flag ) {
                top_flag = 1;
                new_tile->rt = now_tile;
            }
            now_tile->lb = new_tile;    
        }       
        now_tile = now_tile->bl;
    }
    ori.rt = new_tile;
    cout << "output tile:\n";
    cout << ori << *new_tile;
}

