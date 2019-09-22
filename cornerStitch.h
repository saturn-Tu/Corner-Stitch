#ifndef _CORNER_STITCH_H
#define _CORNER_STITCH_H

#include "util.h"

class CornerStitchPlane{
public:
    vector <Rectangle> rec_list;
    Tile *start_tile;
    Tile* PointFinding(Point target, Tile *ref_tile);    
    bool AreaSearch(Rectangle area);
    bool TileCreate(Rectangle tile);
    bool TileDelete(Rectangle tile);
    void SplitTile_H(Tile& ori, Rectangle& tile); // split and create top 
    void SplitTile_V(Tile& ori, Rectangle& tile); // split and create left
};



#endif
