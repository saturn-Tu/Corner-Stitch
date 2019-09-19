#ifndef _CORNER_STITCH_H
#define _CORNER_STITCH_H

#include "util.h"

class CornerStitchPlane{
public:
    Tile *start_tile;
    Tile* PointFinding(Point target);    
    bool AreaSearch(Rectangle area);
    bool TileCreate(Rectangle tile);
    bool TileDelete(Rectangle tile);
    void SplitTile_H(Tile& ori, Rectangle& tile);
};



#endif
