
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
    plane.start_tile = new Tile(*plane.leftBottom, *plane.rightTop, 0);
    for ( auto rec : plane.rec_list ) {
        plane.TileCreate(rec);
        //cout << "\n\n\n*****finish once*****\n";
        //plane.EnumerateAll();
        cout << "\nonce!\n\n";
        plane.OutputSurrondingAll("tmp_output.txt");
        plane.OutputEnumerate("output.txt");
    }
    cout << "After TileCreate\n";

    // delete tile
    for ( auto rec : plane.rec_list ) {
        plane.TileDelete(rec);
        cout << "d tile " << rec;
        cout << "\ndelete once!\n\n";
        plane.OutputSurrondingAll("tmp_output.txt");
        cout << "after\n";
        plane.OutputEnumerate("output.txt");
        int a;
        cin >> a;
    }
    cout << "outside\n";

    plane.OutputEnumerate("output.txt");
    //cout << *plane.start_tile;
    //plane.EnumerateAll();
    cout << "Done\n";
    return 0;
}
