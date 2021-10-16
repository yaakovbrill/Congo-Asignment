#include <iostream>
#include <vector>
#include <ctype.h>

using namespace std;

class Node{
    public:
        char file;
        int rank;
        char peice;
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
            peice = v;
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

        vector<Node*> whitePawnPositions;
        vector<Node*> blackPawnPositions;
        vector<Node*> whiteElephantPositions;
        vector<Node*> blackElephantPositions;
        Node *whiteLionPosition;
        Node *blackLionPosition;
        Node *whiteZebraPosition;
        Node *blackZebraPosition;

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
                        cout << grid[i * 7 + j].peice << " ";
                    }
                    else{
                        cout << grid[i * 7 + j].peice << endl;
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
            cout << node->position << " = " << node->peice << endl;
        }

        void addLocationOfPieces(){
            for(int i = 1; i <= 7; i++){
                for(int j = 1; j <= 7; j++){
                    Node *node = getNode(i, j);
                    char peice = node->peice;
                    if(peice == 'P'){
                        whitePawnPositions.push_back(node);
                    }
                    else if(peice == 'p'){
                        blackPawnPositions.push_back(node); 
                    }
                    else if(peice == 'E'){
                        whiteElephantPositions.push_back(node);
                    }
                    else if(peice == 'e'){
                        blackElephantPositions.push_back(node);
                    }
                    else if(peice == 'L'){
                        whiteLionPosition = node;
                    }
                    else if(peice == 'l'){
                        blackLionPosition = node;
                    }
                    else if(peice == 'Z'){
                        whiteZebraPosition = node;
                    }
                    else if(peice == 'z'){
                        blackZebraPosition = node;
                    }
                }
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

        bool isBlockValid(string colorCameFrom, Node *node){
            bool isWhite = true;
            if(colorCameFrom != "white"){
                isWhite = false;
            }
            if(node != nullptr){
                string colorGoinTo = getColor(node);
                if(colorGoinTo == "empty"){  //empty node
                    return true;
                }
                else if(colorGoinTo == "white"){  //we are going to a white peice
                    if(!isWhite){
                        return true;
                    }
                }
                else{  //we are going to a black peice
                    if(isWhite){
                        return true;
                    }
                }
            }
            return false;
        }

        bool isCapital(char c){
            if(int(c) >=65 && int(c <=90)){
                return true;
            }
            return false;
        }

        string getColor(Node *node){
            char peice = node->peice;
            if(peice == 'y'){
                return "empty";
            }
            else if(isCapital(peice)){
                return "white";
            }
            else{
                return "black";
            }
        }

        bool isInCastle(string myColor, Node *node){
            if(node->columnNum >= 3 && node->columnNum <= 5){
                if(myColor == "white"){
                    if(node->rowNum <= 3){
                        return true;
                    }
                }
                else{
                    if(node->rowNum >=5){
                        return true;
                    }
                }
            }
            return false;
        }

        Node* getLionToLionMove(Node *node, string myColor){    //////////////////////need to test this function
            Node *temp = node;
            if(myColor == "white"){
                if(node->rowNum == 3){
                    if(node->columnNum == 5){
                        temp = node->topLeft;
                        string colorGoinTo = getColor(temp);
                        if(colorGoinTo == "empty"){
                            temp = temp->topLeft;
                            if(temp->peice == 'l'){
                                return temp;
                            }
                        }
                    }
                    else if(node->columnNum == 3){
                        temp = node->topRight;
                        string colorGoinTo = getColor(temp);
                        if(colorGoinTo == "empty"){
                            temp = temp->topRight;
                            if(temp->peice == 'l'){
                                return temp;
                            }
                        }
                    }
                }
                temp = temp->top;
                while(temp != nullptr){
                    if(temp->peice == 'l'){
                        return temp;
                    }
                    else{
                        string colorGoinTo = getColor(temp);
                        if(colorGoinTo == "empty"){
                            temp = temp->top;
                        }
                        else{
                            break;
                        }
                    }
                }
            }
            else{  //myColor is black
                if(node->rowNum == 5){
                    if(node->columnNum == 5){
                        temp = node->bottomLeft;
                        string colorGoinTo = getColor(temp);
                        if(colorGoinTo == "empty"){
                            temp = temp->bottomLeft;
                            if(temp->peice == 'L'){
                                return temp;
                            }
                        }
                    }
                    else if(node->columnNum == 3){
                        temp = node->bottomRight;
                        string colorGoinTo = getColor(temp);
                        if(colorGoinTo == "empty"){
                            temp = temp->bottomRight;
                            if(temp->peice == 'L'){
                                return temp;
                            }
                        }
                    }
                }
                temp = temp->bottom;
                while(temp != nullptr){
                    if(temp->peice == 'L'){
                        return temp;
                    }
                    else{
                        string colorGoinTo = getColor(temp);
                        if(colorGoinTo == "empty"){
                            temp = temp->bottom;
                        }
                        else{
                            break;
                        }
                    }
                }
            }
            return nullptr;
        }

        void sortLionMoves(Node *lionToLion, vector<Node*> &lionMoves){   ////must check
            string stringA = lionToLion->position;
            for(int i =0; i < lionMoves.size(); i++){
                string stringB = lionMoves[i]->position;
                if(stringA.compare(stringB) < 0){ 
                    lionMoves.insert(lionMoves.begin() + i, lionToLion);
                    return;
                }
            }
            lionMoves.push_back(lionToLion);
        }

        vector<Node*> getLionMoves(Node *node){
            string pos = node->position;
            string myColor = getColor(node);
            Node *lionToLion = getLionToLionMove(node, myColor);
            vector<Node*> aroundNode = {node->bottomLeft, node->left, node->topLeft, node->bottom, node->top, node->bottomRight, node->right, node->topRight};
            vector<Node*> lionMoves;
            for(Node *nodeTo: aroundNode){
                if(isBlockValid(myColor, nodeTo)){
                    if(isInCastle(myColor, nodeTo)){
                        lionMoves.push_back(nodeTo);
                    }
                }
            }
            if(lionToLion != nullptr){
                sortLionMoves(lionToLion, lionMoves);
            }
            return lionMoves;
        }

        void printLionMove(Node *node, vector<Node*> lionMoves){
            string pos = node->position;
            for(int i = 0; i < lionMoves.size(); i++){
                cout << pos << lionMoves[i]->position;
                if(i != lionMoves.size()-1){
                    cout << " ";
                }
                else{
                    cout << endl;
                }
            }
        }

        Node* getLionTurnPosition(){
            if(sideToMove == "white"){
                return whiteLionPosition;
            }
            return blackLionPosition;
        }
};

int main(){

    //read FEN string
    int N = 1;
    cin >> N;
    vector<string> positionOfPiecesArray(N);
    vector<char> sideToMoveArray(N);
    vector<int> moveNumberArray(N);
    string position = "2ele1z/ppppppp/7/7/7/PPP1PPP/2ELE1Z";
    char side = 'w';
    int moveNum = 4;
    for(int i = 0; i < N; i++){
        cin >> position >> side >> moveNum;
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
        Node *node = grid.getLionTurnPosition();
        // cout << node->position << endl; 
        // cout << node->right->position << endl; 
        // cout << node->topRight->position << endl; 
        // cout << node->top->position << endl; 
        // cout << node->topLeft->position << endl; 
        // cout << node->left->position << endl; 
        // cout << node->bottomLeft->position << endl; 
        // cout << node->bottom->position << endl; 
        // cout << node->bottomRight->position << endl; 
        // if(node->bottomRight == nullptr){
        //     cout << "Null!" << endl;
        // }

        // cout << grid.whiteElephantPositions[0]->bottom->position;
        vector<Node*> lionMoves = grid.getLionMoves(node);
        grid.printLionMove(node, lionMoves);

    }

    return 0;
}