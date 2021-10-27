#include <iostream>
#include <vector>
#include <ctype.h>
#include <string>

using namespace std;

class Node;

class PossibleMove{
    public:
        Node *nodeTo;
        string positionOfPieces;
        string sideToMove;
        int moveNumber;

        PossibleMove(Node *nTo){
            nodeTo = nTo;
        }
};

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

        vector<PossibleMove> possibleMovesAndFuturState;

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
        Node *whiteLionPosition = nullptr;
        Node *blackLionPosition = nullptr;
        Node *whiteZebraPosition = nullptr;
        Node *blackZebraPosition = nullptr;

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

        Node* getLionToLionMove(Node *node, string myColor){
            Node *temp;
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
                temp = node->top;
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
                temp = node->bottom;
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

        void sortLionMoves(Node *lionToLion, vector<Node*> &lionMoves){
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

        void getLionMoves(Node *node){
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

            for(Node *lionMove: lionMoves){
                PossibleMove possibleMove(lionMove);
                node->possibleMovesAndFuturState.push_back(possibleMove);
            }
        }

        void getZebraMoves(Node *node){
            Node *nodeTo, *temp;
            string pos = node->position;
            string myColor = getColor(node);
            vector<Node*> zebraMoves = {};
            for(int i = 0; i < 4; i++){
                if(i == 0 || i == 1){
                    nodeTo = node->left;
                }
                else if(i == 2 || i == 3){
                    nodeTo = node->right;
                }
                if(nodeTo != nullptr){
                    if(i == 0){
                        nodeTo = nodeTo->left;
                    }
                    else if(i == 3){
                        nodeTo = nodeTo->right;
                    }
                }
                else{
                    continue;
                }

                if(nodeTo != nullptr){
                    if(i == 0 || i == 3){
                        temp = nodeTo->bottom;
                    }
                    else if(i == 1 || i == 2){
                        temp = nodeTo->bottom;
                        if(temp != nullptr){
                            temp = temp->bottom;
                        }
                    }
                    if(isBlockValid(myColor, temp)){
                        zebraMoves.push_back(temp);
                    }
                    if(i == 0 || i == 3){
                        temp = nodeTo->top;
                    }
                    else if(i == 1 || i == 2){
                        temp = nodeTo->top;
                        if(temp != nullptr){
                            temp = temp->top;
                        }
                    }
                    if(isBlockValid(myColor, temp)){
                        zebraMoves.push_back(temp);
                    }
                }
            }
            for(Node *zebraMove: zebraMoves){
                PossibleMove possibleMove(zebraMove);
                node->possibleMovesAndFuturState.push_back(possibleMove);
            }
        }

        void getElephantsMoves(Node *node){
            Node *nodeTo, *temp;
            string pos = node->position;
            string myColor = getColor(node);
            for(int i = 0; i < 4; i++){
                if(i == 0){
                    nodeTo = node->left;
                }
                else if(i == 1){
                    nodeTo = node->bottom;
                }
                else if(i == 2){
                    nodeTo = node->top;
                }
                else{
                    nodeTo = node->right;
                }
                if(nodeTo != nullptr){
                    if(i == 0){
                        temp = nodeTo->left;
                    }
                    else if(i == 1){
                        temp = nodeTo->bottom;
                    }
                    else if(i == 2){
                        if(isBlockValid(myColor, nodeTo)){
                            PossibleMove possibleMove(nodeTo);
                            node->possibleMovesAndFuturState.push_back(possibleMove);
                        }
                        temp = nodeTo->top;
                    }
                    else{
                        if(isBlockValid(myColor, nodeTo)){
                            PossibleMove possibleMove(nodeTo);
                            node->possibleMovesAndFuturState.push_back(possibleMove);
                        }
                        temp = nodeTo->right;
                    }
                }
                else{
                    continue;
                }

                if(temp != nullptr){
                    if(isBlockValid(myColor, temp)){
                        PossibleMove possibleMove(temp);
                        node->possibleMovesAndFuturState.push_back(possibleMove);
                    }
                }
                if(i == 2 || i == 3){
                    continue;
                }
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(nodeTo);
                    node->possibleMovesAndFuturState.push_back(possibleMove);
                }
            }
        }

        void getPawnsMoves(Node *node){
            Node *nodeTo, *temp;
            string pos = node->position;
            string myColor = getColor(node);
            if(myColor == "white"){
                nodeTo = node->topLeft;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(nodeTo);
                    node->possibleMovesAndFuturState.push_back(possibleMove);
                }
                if(node->rank > 4){
                    nodeTo = node->bottom;
                    if(nodeTo != nullptr){
                        if(nodeTo->peice == 'y'){
                            temp = nodeTo->bottom;
                            if(temp->peice == 'y'){
                                PossibleMove possibleMove(temp);
                                node->possibleMovesAndFuturState.push_back(possibleMove);
                            }
                        }
                        if(nodeTo != nullptr && nodeTo->peice == 'y'){
                            PossibleMove possibleMove(nodeTo);
                            node->possibleMovesAndFuturState.push_back(possibleMove);
                        }
                    }
                }
                nodeTo = node->top;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(nodeTo);
                    node->possibleMovesAndFuturState.push_back(possibleMove);
                }
                nodeTo = node->topRight;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(nodeTo);
                    node->possibleMovesAndFuturState.push_back(possibleMove);
                }
            }
            else if(myColor == "black"){
                nodeTo = node->bottomLeft;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(nodeTo);
                    node->possibleMovesAndFuturState.push_back(possibleMove);
                }
                nodeTo = node->bottom;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(nodeTo);
                    node->possibleMovesAndFuturState.push_back(possibleMove);
                }
                if(node->rank < 4){
                    nodeTo = node->top;
                    if(nodeTo != nullptr){
                        if(nodeTo->peice == 'y'){
                            PossibleMove possibleMove(nodeTo);
                            node->possibleMovesAndFuturState.push_back(possibleMove);
                            temp = nodeTo->top;
                            if(temp->peice == 'y'){
                                PossibleMove possibleMove(temp);
                                node->possibleMovesAndFuturState.push_back(possibleMove);
                            }
                        }
                    }
                }
                nodeTo = node->bottomRight;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(nodeTo);
                    node->possibleMovesAndFuturState.push_back(possibleMove);
                }
            }
        }

        // void printPeiceMove(Node *node, bool &b){
        //     vector<Node*> peiceMoves = node->possibleMoves;
        //     string pos = node->position;
        //     for(int i = 0; i < peiceMoves.size(); i++){
        //         if(b){
        //             cout << " ";
        //         }
        //         else{
        //             b = true;
        //         }
        //         cout << pos << peiceMoves[i]->position;
        //         // if(i != peiceMoves.size()-1){
        //         //     cout << " ";
        //         // }
        //     }
        // }

        void printPeiceMove(Node *node){
            vector<PossibleMove> possibleMoves = node->possibleMovesAndFuturState;
            string pos = node->position;
            for(int i = 0; i < possibleMoves.size(); i++){
                cout << pos << possibleMoves[i].nodeTo->position;
                if(i != possibleMoves.size()-1){
                    cout << " ";
                }
            }
            cout << endl;
        }

        Node* getLionTurnPosition(){
            if(sideToMove == "white"){
                return whiteLionPosition;
            }
            return blackLionPosition;
        }

        Node* getZebraTurnPosition(){
            if(sideToMove == "white"){
                return whiteZebraPosition;
            }
            return blackZebraPosition;
        }

        vector<Node*> getElephantsTurnPosition(){
            if(sideToMove == "white"){
                return whiteElephantPositions;
            }
            return blackElephantPositions;
        }

        vector<Node*> getPawnsTurnPosition(){
            if(sideToMove == "white"){
                return whitePawnPositions;
            }
            return blackPawnPositions;
        }

        vector<Node*> getMoveFromAndTo(string moveToBePlayed){
            vector<Node*> moveFromAndTo;
            Node *startNode = getNode(int(moveToBePlayed[0])-96, int(moveToBePlayed[1])-48);
            moveFromAndTo.push_back(startNode);
            Node *endNode = getNode(int(moveToBePlayed[2])-96, int(moveToBePlayed[3])-48);
            moveFromAndTo.push_back(endNode);
            return moveFromAndTo;
        }

        void updateStringAfterNodeRemoved(int position){ 
            positionOfPieces.erase(position, 1);
            char before, after;
            if(position > 0){
                before = positionOfPieces[position-1];
            }
            if(position < positionOfPieces.size()){
                after = positionOfPieces[position];
            }
            if(isdigit(before)){
                if(isdigit(after)){
                    positionOfPieces.erase(position-1, 2);
                    int total = int(before)-48 +1 + int(after)-48;
                    positionOfPieces.insert(position-1, to_string(total));
                }
                else{
                    positionOfPieces.erase(position-1, 1);
                    int total = int(before)-48 +1;
                    positionOfPieces.insert(position-1, to_string(total));
                }
            }
            else{
                if(isdigit(after)){
                    positionOfPieces.erase(position, 1);
                    int total = int(after)-48 +1;
                    positionOfPieces.insert(position, to_string(total));
                }
                else{
                    positionOfPieces.insert(position, "1");
                }
            }
        }

        void removeNode(Node *node){
            int rowNum = node->rowNum;
            int colNum = node->columnNum;
            int rowIterationNum = 7;
            int colIterationNum = 1;
            for(int position = 0; position < positionOfPieces.length(); position++){
                char c = positionOfPieces[position];
                if(rowNum == rowIterationNum){
                    if(colNum == colIterationNum){  //update string here
                        updateStringAfterNodeRemoved(position);
                        return;
                    }
                    else if(isdigit(c)){
                        int intC = int(c)-48;
                        colIterationNum += intC;
                    }
                    else{
                        colIterationNum++;
                    }
                }
                else if(c == '/'){
                    rowIterationNum--;
                    colIterationNum = 1;
                }
            }
        }

        void updateStateGivenMove(Node *startNode, Node *finalNode){  //must continue here
            removeNode(startNode);
        }
};

int main(){

    //read FEN string
    int N = 1;
    // cin >> N;
    vector<string> positionOfPiecesArray(N);
    vector<char> sideToMoveArray(N);
    vector<int> moveNumberArray(N);
    vector<string> moveToBePlayedArray(N);
    string position = "4E2/1P1Pl2/7/7/1p2L1p/6P/7";
    char side = 'w';
    int moveNum = 12;
    string moveToBePlayed = "e7e6";
    for(int i = 0; i < N; i++){
        // cin >> position >> side >> moveNum >> moveToBePlayed;
        positionOfPiecesArray[i] = position;
        sideToMoveArray[i] = side;
        moveNumberArray[i] = moveNum;
        moveToBePlayedArray[i] = moveToBePlayed;
    }

    //create a grid
    for(int i = 0; i < N; i++){
        Grid grid(positionOfPiecesArray[i], sideToMoveArray[i], moveNumberArray[i]);
        grid.createGrid();
        // grid.printGrid();
        grid.addLocationOfPieces();
        grid.setNodeAdjacencies();
        // grid.printLocationOfPieces();
        // grid.printSideToMove();
        // cout << grid.whiteElephantPositions[0]->bottom->position;
         
        //lion's moves
        Node *nodeLion = grid.getLionTurnPosition();
        grid.getLionMoves(nodeLion);
        // grid.printPeiceMove(nodeLion);

        //zebra's moves
        Node *nodeZebra = grid.getZebraTurnPosition();
        if(nodeZebra != nullptr){
            grid.getZebraMoves(nodeZebra);
            // grid.printPeiceMove(nodeZebra);
        }

        //elephant's moves
        vector<Node*> nodeElephants = grid.getElephantsTurnPosition();
        for(Node *nodeElephant: nodeElephants){
            grid.getElephantsMoves(nodeElephant);
            // grid.printPeiceMove(nodeElephant);
        }

        //pawn's moves
        // bool b = false;
        vector<Node*> nodePawns = grid.getPawnsTurnPosition();
        for(Node *nodePawn: nodePawns){
            grid.getPawnsMoves(nodePawn);
            // grid.printPeiceMove(nodePawn);
        }

        // NB use!!!
        vector<Node*> moveFromAndTo = grid.getMoveFromAndTo(moveToBePlayedArray[i]);
        grid.updateStateGivenMove(moveFromAndTo[0], moveFromAndTo[1]);
        cout << grid.positionOfPieces << endl;
    }

    return 0;
}