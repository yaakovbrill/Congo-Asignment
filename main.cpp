#include <iostream>
#include <vector>

using namespace std;

class Node{
    public:
        int file;
        int rank;
        char value;

        Node(int column, int row, char v){
            file = column;
            rank = row;
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
            int i = 7;
            int j = 1;
            for(int position = 0; position < positionOfPieces.length(); position++){
                char c = positionOfPieces[position];
                if(isdigit(c)){
                    int intC = int(c)-48;
                    for(int k = 0; k < intC; k++){
                        createNode(j, i, 'y'); 
                        j++;
                    }
                }
                else if(c == '/'){
                    i--;
                    j = 1;
                }
                else{
                    createNode(j, i, c);
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

        Node getNode(int file, int rank){
            if(rank == 0 || file == 0){
                cout << "Rank or File should not be 0!\n";
            }
            return grid[49 - 7*rank + file -1];
        }

        void printNodeInfo(int file, int rank){
            Node node = getNode(file, rank);
            cout << node.file << "," << node.rank << " = " << node.value << endl;
        }
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
    grid.printNodeInfo(1,1);

    return 0;
}