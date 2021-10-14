#include <iostream>
#include <vector>

using namespace std;

class Node{
    public:
        char file;
        int rank;
        char value;
        string position;
        int columnNum;
        int rowNum;

        Node *topLeft = nullptr;
        Node *top = nullptr;
        Node *topRight = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;
        Node *bottomLeft = nullptr;
        Node *bottom = nullptr;
        Node *bottomRight = nullptr;

        Node(int column, int row, char v){
            file = char(column+96);
            rank = row;
            position = file + to_string(rank);
            value = v;
            columnNum = column;
            rowNum = row;
        }
};

class Grid{
    public:
        vector<Node> grid;
        string positionOfPieces;
        string sideToMove;
        int moveNumber;

        vector<vector<string>> peicesPositions = {{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}};

        //constructor
        Grid(string position, char side, int moveNum){
            positionOfPieces = position;
            if(side == 'w'){
                sideToMove = "white";
            }
            else{
                sideToMove = "black";
            }
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

        Node *getNode(int file, int rank){
            if(rank == 0 || file == 0){
                cout << "Rank or File should not be 0!\n";
            }
            return &grid[49 - 7*rank + file -1];
        }

        void printNodeInfo(int file, int rank){
            Node *node = getNode(file, rank);
            cout << node->position << " = " << node->value << endl;
        }

        void addLocationOfPieces(){
            for(int i = 1; i <= 7; i++){
                for(int j = 1; j <= 7; j++){
                    Node *node = getNode(i, j);
                    char peice = node->value;
                    if(peice == 'P'){
                        peicesPositions[0].push_back(node->position);
                    }
                    else if(peice == 'p'){
                        peicesPositions[1].push_back(node->position);
                    }
                    else if(peice == 'S'){
                        peicesPositions[2].push_back(node->position);
                    }
                    else if(peice == 's'){
                        peicesPositions[3].push_back(node->position);
                    }
                    else if(peice == 'G'){
                        peicesPositions[4].push_back(node->position);
                    }
                    else if(peice == 'g'){
                        peicesPositions[5].push_back(node->position);
                    }
                    else if(peice == 'M'){
                        peicesPositions[6].push_back(node->position);
                    }
                    else if(peice == 'm'){
                        peicesPositions[7].push_back(node->position);
                    }
                    else if(peice == 'E'){
                        peicesPositions[8].push_back(node->position);
                    }
                    else if(peice == 'e'){
                        peicesPositions[9].push_back(node->position);
                    }
                    else if(peice == 'L'){
                        peicesPositions[10].push_back(node->position);
                    }
                    else if(peice == 'l'){
                        peicesPositions[11].push_back(node->position);
                    }
                    else if(peice == 'C'){
                        peicesPositions[12].push_back(node->position);
                    }
                    else if(peice == 'c'){
                        peicesPositions[13].push_back(node->position);
                    }
                    else if(peice == 'Z'){
                        peicesPositions[14].push_back(node->position);
                    }
                    else if(peice == 'z'){
                        peicesPositions[15].push_back(node->position);
                    }
                }
            }
        }

        void printLocationOfPieces(){
            for(int i = 0; i < 16; i++){
                if(i == 0){
                    cout << "white pawn:";
                }
                else if(i == 1){
                    cout << "black pawn:";
                }
                else if(i == 2){
                    cout << "white superpawn:";
                }
                else if(i == 3){
                    cout << "black superpawn:";
                }
                else if(i == 4){
                    cout << "white giraffe:";
                }
                else if(i == 5){
                    cout << "black giraffe:";
                }
                else if(i == 6){
                    cout << "white monkey:";
                }
                else if(i == 7){
                    cout << "black monkey:";
                }
                else if(i == 8){
                    cout << "white elephant:";
                }
                else if(i == 9){
                    cout << "black elephant:";
                }
                else if(i == 10){
                    cout << "white lion:";
                }
                else if(i == 11){
                    cout << "black lion:";
                }
                else if(i == 12){
                    cout << "white crocodile:";
                }
                else if(i == 13){
                    cout << "black crocodile:";
                }
                else if(i == 14){
                    cout << "white zebra:";
                }
                else if(i == 15){
                    cout << "black zebra:";
                }
                int size = peicesPositions[i].size();
                for(int j = 0; j < size; j++){
                    cout << " " << peicesPositions[i][j];
                }
                cout << endl;
            }
        }

        void printSideToMove(){
            cout << "side to play: " << sideToMove;
        }

        void setNodeAdjacencies(){
            for(int j = 1; j <= 7; j++){
                for(int i = 1; i <= 7; i++){
                    Node *node = getNode(i, j);
                    if(i < 7){ //it has a node to the right
                        Node *nodeRight = getNode(i+1, j);
                        node->right = nodeRight;
                        nodeRight->left = node;
                    }
                    if(j < 7){ //it has a node above
                        Node *nodeTop = getNode(i, j+1);
                        node->top = nodeTop;
                        nodeTop->bottom = node;
                        if(i > 1){ //it has a node to the topLeft
                            Node *nodeTopLeft = getNode(i-1, j+1);
                            node->topLeft = nodeTopLeft;
                            nodeTopLeft->bottomRight = node;
                        }
                        if(i < 7){ //it has a node to the topRight
                            Node *nodeTopRight = getNode(i+1, j+1);
                            node->topRight = nodeTopRight;
                            nodeTopRight->bottomLeft = node;
                        }
                    }
                }
            }
        }
};

int main(){

    //read FEN string
    int N = 1;
    // cin >> N;
    vector<string> positionOfPiecesArray(N);
    vector<char> sideToMoveArray(N);
    vector<int> moveNumberArray(N);
    string position = "2l4/7/4z2/4c2/PP2EP1/3L2p/7";
    char side = 'b';
    int moveNum = 23;
    for(int i = 0; i < N; i++){
        // cin >> position >> side >> moveNum;
        positionOfPiecesArray[i] = position;
        sideToMoveArray[i] = side;
        moveNumberArray[i] = moveNum;
    }

    //create a grid
    for(int i = 0; i < N; i++){
        Grid grid(positionOfPiecesArray[i], sideToMoveArray[i], moveNumberArray[i]);
        grid.createGrid();
        // grid.printGrid();
        grid.addLocationOfPieces();
        // grid.printLocationOfPieces();
        // grid.printSideToMove();
        
        grid.setNodeAdjacencies(); 
        Node *node = grid.getNode(4, 4);
        cout << node->position << endl; 
        cout << node->right->position << endl; 
        cout << node->topRight->position << endl; 
        cout << node->top->position << endl; 
        cout << node->topLeft->position << endl; 
        cout << node->left->position << endl; 
        cout << node->bottomLeft->position << endl; 
        cout << node->bottom->position << endl; 
        cout << node->bottomRight->position << endl; 
        if(node->bottomRight == nullptr){
            cout << "Null!" << endl;
        }
    }

    return 0;
}