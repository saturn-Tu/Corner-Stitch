#include "cornerStitch.h"

Tile* CornerStitchPlane::PointFinding(Point target, Tile *ref_tile) {
    bool direction = 0; //0: vertical 1:horizontal 
    Tile *now_rec = start_tile;
    // use ref_tile to save search time
    now_rec = (ref_tile) ? ref_tile : start_tile;
    while( !now_rec->InRectangle(target) ) {
        cout << "pointfinding " << *now_rec << endl;
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
    Tile *now_rec = this->PointFinding(leftTop, 0);
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
    Tile *top_tile = this->PointFinding(tile.rightTop, 0);
    this->SplitTile_H(*top_tile, tile);
    top_tile->rightTop.y = tile.rightTop.y;
    // Split bottom space tile
    Tile *bottom_tile = PointFinding(tile.leftBottom, top_tile);
    Rectangle tmp_bottom(tile.leftBottom.x, bottom_tile->leftBottom.y, tile.rightTop.x, tile.leftBottom.y);
    this->SplitTile_H(*bottom_tile, tmp_bottom);
    bottom_tile->rightTop.y = tile.leftBottom.y;
    // Split left space tile
    //Tile *middle_tile = top_tile;
    //do {
    //    middle_tile = middle_tile.bl;
    //    while ( middle_tile->rightTop.x < tile.leftBottom.x ) {
    //        middle_tile = middle->tr;    
    //    }
    //    Rectangle *left_rec();

    //}
    //while ()
    //Tile *left_tile = Point 
    
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
    if ( !now_tile )  ori.tr = now_tile;
    // update upper tile of origin tile
    now_tile = ori.rt;
    new_tile->rt = ori.rt;
    while ( now_tile && now_tile->leftBottom.x >= new_tile->leftBottom.x ) {
        if ( now_tile->leftBottom.x >= new_tile->leftBottom.x ) {
            now_tile->lb = new_tile;    
        }       
        now_tile = now_tile->bl;
    }
    ori.rt = new_tile;
    cout << "output tile:\n";
    cout << ori << *new_tile;
}

void CornerStitchPlane::SplitTile_V(Tile& ori, Rectangle& tile) {
    if ( tile.leftBottom.x == ori.leftBottom.x ) return;
    Tile *new_tile = new Tile(ori.leftBottom.x, ori.leftBottom.y, tile.leftBottom.x, ori.rightTop.y, 0);
    new_tile->tr = &ori;
    // update bottom tile of origin tile
    bool bottom_flag = 0;
    Tile *now_tile = ori.lb;
    while ( now_tile && now_tile->rightTop.x <= new_tile->rightTop.x ) {
        if ( now_tile->rightTop.x > new_tile->leftBottom.x ) {
            if ( !bottom_flag ) {
                bottom_flag = 1;
                new_tile->lb = now_tile;
            }
            now_tile->rt = new_tile;    
        }       
        now_tile = now_tile->tr;
    }
    if ( !now_tile )  ori.lb = now_tile;
    // update top tile of origin tile
    bool top_flag = 0;
    now_tile = ori.rt;
    while ( now_tile && now_tile->leftBottom.x >= new_tile->leftBottom.x ) {
        if ( now_tile->leftBottom.x >= new_tile->rightTop.x ) {
            if ( !top_flag ) {
                top_flag = 1;
                new_tile->rt = now_tile;
            }
            now_tile->lb = new_tile;    
        }  
        now_tile = now_tile->bl;
    }
    // update left tile of origin tile
    now_tile = ori.bl;
    new_tile->rt = now_tile;
    while ( now_tile && now_tile->rightTop.y < new_tile->rightTop.y ) {
        if ( now_tile->rightTop.y >= new_tile->leftBottom.y ) {
            now_tile->tr = new_tile;    
        }       
        now_tile = now_tile->rt;
    }
    ori.bl = new_tile;
    cout << "output tile:\n";
    cout << ori << *new_tile;
}

void CornerStitchPlane::EnumerateRight( Tile ref_tile ) {
    Tile *now_tile = ref_tile.tr;
    while ( now_tile && now_tile->leftBottom.y >= ref_tile.leftBottom.y ) {
        while ( now_tile->leftBottom.x < ref_tile.leftBottom.x ) {
            now_tile = now_tile->tr;    
        }
        //  not to enumerate
        if ( now_tile->bl != &ref_tile )  return;
        else {
            cout << "ENUMERATE: " << &now_tile;
            this->EnumerateRight( *now_tile );
        }
        now_tile = now_tile->lb;
    }
}

void CornerStitchPlane::EnumerateAll() {
    //(0,100) is tmp
    Point leftTop(0, 100);
    Tile *left_tile = this->PointFinding(leftTop, 0);
    while( left_tile ) {
        cout << "ENUMERATE: " << *left_tile;
        this->EnumerateRight( *left_tile );
        left_tile = left_tile->lb;
    }
}
