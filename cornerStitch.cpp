#include "cornerStitch.h"

Tile* CornerStitchPlane::PointFinding(F_Point target, Tile *ref_tile, bool downward) {
    bool direction = 0; //0: vertical 1:horizontal 
    Tile *now_rec = start_tile;
    // use ref_tile to save search time
    now_rec = (ref_tile) ? ref_tile : start_tile;
    while( !now_rec->InTile(target, downward) ) {
        // cout << direction << " pointfinding " << *now_rec;
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
    //cout << "point finding FIND: " << *now_rec;
    return now_rec;
}

bool CornerStitchPlane::AreaSearch(Rectangle area) {
    F_Point leftTop(area.leftBottom.x+0.5, area.rightTop.y-0.5);
    Tile *now_rec = this->PointFinding(leftTop, 0);
    if ( now_rec && now_rec->type ) return 0;
    if ( now_rec && now_rec->rightTop.x < area.rightTop.x ) return 0;
    while( now_rec && now_rec->rightTop.y >= area.leftBottom.y ) {
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
    F_Point tmp_rt(tile.rightTop.x-0.5, tile.rightTop.y-0.5);
    Tile *top_tile = this->PointFinding(tmp_rt, 0);
    this->SplitTile_H(*top_tile, tile.rightTop.y);
    top_tile->rightTop.y = tile.rightTop.y;
    // Split bottom space tile

    //OutputSurrondingAll("tmp_output.txt");
    F_Point tmp_lb(tile.leftBottom.x+0.5, tile.leftBottom.y+0.5);
    Tile *bottom_tile = this->PointFinding(tmp_lb, top_tile, 1);
    //cout << "Bottom tile: " << *bottom_tile;
    //Rectangle tmp_bottom(tile.leftBottom.x, bottom_tile->leftBottom.y, tile.rightTop.x, );
    int split_y = tile.leftBottom.y;
    // cuz pointFinding edge case, need to avoid split when y = y
    if ( split_y != bottom_tile->leftBottom.y ) {
        this->SplitTile_H(*bottom_tile, split_y);
        bottom_tile->rightTop.y = tile.leftBottom.y;
    }
    //cout << "after hori\n"; 
    //this->EnumerateAll();
    // Split V  tile
    Tile *middle_tile = this->PointFinding(tmp_rt, 0);
    Tile *solid_tile; // record solid tile for neighbor space tile merging
    // Find tile for split
    while ( middle_tile && middle_tile->leftBottom.y >= tile.leftBottom.y ){ 
        while ( middle_tile->rightTop.x < tile.leftBottom.x ) {
            middle_tile = middle_tile->tr;    
        }
        // Split left space tile
        Rectangle tmp_middle(tile.leftBottom.x, middle_tile->leftBottom.y, tile.rightTop.x, middle_tile->rightTop.y);
        // cuz pointFinding edge case, need to avoid split when x = x
        if ( tmp_middle.leftBottom.x != tmp_middle.rightTop.x ) {
            this->SplitTile_V(*middle_tile, tmp_middle);
            middle_tile->leftBottom.x = tmp_middle.leftBottom.x; 
        }
        //cout << "middle: " << *middle_tile;
        // Split right space tile
        //this->EnumerateAll();
        //cout << "\nSPLIT RIGHT\n\n";
        Rectangle tmp_right(tile.rightTop.x, middle_tile->leftBottom.y, middle_tile->rightTop.x, middle_tile->rightTop.y);
        // cuz pointFinding edge case, need to avoid split when x = x
        if ( tmp_right.leftBottom.x != tmp_right.rightTop.x ) {
            this->SplitTile_V(*middle_tile, tmp_right);
            middle_tile->leftBottom.x = tmp_right.leftBottom.x; 
            middle_tile->bl->type = 1;
            MergeTileUpward(middle_tile->bl, tile.rightTop.y);
            solid_tile = middle_tile->bl;
            middle_tile = solid_tile->lb;
        }
        else {
            middle_tile->type = 1;
            MergeTileUpward(middle_tile, tile.rightTop.y);
            solid_tile = middle_tile;
            middle_tile = middle_tile->lb;
        }
    }
    MergeNeighborSpaceTile_V(solid_tile);
    return 1;
}

int CornerStitchPlane::SplitTile_H(Tile& ori, int split_y) {
    //cout << "in H\n";
    //cout << "ori: " << ori << "tile: " << tile;
    //if ( tile.rightTop.y == ori.rightTop.y ) return;
    if ( split_y == ori.rightTop.y ) return 0;
    Tile *new_tile = new Tile(ori.leftBottom.x, split_y, ori.rightTop.x, ori.rightTop.y, 0);
    new_tile->lb = &ori;
    // update left tile of origin tile
    bool left_flag = 0;
    Tile *now_tile = ori.bl;
    new_tile->bl = ori.bl;
    while ( now_tile && now_tile->leftBottom.y <= ori.rightTop.y ) {
        if ( now_tile->rightTop.y > new_tile->leftBottom.y ) {
            if ( !left_flag ) {
                left_flag = 1;
                new_tile->bl = now_tile;
            }
            if ( now_tile->rightTop.y <= new_tile->rightTop.y )
                now_tile->tr = new_tile;    
        }       
        now_tile = now_tile->rt;
    }
    // update right tile of origin tile
    bool right_flag = 0;
    now_tile = ori.tr;
    new_tile->tr = ori.tr;
    while ( now_tile && now_tile->leftBottom.y >= new_tile->leftBottom.y ) {
        now_tile->bl = new_tile;    
        now_tile = now_tile->lb;
    }
    ori.tr = now_tile;
    // update upper tile of origin tile
    now_tile = ori.rt;
    new_tile->rt = ori.rt;
    while ( now_tile && now_tile->leftBottom.x >= new_tile->leftBottom.x ) {
        //cout << "***Bottom now_tile: " << now_tile;
        now_tile->lb = new_tile;       
        now_tile = now_tile->bl;
    }
    ori.rt = new_tile;
    //cout << "output tile:\n";
    return 1;
}

void CornerStitchPlane::SplitTile_V(Tile& ori, Rectangle& tile) {
    //cout << "in V\n";
    //cout << "ori: " << ori << "tile: " << tile;
    if ( tile.leftBottom.x == ori.leftBottom.x ) return;
    Tile *new_tile = new Tile(ori.leftBottom.x, ori.leftBottom.y, tile.leftBottom.x, ori.rightTop.y, 0);
    new_tile->tr = &ori;
    // update bottom tile of origin tile
    Tile *now_tile = ori.lb;
    //cout << "before while\n";
    new_tile->lb = ori.lb;
    while ( now_tile && now_tile->rightTop.x <= new_tile->rightTop.x ) {
        if ( now_tile->rightTop.x > new_tile->leftBottom.x ) {
            now_tile->rt = new_tile;    
        }       
        now_tile = now_tile->tr;
    }

    ori.lb = now_tile;
    // update top tile of origin tile
    bool top_flag = 0;
    now_tile = ori.rt;
    new_tile->rt = ori.rt;
    while ( now_tile && now_tile->rightTop.x >= new_tile->leftBottom.x ) {
        if ( now_tile->leftBottom.x < new_tile->rightTop.x ) {
            if ( !top_flag ) {
                top_flag = 1;
                new_tile->rt = now_tile;
            }

            if ( now_tile->leftBottom.x >= new_tile->leftBottom.x ) 
                now_tile->lb = new_tile;    
        }  
        now_tile = now_tile->bl;
    }
    // update left tile of origin tile
    now_tile = ori.bl;
    new_tile->bl = ori.bl;
    while ( now_tile && now_tile->rightTop.y <= new_tile->rightTop.y ) {
        if ( now_tile->rightTop.y >= new_tile->leftBottom.y ) {
            now_tile->tr = new_tile;    
        }       
        now_tile = now_tile->rt;
    }
    ori.bl = new_tile;
    //cout << "output tile:\n";
    //cout << ori << *new_tile;
}

void CornerStitchPlane::EnumerateRight( Tile& ref_tile ) {
    Tile *now_tile = ref_tile.tr;
    while ( now_tile && now_tile->leftBottom.y >= ref_tile.leftBottom.y ) {
        //  not to enumerate
        if ( now_tile->bl != &ref_tile )  return;
        else {
            //cout << "ENUMERATE: " << *now_tile;
            //cout << now_tile->ReturnOutlineString();
            if (now_tile->type == 1) {
                this->solid_area += now_tile->GetArea();
                this->solid_count++;
            }
            this->EnumerateRight( *now_tile );
        }
        now_tile = now_tile->lb;
    }
}

void CornerStitchPlane::EnumerateAll() {
    //(0,100) is tmp
    this->solid_area = 0;
    this->solid_count = 0;
    F_Point leftTop(this->leftBottom->x+0.5, this->rightTop->y-0.5);
    Tile *left_tile = this->PointFinding(leftTop, 0);
    while( left_tile ) {
        //cout << "ENUMERATE: " << *left_tile;
        //cout << left_tile->ReturnOutlineString();
        if (left_tile->type == 1) {
            this->solid_area += left_tile->GetArea();
            this->solid_count++;
        }
        this->EnumerateRight( *left_tile );
        left_tile = left_tile->lb;
    }
}

void CornerStitchPlane::OutputEnumerate(string filename) {
    ofstream o_file;
    o_file.open(filename);
    F_Point leftTop(this->leftBottom->x+0.5, this->rightTop->y-0.5);
    Tile *left_tile = this->PointFinding(leftTop, 0);
    while( left_tile ) {
        o_file << left_tile->ReturnOutlineString();
        this->OutputEnumerateRight(o_file, *left_tile);
        left_tile = left_tile->lb;
    }
    o_file.close();
}

void CornerStitchPlane::OutputEnumerateRight(ofstream& o_file, Tile& ref_tile) {
    Tile *now_tile = ref_tile.tr;
    while ( now_tile && now_tile->leftBottom.y >= ref_tile.leftBottom.y ) {
        if ( now_tile->bl != &ref_tile )  return;
        else {
            o_file << now_tile->ReturnOutlineString();
            this->OutputEnumerateRight(o_file, *now_tile);
        }
        now_tile = now_tile->lb;
    }
}

void CornerStitchPlane::MergeTileUpward(Tile* tile, int bound_y) {
    bool type = tile->type;
    Tile* up_tile = tile->rt;
    while (up_tile && up_tile->leftBottom.y < bound_y) {
        if (up_tile->leftBottom.x == tile->leftBottom.x && up_tile->rightTop.x == tile->rightTop.x 
         && up_tile->type == tile->type) {
            MergeTileUpdate_V(tile, up_tile);
        }
        else break;
        // tile just merge once, space can merge unlimit
        if (type == 1) break;
        up_tile = tile->rt;
    }
}

void CornerStitchPlane::MergeTileDownward(Tile* tile) {
    bool type = tile->type;
    Tile* down_tile = tile->lb;
    while (down_tile) {
        if (down_tile->leftBottom.x == tile->leftBottom.x && down_tile->rightTop.x == tile->rightTop.x
         && down_tile->type == tile->type) {
            Tile* tmp = tile;
            tile = down_tile;
            down_tile = tmp;
            MergeTileUpdate_V(tile, down_tile);
        }
        else break;
        // solid tile just merge once, space can merge unlimit
        if (type == 1) break;
        down_tile = tile->lb;
    }
}

void CornerStitchPlane::MergeTileRightward(Tile* tile) {
    Tile* right_tile = tile->tr;  
    while (right_tile) {
        if (right_tile->leftBottom.y == tile->leftBottom.y && right_tile->rightTop.y == tile->rightTop.y
         && right_tile->type == tile->type) {
            MergeTileUpdate_H(tile, right_tile);
        }
        else break;
        right_tile = right_tile->tr;
    }
}

void CornerStitchPlane::MergeTileLeftward(Tile* tile) {
    Tile* left_tile = tile->bl;
    while (left_tile) {
        if (left_tile->leftBottom.y == tile->leftBottom.y && left_tile->rightTop.y == tile->rightTop.y
         && left_tile->type == tile->type) {
            Tile* tmp = tile;
            tile = left_tile;
            left_tile = tmp;
            MergeTileUpdate_H(tile, left_tile);
        }
        else break;
        left_tile = tile->bl;
    }
}

void CornerStitchPlane::MergeTileUpdate_V(Tile* tile_l, Tile* tile_u) {
    // update info at left tile
    Tile* left_tile = tile_u->bl;
    while (left_tile && left_tile->rightTop.y <= tile_u->rightTop.y) {
        left_tile->tr = tile_l;
        left_tile = left_tile->rt;
    }
    // update info at right tile
    Tile* right_tile = tile_u->tr;
    while (right_tile && right_tile->leftBottom.y >= tile_u->leftBottom.y) {
        right_tile->bl = tile_l;
        right_tile = right_tile->lb;
    }
    // update info at upper tile
    Tile* upper_tile = tile_u->rt;
    while (upper_tile && upper_tile->leftBottom.x >= tile_u->leftBottom.x) {
        upper_tile->lb = tile_l;
        upper_tile = upper_tile->bl;
    }
    tile_l->rt = tile_u->rt;
    tile_l->tr = tile_u->tr;
    tile_l->rightTop.y = tile_u->rightTop.y;
    if (this->start_tile == tile_u)
        this->start_tile = tile_l;
    delete(tile_u);
}

void CornerStitchPlane::MergeNeighborSpaceTile_V(Tile* tile) {
    if (tile->bl) {
        MergeTileUpward(tile->bl);
        MergeTileDownward(tile->bl);
    }
    if (tile->tr) {
        MergeTileUpward(tile->tr);
        MergeTileDownward(tile->tr);
    }
}

void CornerStitchPlane::OutputSurrondingAll(string filename) {
    ofstream o_file;
    o_file.open(filename);
    F_Point leftTop(this->leftBottom->x+0.5, this->rightTop->y-0.5);
    Tile *left_tile = this->PointFinding(leftTop, 0);
    while( left_tile ) {
        o_file << "\n" << left_tile->ReturnOutlineString();
        OutputSurrondingTile(o_file, left_tile);
        o_file << "\n";
        this->OutputSurronding(o_file, *left_tile);
        left_tile = left_tile->lb;
    }
    o_file.close();
}

void CornerStitchPlane::OutputSurronding(ofstream& o_file, Tile& ref_tile) {
    Tile *now_tile = ref_tile.tr;
    while ( now_tile && now_tile->leftBottom.y >= ref_tile.leftBottom.y ) {
        if ( now_tile->bl != &ref_tile )  return;
        else {
            o_file << "\n" << now_tile->ReturnOutlineString();
            OutputSurrondingTile(o_file, now_tile);
            o_file << "\n";
            this->OutputSurronding(o_file, *now_tile);
        }
        now_tile = now_tile->lb;
    }
}

void CornerStitchPlane::OutputSurrondingTile(ofstream& o_file, Tile* ref_tile) {
    if (ref_tile->tr != 0) 
        o_file << "r: " << ref_tile->tr->ReturnOutlineString();
    if (ref_tile->rt != 0) 
        o_file << "t: " << ref_tile->rt->ReturnOutlineString();
    if (ref_tile->bl != 0) 
        o_file << "l: " << ref_tile->bl->ReturnOutlineString();
    if (ref_tile->lb != 0) 
        o_file << "b: " << ref_tile->lb->ReturnOutlineString();
}

void CornerStitchPlane::TileDelete(Rectangle tile) {
    F_Point tmp_lt(tile.leftBottom.x+0.5, tile.rightTop.y-0.5);
    Tile* target_tile = this->PointFinding(tmp_lt, 0);
    if (!(tile.rightTop == target_tile->rightTop && tile.leftBottom == target_tile->leftBottom))
        return;
    int y_ubound = target_tile->rightTop.y;
    target_tile->type = 0;
    // handle right adjacent tile
    Tile* left_tile = target_tile->bl;
    TileDeleteRight(target_tile);
    TileDeleteLeft(left_tile, y_ubound);
}

void CornerStitchPlane::MergeTileUpdate_H(Tile* tile_l, Tile* tile_r) {
    if (tile_l == 0 || tile_r == 0) return;
    // update info at lower tile
    Tile* lower_tile = tile_r->lb;
    while (lower_tile && lower_tile->rightTop.x <= tile_r->rightTop.x) {
        lower_tile->rt = tile_l;
        lower_tile = lower_tile->tr;
    }
    // update info at upper tile
    Tile* upper_tile = tile_r->rt;
    while (upper_tile && upper_tile->leftBottom.x >= tile_r->leftBottom.x) {
        upper_tile->lb = tile_l;
        upper_tile = upper_tile->bl;
    }
    // update info at right tile
    Tile* right_tile = tile_r->tr;
    while (right_tile && right_tile->leftBottom.y >= tile_r->leftBottom.y) {
        right_tile->bl = tile_l;
        right_tile = right_tile->lb;
    }
    tile_l->rt = tile_r->rt;
    tile_l->tr = tile_r->tr;
    tile_l->rightTop.x = tile_r->rightTop.x;
    if (this->start_tile == tile_r)
        this->start_tile = tile_l;
    delete(tile_r);
}

void CornerStitchPlane::TileDeleteRight(Tile* target_tile, bool from_left) {
    if (target_tile->type == 1) return;
    int y_lbound = target_tile->leftBottom.y;
    bool v_merge_tile = (target_tile->bl==0);
    F_Point tmp_lt(target_tile->leftBottom.x+0.5, target_tile->rightTop.y-0.5);
    while (target_tile && target_tile->leftBottom.y >= y_lbound) {
        int y_record = target_tile->rightTop.y;
        // split right tile vertically
        Tile* right_tile = target_tile->tr;
        SplitFitTile_V(target_tile, right_tile);
        // split dead tile vertically
        SplitFitTile_V(target_tile->tr,target_tile);
        // find real target_tile because after split will offset
        while (target_tile->rightTop.y < y_record) {
            target_tile = target_tile->rt;
        }
        Tile* next_tile = target_tile->lb;
        MergeTileRightward(target_tile);
        if (from_left)
            MergeTileLeftward(target_tile);
        target_tile = next_tile;
    }
    if (from_left || (!from_left&&v_merge_tile)) {
        Tile* neighbor_tile = this->PointFinding(tmp_lt, 0);
        while (neighbor_tile && neighbor_tile->leftBottom.y >= y_lbound) {
            MergeTileUpward(neighbor_tile);
            MergeTileDownward(neighbor_tile);
            neighbor_tile = neighbor_tile->lb;
        }
    }
}

void CornerStitchPlane::SplitFitTile_V(Tile* ref, Tile* tar) {
    if (ref == 0 || tar == 0) return;
    // split right tile vertically
    if (tar->type == 0 && tar->rightTop.y > ref->rightTop.y) {
        int split_y = ref->rightTop.y;
        this->SplitTile_H(*tar, split_y);
        tar->rightTop.y = split_y;
    }
    if (tar->type == 0 && tar->leftBottom.y < ref->leftBottom.y) {
        int split_y = ref->leftBottom.y;
        this->SplitTile_H(*tar, split_y);
        tar->rightTop.y = split_y;
    }
}

void CornerStitchPlane::TileDeleteLeft(Tile* neighbor_tile, int y_ubound) {
    if (neighbor_tile == 0) return;
    if (neighbor_tile->type == 1) return;
    while (neighbor_tile && neighbor_tile->rightTop.y <= y_ubound) {
        Tile* next_tile = neighbor_tile->rt;
        TileDeleteRight(neighbor_tile, 1);
        neighbor_tile = next_tile;
    }
}