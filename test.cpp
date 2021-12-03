#include <iostream>
#include <array>

using namespace std;
int
main(int argc, char* argv[]) {
    int vec[361][4][4], x, y;
    int lmao[4] = {1, 19, -1, -19};
    for (int i=0; i<361; i++) {
        x = i%19; y = i/19;

        for (int lol=0; lol<4; lol++) {
            vec[x][y][lol] = lmao[lol];
        }

        if (x==18) {
            vec[x][y][0] = 0;
        } else if (x==0) {
            vec[x][y][2] = 0;
        }
        if (y==18) {
            vec[x][y][1] = 0;
        } else if (y==0) {
            vec[x][y][3] = 0;
        }
        cout << "{";
        for (int lol=0; lol<4; lol++) {
            cout << vec[x][y][lol] << ",";
        }
        cout << "}";
        if (x==18) {
            cout << endl;
        } else {
            cout << ",";
        }
    }
    cout << endl;
    return 0;
}