#include <iostream>
#include <vector>

using namespace std;

class Node{
    public:
        int rank;
        int file;
        char value;

        Node(int row, int column, char v){
            rank = row;
            file = column;
            value = v;
        }
};

class Grid{
    public:
        vector<Node> grid;

        // Grid(string positionOfPieces, )
};

int main(){
    return 0;
}