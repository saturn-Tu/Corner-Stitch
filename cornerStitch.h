#ifndef _CORNER_STITCH_H
#define _CORNER_STITCH_H

#include "util.h"
#include<iostream>
#include<fstream>

class CornerStitchPlane{
public:
    vector <Rectangle> rec_list;
    Tile *start_tile;
    Point *leftBottom, *rightTop;
    Tile* PointFinding(Point target, Tile *ref_tile, bool downward=0);    
    bool AreaSearch(Rectangle area);
    bool TileCreate(Rectangle tile);
    bool TileDelete(Rectangle tile);
    void SplitTile_H(Tile& ori, Rectangle& tile); // split and create top 
    void SplitTile_V(Tile& ori, Rectangle& tile); // split and create left
    void EnumerateRight(Tile& ref_tile);
    void EnumerateAll();
    void OutputEnumerate(string filename);
    void OutputEnumerateRight(ofstream& o_file, Tile& ref_tile);
    void MergeTileUpward(Tile* tile);
    // merge upper tile to lower tile
    void MergeTileUpdate(Tile* tile_l, Tile* tile_u);
};
#endif
