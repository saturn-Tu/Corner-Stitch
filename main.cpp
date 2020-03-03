
#include<vector>
#include<string>
#include<sstream>
#include "cornerStitch.h"
#include<cctype>
 
int main( int argc, char *argv[] ){
    //char filename[] = "TestData/chip.trp";
    char *filename = argv[1];
    std::ifstream input_file;
    CornerStitchPlane plane;
    
    // parse input_file to rec_list 
    input_file.open(filename);
    if( input_file.fail() ) {
        cout << "Fail to open file" << endl;    
        return 0;
    }
    vector <int> rectangle_list;
    unsigned int min_x = -1, min_y = -1, max_x = 0, max_y = 0;
    cout << "parsing\n";
    // parse file
    while ( !input_file.eof() ) {
        stringstream ss; 
        string line, word;
        vector <Point> tmp_point;
        getline( input_file, line );
        ss.str("");
        ss.clear();
        ss << line;
        ss >> word;
        if ( !isdigit(word[0]) )
            continue; 
        for ( int n = 0; n < 4; n++ ) {
            int x, y;
            x = stoi(word);
            ss >> word;
            y = stoi(word);
            tmp_point.emplace_back(x,y);
            if ( n == 3 ) break;
            ss >> word;
        }
        plane.rec_list.emplace_back(tmp_point);
        auto& rec = plane.rec_list.back();
        min_x = (rec.leftBottom.x < min_x) ?rec.leftBottom.x :min_x;
        min_y = (rec.leftBottom.y < min_y) ?rec.leftBottom.y :min_y;
        max_x = (rec.rightTop.x > max_x) ?rec.rightTop.x :max_x;
        max_y = (rec.rightTop.y > max_y) ?rec.rightTop.y :max_y;
    }
    cout << "after parsing\n";
    plane.leftBottom = new Point(min_x, min_y); 
    plane.rightTop = new Point(max_x, max_y);
    cout << "Boundary:" << *plane.leftBottom << *plane.rightTop << endl;
    for ( auto rec : plane.rec_list) {
        cout << rec; 
    }
    // create tile
    int area_sum = 0, area_count = 0;
    int c_count = 0;
    plane.start_tile = new Tile(*plane.leftBottom, *plane.rightTop, 0);
    for ( auto rec : plane.rec_list ) {
        cout << "tile: " << rec;
        bool success_create = plane.TileCreate(rec);
        area_sum = (success_create) ? area_sum+rec.GetArea() : area_sum;
        area_count = (success_create) ? area_count+1 : area_count;
        //plane.EnumerateAll();
        cout << "once! " << area_count << " " << c_count++ << "\n";
    }
    cout << "After TileCreate\n";

    plane.EnumerateAll();
    int conpute_area = plane.solid_area;
    int compute_count = plane.solid_count;
    cout << "\nreal_sum: " << area_sum << endl;
    cout << "area_sum: " << plane.solid_area << endl;
    bool areaPass = 0, createCountPass = 0;
    if (area_sum == plane.solid_area)
        areaPass = 1;
    if (area_count == plane.solid_count)
        createCountPass = 1;
    plane.OutputSurrondingAll("tmp_output.txt");
    plane.OutputEnumerate("output.txt");
    // delete tile
    int d_count = 0;
    for ( auto rec : plane.rec_list ) {
        plane.TileDelete(rec);
        cout << "delete once! " << d_count++ << "\n";
    }
    plane.EnumerateAll();
    int deletePass = 0;
    if (plane.solid_count == 0 && plane.start_tile->type == 0)
        deletePass = 1;
    //plane.OutputEnumerate("output.txt");

    cout << "Done\n";
    cout << "\n\n------------------------------------\n";
    cout << "AreaPass:\t\t" << areaPass << endl;
    cout << "real " << area_sum << endl;
    cout << "copt " << conpute_area << endl;
    cout << "CreateCountPass:\t" << createCountPass << endl;
    cout << "real " << area_count << endl;
    cout << "copt " << compute_count << endl;
    cout << "DeletePass:\t\t" << deletePass << endl;

    return 0;
}
