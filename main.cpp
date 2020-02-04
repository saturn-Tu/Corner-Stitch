
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

    //Point a1(3,5);
    //Point a2(3,8);
    //Point a3(6,8);
    //Point a4(6,5);
    //Point target1(4,6);
    //Point target2(2,6);
    //vector <Point> tmp;
    //tmp.push_back(a3);
    //tmp.push_back(a1);
    //tmp.push_back(a2);
    //tmp.push_back(a4);
    //Rectangle tmp_rec(tmp);
    //cout << tmp_rec; 
    //cout << tmp_rec.InRectangle(target1) << endl; 
    //cout << tmp_rec.InRectangle(target2) << endl;

    //int a;
    //cin >> a;
    
    // parse input_file to rec_list 
    input_file.open(filename);
    if( input_file.fail() ) {
        cout << "Fail to open file" << endl;    
        return 0;
    }
    vector <int> rectangle_list;
    unsigned int min_x = -1, min_y = -1, max_x = 0, max_y = 0;
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
    plane.leftBottom = new Point(min_x, min_y); 
    plane.rightTop = new Point(max_x, max_y);
    cout << "Boundary:" << *plane.leftBottom << *plane.rightTop << endl;
    for ( auto rec : plane.rec_list) {
        cout << rec;    
    }
    plane.start_tile = new Tile(*plane.leftBottom, *plane.rightTop, 1);
    for ( auto rec : plane.rec_list ) {
        plane.TileCreate(rec);
        cout << "\n\n\n*****finish once*****\n";
        plane.EnumerateAll();
        cout << "\n\n\n";
    }

    cout << "After TileCreate\n";
    plane.OutputEnumerate("output.txt");
    //cout << *plane.start_tile;
    //plane.EnumerateAll();
    return 0;
}
