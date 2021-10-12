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
        string positionOfPieces;
        char sideToMove;
        int moveNumber;

        //constructor
        Grid(string position, char side, int moveNum){
            positionOfPieces = position;
            sideToMove = side;
            moveNumber = moveNum;
        }

        void createNode(int x, int y, char v){
            Node node(x, y, v);
            grid.push_back(node);
        }

        void createGrid(){
            int i = 0;
            int j = 0;
            for(int position = 0; position < positionOfPieces.length(); position++){
                char c = positionOfPieces[position];
                if(isdigit(c)){
                    int intC = int(c)-48;
                    for(int k = 0; k < intC; k++){
                        createNode(i, j, 'y'); 
                        j++;
                    }
                }
                else if(c == '/'){
                    i++;
                    j = 0;
                }
                else{
                    createNode(i, j, c);
                    j++;
                }
            }
        }

        void printGrid(){
            for(int i = 0; i < 7; i++){
                for(int j = 0; j < 7; j++){
                    if(j != 6){
                        cout << grid[i * 7 + j].value << " ";
                    }
                    else{
                        cout << grid[i * 7 + j].value << endl;
                    }
                }
            }
        }

        // void printNodeInfo(int i){
        //     Node node = grid[i];
        //     cout << "Node x: " << node.
        // }
};

int main(){

    //read FEN string
    string position;
    char side;
    int moveNum;
    cin >> position >> side >> moveNum;

    //create a grid
    Grid grid(position, side, moveNum);
    grid.createGrid();
    grid.printGrid();
    // cout << grid[0].

    return 0;
}