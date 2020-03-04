#ifndef _CORNER_STITCH_H
#define _CORNER_STITCH_H

#include "util.h"
#include<iostream>
#include<fstream>
#include <climits>

class CornerStitchPlane{
public:
    vector <Rectangle> rec_list;
    Tile *start_tile;
    Point *leftBottom, *rightTop;
    int solid_area, solid_count;
    Tile* PointFinding(Point target, Tile *ref_tile, bool downward=0);    
    bool AreaSearch(Rectangle area);
    bool TileCreate(Rectangle tile);
    void TileDelete(Rectangle tile);
    int SplitTile_H(Tile& ori, int split_y); // split and create top, use ref tile rt.y as split
    void SplitTile_V(Tile& ori, Rectangle& tile); // split and create left
    void EnumerateRight(Tile& ref_tile);
    void EnumerateAll();
    void OutputEnumerate(string filename);
    void OutputEnumerateRight(ofstream& o_file, Tile& ref_tile);
    void MergeTileUpward(Tile* tile, int bound_y=INT_MAX);
    void MergeTileDownward(Tile* tile);
    // just for space tile mergeing
    void MergeTileRightward(Tile* tile);
    void MergeTileLeftward(Tile* tile);
    // merge upper tile to lower tile
    void MergeTileUpdate_V(Tile* tile_l, Tile* tile_u);
    void MergeNeighborSpaceTile_V(Tile* tile);
    // Output all surronding tile for debug
    void OutputSurrondingAll(string filename);
    void OutputSurronding(ofstream& o_file, Tile& ref_tile);
    void OutputSurrondingTile(ofstream& o_file, Tile* ref_tile);
    // merge right tile to left tile
    void MergeTileUpdate_H(Tile* tile_l, Tile* tile_r);
    void TileDeleteRight(Tile* target_tile, bool from_left=0);
    void SplitFitTile_V(Tile* ref, Tile* tar); // split tile according to ref tile
    void TileDeleteLeft(Tile* neighbor_tile, int y_ubound);
};
#endif
