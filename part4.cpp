#include <iostream>
#include <vector>
#include <ctype.h>
#include <string>

using namespace std;

// class Node;

// class PossibleMove{
//     public:
//         Node *nodeFrom;
//         Node *nodeTo;
//         string positionOfPieces;
//         string sideToMove;
//         int moveNumber;
//         string winner;
//         char piece; 
//         int value;
//         vector<bool> inRiverForColorTurn;

//         PossibleMove(Node *nFrom, Node *nTo, char p){
//             nodeFrom = nFrom;
//             nodeTo = nTo;
//             piece = p;
            
//             char pieceCaptured = nTo->piece;
//             if(pieceCaptured == 'P'){
//                 value = 1;
//             }
//             else if(pieceCaptured == 'p'){
//                 value = 1; 
//             }
//             else if(pieceCaptured == 'E'){
//                 value = 1;
//             }
//             else if(pieceCaptured == 'e'){
//                 value = 1;
//             }
//             else if(pieceCaptured == 'L'){
//                 value = 11;
//             }
//             else if(pieceCaptured == 'l'){
//                 value = 11;
//             }
//             else if(pieceCaptured == 'Z'){
//                 value = 1;
//             }
//             else if(pieceCaptured == 'z'){
//                 value = 1;
//             }
//             else{
//                 value = 0;
//             }
//         }
// };

class Node{
    public:
        char file;
        int rank;
        char piece;
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
            piece = v;
            columnNum = column;
            rowNum = row;
        }
};

class PossibleMove{
    public:
        Node *nodeFrom;
        Node *nodeTo;
        string positionOfPieces;
        string sideToMove;
        int moveNumber;
        string winner;
        char piece; 
        int value;
        vector<bool> inRiverForColorTurn;

        PossibleMove(Node *nFrom, Node *nTo, char p){
            nodeFrom = nFrom;
            nodeTo = nTo;
            piece = p;
            
            char pieceCaptured = nTo->piece;
            if(pieceCaptured == 'P'){
                value = 1;
            }
            else if(pieceCaptured == 'p'){
                value = 1; 
            }
            else if(pieceCaptured == 'E'){
                value = 1;
            }
            else if(pieceCaptured == 'e'){
                value = 1;
            }
            else if(pieceCaptured == 'L'){
                value = 11;
            }
            else if(pieceCaptured == 'l'){
                value = 11;
            }
            else if(pieceCaptured == 'Z'){
                value = 1;
            }
            else if(pieceCaptured == 'z'){
                value = 1;
            }
            else{
                value = 0;
            }
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

        vector<PossibleMove> moves;
        vector<PossibleMove> movesNotTurn;

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
            char c;
            for(int position = 0; position < positionOfPieces.length(); position++){
                c = positionOfPieces[position];
                if(isdigit(c)){
                    int intC = int(c)-48;
                    for(int k = 0; k < intC; k++){
                        Node node(j, i, 'y');
                        grid.push_back(node);
                        j++;
                    }
                }
                else if(c == '/'){
                    i--;
                    j = 1;
                }
                else{
                    Node node(j, i, c);
                    grid.push_back(node);
                    j++;
                }
            }
        }

        void printGrid(){
            for(int i = 0; i < 7; i++){
                for(int j = 0; j < 7; j++){
                    if(j != 6){
                        cout << grid[i * 7 + j].piece << " ";
                    }
                    else{
                        cout << grid[i * 7 + j].piece << endl;
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
            cout << node->position << " = " << node->piece << endl;
        }

        void addLocationOfPiecesAndSetNodeAdjacencies(){
            for(int j = 7; j >= 1; j--){
                for(int i = 1; i <= 7; i++){
                    Node *node = getNode(i, j);
                    char piece = node->piece;
                    if(piece == 'P'){
                        whitePawnPositions.push_back(node);
                    }
                    else if(piece == 'p'){
                        blackPawnPositions.push_back(node); 
                    }
                    else if(piece == 'E'){
                        whiteElephantPositions.push_back(node);
                    }
                    else if(piece == 'e'){
                        blackElephantPositions.push_back(node);
                    }
                    else if(piece == 'L'){
                        whiteLionPosition = node;
                    }
                    else if(piece == 'l'){
                        blackLionPosition = node;
                    }
                    else if(piece == 'Z'){
                        whiteZebraPosition = node;
                    }
                    else if(piece == 'z'){
                        blackZebraPosition = node;
                    }

                    // setNodeAdjacencies
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

        void printSideToMove(){
            cout << "side to play: " << sideToMove;
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
                else if(colorGoinTo == "white"){  //we are going to a white piece
                    if(!isWhite){
                        return true;
                    }
                }
                else{  //we are going to a black piece
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
            char piece = node->piece;
            if(piece == 'y'){
                return "empty";
            }
            else if(isCapital(piece)){
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
                            if(temp->piece == 'l'){
                                return temp;
                            }
                        }
                    }
                    else if(node->columnNum == 3){
                        temp = node->topRight;
                        string colorGoinTo = getColor(temp);
                        if(colorGoinTo == "empty"){
                            temp = temp->topRight;
                            if(temp->piece == 'l'){
                                return temp;
                            }
                        }
                    }
                }
                temp = node->top;
                while(temp != nullptr){
                    if(temp->piece == 'l'){
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
                            if(temp->piece == 'L'){
                                return temp;
                            }
                        }
                    }
                    else if(node->columnNum == 3){
                        temp = node->bottomRight;
                        string colorGoinTo = getColor(temp);
                        if(colorGoinTo == "empty"){
                            temp = temp->bottomRight;
                            if(temp->piece == 'L'){
                                return temp;
                            }
                        }
                    }
                }
                temp = node->bottom;
                while(temp != nullptr){
                    if(temp->piece == 'L'){
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
            char piece = node->piece;
            vector<Node*> aroundNode = {node->bottomLeft, node->left, node->topLeft, node->bottom, node->top, node->bottomRight, node->right, node->topRight};
            for(Node *nodeTo: aroundNode){
                if(isBlockValid(myColor, nodeTo)){
                    if(isInCastle(myColor, nodeTo)){
                        PossibleMove possibleMove(node, nodeTo, piece);
                        moves.push_back(possibleMove);
                    }
                }
            }
            if(lionToLion != nullptr){
                PossibleMove possibleMove(node, lionToLion, piece);
                moves.push_back(possibleMove);
            }
        }

        void getLionMovesNotTurn(Node *node){
            string pos = node->position;
            string myColor = getColor(node);
            Node *lionToLion = getLionToLionMove(node, myColor);
            char piece = node->piece;
            vector<Node*> aroundNode = {node->bottomLeft, node->left, node->topLeft, node->bottom, node->top, node->bottomRight, node->right, node->topRight};
            for(Node *nodeTo: aroundNode){
                if(isBlockValid(myColor, nodeTo)){
                    if(isInCastle(myColor, nodeTo)){
                        PossibleMove possibleMove(node, nodeTo, piece);
                        movesNotTurn.push_back(possibleMove);
                    }
                }
            }
            if(lionToLion != nullptr){
                PossibleMove possibleMove(node, lionToLion, piece);
                movesNotTurn.push_back(possibleMove);
            }
        }

        void getZebraMoves(Node *node){
            Node *nodeTo, *temp;
            string pos = node->position;
            string myColor = getColor(node);
            char piece = node->piece;
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
                        PossibleMove possibleMove(node, temp, piece);
                        moves.push_back(possibleMove);
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
                        PossibleMove possibleMove(node, temp, piece);
                        moves.push_back(possibleMove);
                    }
                }
            }
        }

        void getZebraMovesNotTurn(Node *node){
            Node *nodeTo, *temp;
            string pos = node->position;
            string myColor = getColor(node);
            char piece = node->piece;
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
                        PossibleMove possibleMove(node, temp, piece);
                        movesNotTurn.push_back(possibleMove);
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
                        PossibleMove possibleMove(node, temp, piece);
                        movesNotTurn.push_back(possibleMove);
                    }
                }
            }
        }

        void getElephantsMoves(Node *node){
            Node *nodeTo, *temp;
            string pos = node->position;
            string myColor = getColor(node);
            char piece = node->piece;
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
                            PossibleMove possibleMove(node, nodeTo, piece);
                            moves.push_back(possibleMove);
                        }
                        temp = nodeTo->top;
                    }
                    else{
                        if(isBlockValid(myColor, nodeTo)){
                            PossibleMove possibleMove(node, nodeTo, piece);
                            moves.push_back(possibleMove);
                        }
                        temp = nodeTo->right;
                    }
                }
                else{
                    continue;
                }

                if(temp != nullptr){
                    if(isBlockValid(myColor, temp)){
                        PossibleMove possibleMove(node, temp, piece);
                        moves.push_back(possibleMove);
                    }
                }
                if(i == 2 || i == 3){
                    continue;
                }
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    moves.push_back(possibleMove);
                }
            }
        }

        void getElephantsMovesNotTurn(Node *node){
            Node *nodeTo, *temp;
            string pos = node->position;
            string myColor = getColor(node);
            char piece = node->piece;
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
                            PossibleMove possibleMove(node, nodeTo, piece);
                            movesNotTurn.push_back(possibleMove);
                        }
                        temp = nodeTo->top;
                    }
                    else{
                        if(isBlockValid(myColor, nodeTo)){
                            PossibleMove possibleMove(node, nodeTo, piece);
                            movesNotTurn.push_back(possibleMove);
                        }
                        temp = nodeTo->right;
                    }
                }
                else{
                    continue;
                }

                if(temp != nullptr){
                    if(isBlockValid(myColor, temp)){
                        PossibleMove possibleMove(node, temp, piece);
                        movesNotTurn.push_back(possibleMove);
                    }
                }
                if(i == 2 || i == 3){
                    continue;
                }
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    movesNotTurn.push_back(possibleMove);
                }
            }
        }

        void getPawnsMoves(Node *node){
            Node *nodeTo, *temp;
            string pos = node->position;
            string myColor = getColor(node);
            char piece = node->piece;
            if(myColor == "white"){
                nodeTo = node->topLeft;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    moves.push_back(possibleMove);
                }
                if(node->rank > 4){
                    nodeTo = node->bottom;
                    if(nodeTo != nullptr){
                        if(nodeTo->piece == 'y'){
                            temp = nodeTo->bottom;
                            if(temp->piece == 'y'){
                                PossibleMove possibleMove(node, temp, piece);
                                moves.push_back(possibleMove);
                            }
                        }
                        if(nodeTo != nullptr && nodeTo->piece == 'y'){
                            PossibleMove possibleMove(node, nodeTo, piece);
                            moves.push_back(possibleMove);
                        }
                    }
                }
                nodeTo = node->top;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    moves.push_back(possibleMove);
                }
                nodeTo = node->topRight;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    moves.push_back(possibleMove);
                }
            }
            else if(myColor == "black"){
                nodeTo = node->bottomLeft;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    moves.push_back(possibleMove);
                }
                nodeTo = node->bottom;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    moves.push_back(possibleMove);
                }
                if(node->rank < 4){
                    nodeTo = node->top;
                    if(nodeTo != nullptr){
                        if(nodeTo->piece == 'y'){
                            PossibleMove possibleMove(node, nodeTo, piece);
                            moves.push_back(possibleMove);
                            temp = nodeTo->top;
                            if(temp->piece == 'y'){
                                PossibleMove possibleMove(node, temp, piece);
                                moves.push_back(possibleMove);
                            }
                        }
                    }
                }
                nodeTo = node->bottomRight;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    moves.push_back(possibleMove);
                }
            }
        }

        void getPawnsMovesNotTurn(Node *node){
            Node *nodeTo, *temp;
            string pos = node->position;
            string myColor = getColor(node);
            char piece = node->piece;
            if(myColor == "white"){
                nodeTo = node->topLeft;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    movesNotTurn.push_back(possibleMove);
                }
                if(node->rank > 4){
                    nodeTo = node->bottom;
                    if(nodeTo != nullptr){
                        if(nodeTo->piece == 'y'){
                            temp = nodeTo->bottom;
                            if(temp->piece == 'y'){
                                PossibleMove possibleMove(node, temp, piece);
                                movesNotTurn.push_back(possibleMove);
                            }
                        }
                        if(nodeTo != nullptr && nodeTo->piece == 'y'){
                            PossibleMove possibleMove(node, nodeTo, piece);
                            movesNotTurn.push_back(possibleMove);
                        }
                    }
                }
                nodeTo = node->top;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    movesNotTurn.push_back(possibleMove);
                }
                nodeTo = node->topRight;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    movesNotTurn.push_back(possibleMove);
                }
            }
            else if(myColor == "black"){
                nodeTo = node->bottomLeft;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    movesNotTurn.push_back(possibleMove);
                }
                nodeTo = node->bottom;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    movesNotTurn.push_back(possibleMove);
                }
                if(node->rank < 4){
                    nodeTo = node->top;
                    if(nodeTo != nullptr){
                        if(nodeTo->piece == 'y'){
                            PossibleMove possibleMove(node, nodeTo, piece);
                            movesNotTurn.push_back(possibleMove);
                            temp = nodeTo->top;
                            if(temp->piece == 'y'){
                                PossibleMove possibleMove(node, temp, piece);
                                movesNotTurn.push_back(possibleMove);
                            }
                        }
                    }
                }
                nodeTo = node->bottomRight;
                if(isBlockValid(myColor, nodeTo)){
                    PossibleMove possibleMove(node, nodeTo, piece);
                    movesNotTurn.push_back(possibleMove);
                }
            }
        }

        // void printPeiceMove(Node *node){
        //     vector<PossibleMove> possibleMoves = node->possibleMovesAndFuturState;
        //     string pos = node->position;
        //     for(int i = 0; i < possibleMoves.size(); i++){
        //         cout << pos << possibleMoves[i].nodeTo->position;
        //         if(i != possibleMoves.size()-1){
        //             cout << " ";
        //         }
        //     }
        //     cout << endl;
        // }

        Node* getLionTurnPosition(){
            if(sideToMove == "white"){
                return whiteLionPosition;
            }
            return blackLionPosition;
        }

        Node* getLionNotTurnPosition(){
            if(sideToMove == "white"){
                return blackLionPosition;
            }
            return whiteLionPosition;
        }

        Node* getZebraTurnPosition(){
            if(sideToMove == "white"){
                return whiteZebraPosition;
            }
            return blackZebraPosition;
        }

        Node* getZebraNotTurnPosition(){
            if(sideToMove == "white"){
                return blackZebraPosition;
            }
            return whiteZebraPosition;
        }

        vector<Node*> getElephantsTurnPosition(){
            if(sideToMove == "white"){
                return whiteElephantPositions;
            }
            return blackElephantPositions;
        }

        vector<Node*> getElephantsNotTurnPosition(){
            if(sideToMove == "white"){
                return blackElephantPositions;
            }
            return whiteElephantPositions;
        }

        vector<Node*> getPawnsTurnPosition(){
            if(sideToMove == "white"){
                return whitePawnPositions;
            }
            return blackPawnPositions;
        }

        vector<Node*> getPawnsNotTurnPosition(){
            if(sideToMove == "white"){
                return blackPawnPositions;
            }
            return whitePawnPositions;
        }

        vector<Node*> getMoveFromAndTo(string moveToBePlayed){
            vector<Node*> moveFromAndTo;
            Node *startNode = getNode(int(moveToBePlayed[0])-96, int(moveToBePlayed[1])-48);
            moveFromAndTo.push_back(startNode);
            Node *endNode = getNode(int(moveToBePlayed[2])-96, int(moveToBePlayed[3])-48);
            moveFromAndTo.push_back(endNode);
            return moveFromAndTo;
        }

        void updateStringAfterNodeRemoved(int position, PossibleMove &possibleMove){ 
            possibleMove.positionOfPieces.erase(position, 1);
            char before = '\0', after = '\0';
            if(position > 0){
                before = possibleMove.positionOfPieces[position-1];
            }
            if(position < possibleMove.positionOfPieces.size()){
                after = possibleMove.positionOfPieces[position];
            }
            if(isdigit(before)){
                if(isdigit(after)){
                    possibleMove.positionOfPieces.erase(position-1, 2);
                    int total = int(before)-48 +1 + int(after)-48;
                    possibleMove.positionOfPieces.insert(position-1, to_string(total));
                }
                else{
                    possibleMove.positionOfPieces.erase(position-1, 1);
                    int total = int(before)-48 +1;
                    possibleMove.positionOfPieces.insert(position-1, to_string(total));
                }
            }
            else{
                if(isdigit(after)){
                    possibleMove.positionOfPieces.erase(position, 1);
                    int total = int(after)-48 +1;
                    possibleMove.positionOfPieces.insert(position, to_string(total));
                }
                else{
                    possibleMove.positionOfPieces.insert(position, "1");
                }
            }
        }

        void updateStringAfterNodeAdded(int position, string piece, int colNumBefore, Node *finalNode, PossibleMove &possibleMove){
            int colNumCurrent = finalNode->columnNum;
            char before = '\0', after = '\0';
            if(position > 0){  //must check that this is correct
                before = possibleMove.positionOfPieces[position-1];
            }
            if(position < possibleMove.positionOfPieces.size()){  //must check that this is correct
                after = possibleMove.positionOfPieces[position];
            }
            if(after == '1'){
                possibleMove.positionOfPieces.erase(position, 1);
                possibleMove.positionOfPieces.insert(position, piece);
            }
            else if(isdigit(before)){ // no digit after possible
                int totalBefore = colNumCurrent-colNumBefore;
                int totalAfter = (int(before)-48) -totalBefore -1;
                if(totalBefore > 0){
                    possibleMove.positionOfPieces.erase(position-1, 1);
                    possibleMove.positionOfPieces.insert(position-1, to_string(totalBefore));
                    possibleMove.positionOfPieces.insert(position, piece);
                }
                else{
                    possibleMove.positionOfPieces.erase(position-1, 1);
                    possibleMove.positionOfPieces.insert(position-1, piece);
                }
                if(totalAfter >0){
                    possibleMove.positionOfPieces.insert(position+1, to_string(totalAfter));
                }
            }
            else{
                int totalAfter = (int(after)-48) -1;
                if(totalAfter > 0){
                    possibleMove.positionOfPieces.erase(position, 1);
                    possibleMove.positionOfPieces.insert(position, piece);
                    possibleMove.positionOfPieces.insert(position+1, to_string(totalAfter));
                }
                else{  //need to check
                    possibleMove.positionOfPieces.erase(position, 1);
                    possibleMove.positionOfPieces.insert(position, piece);
                }
            }
        }

        void removeNode(Node *node, PossibleMove &possibleMove){
            int rowNum = node->rowNum;
            int colNum = node->columnNum;
            int rowIterationNum = 7;
            int colIterationNum = 1;
            for(int position = 0; position < possibleMove.positionOfPieces.length()+1; position++){
                char c = '\0';
                if(position == possibleMove.positionOfPieces.length()){
                    c = '/';
                }
                else{
                    c = possibleMove.positionOfPieces[position];
                }
                if(rowNum == rowIterationNum){
                    if(colNum == colIterationNum){  //update string here
                        updateStringAfterNodeRemoved(position, possibleMove);
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

        void addNode(Node *node, Node *finalNode, PossibleMove &possibleMove){
            string stringPiece(1, node->piece);
            int rowNum = finalNode->rowNum;
            int colNum = finalNode->columnNum;
            int rowIterationNum = 7;
            int colIterationNum = 1;
            int intC =0;
            for(int position = 0; position < possibleMove.positionOfPieces.length()+1; position++){
                char c = '\0';
                if(position == possibleMove.positionOfPieces.length()){
                    c = '/';
                }
                else{
                    c = possibleMove.positionOfPieces[position];
                }
                if(rowNum == rowIterationNum){
                    if(colNum <= colIterationNum){  //<= because we can add extra to the colIterationNum
                        updateStringAfterNodeAdded(position, stringPiece, colIterationNum-intC, finalNode, possibleMove);  //update string here
                        return;
                    }
                    else if(isdigit(c)){
                        intC = int(c)-48;
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

        void switchTurns(PossibleMove &possibleMove){
            if(sideToMove == "white"){
                possibleMove.sideToMove = "black";
            }
            else{
                possibleMove.sideToMove = "white";
            }
        }

        void updateTurnCount(PossibleMove &possibleMove){
            if(possibleMove.sideToMove == "white"){
                possibleMove.moveNumber++;
            }
        }

        void setInRiverForColorTurn(PossibleMove &possibleMove){
            int rowIterationNum = 7;
            int colIterationNum = 1;
            for(int position = 0; position < possibleMove.positionOfPieces.length()+1; position++){
                char c = '\0';
                if(position == possibleMove.positionOfPieces.length()){
                    c = '/';
                }
                else{
                    c = possibleMove.positionOfPieces[position];
                }
                if(rowIterationNum == 4){
                    if(c == '/'){
                        return;
                    }
                    else if(isdigit(c)){
                        int intC = int(c)-48;
                        colIterationNum += intC;
                    }
                    else if(isCapital(c) && sideToMove == "white" || !isCapital(c) && sideToMove == "black"){  //update string here
                        possibleMove.inRiverForColorTurn[colIterationNum-1] = true;
                    }
                    else{
                        colIterationNum++;
                    }
                }
                else if(c == '/'){
                    rowIterationNum--;
                }
            }
        }

        void removeRiverForColorTurn(PossibleMove &possibleMove){
            int drownPieceCol;
            for(int i =0; i<possibleMove.inRiverForColorTurn.size(); i++){
                if(possibleMove.inRiverForColorTurn[i]){
                    drownPieceCol = i+1;
                    break;
                }
            }
            int rowIterationNum = 7;
            int colIterationNum = 1;
            for(int position = 0; position < possibleMove.positionOfPieces.length()+1; position++){
                char c = '\0';
                if(position == possibleMove.positionOfPieces.length()){
                    c = '/';
                }
                else{
                    c = possibleMove.positionOfPieces[position];
                }
                if(rowIterationNum == 4){
                    if(c == '/'){
                        return;
                    }
                    else if(isdigit(c)){
                        int intC = int(c)-48;
                        colIterationNum += intC;
                    }
                    else if(colIterationNum == drownPieceCol){  //update string here
                        updateStringAfterNodeRemoved(position, possibleMove);
                        return;
                    }
                    else{
                        colIterationNum++;
                    }
                }
                else if(c == '/'){
                    rowIterationNum--;
                }
            }
        }

        void updateStateGivenMove(PossibleMove &possibleMove){  //must continue here
            Node *startNode = possibleMove.nodeFrom;
            Node *finalNode = possibleMove.nodeTo;
            possibleMove.positionOfPieces = positionOfPieces;  //initialize
            possibleMove.moveNumber = moveNumber;  //initialize
            possibleMove.inRiverForColorTurn = {false, false, false, false, false, false, false};
            setInRiverForColorTurn(possibleMove);
            removeNode(startNode, possibleMove);
            if(finalNode->piece != 'y'){  //not empty (capture)
                if(finalNode->piece == 'l'){
                    possibleMove.winner = "white";
                }
                else if(finalNode->piece == 'L'){
                    possibleMove.winner = "black";
                }
                removeNode(finalNode, possibleMove);
            }
            addNode(startNode, finalNode, possibleMove);
            switchTurns(possibleMove);
            updateTurnCount(possibleMove);
            if(startNode->rowNum == 4){
                if(finalNode->rowNum == 4){
                    possibleMove.inRiverForColorTurn[finalNode->columnNum-1] = true;
                }
                possibleMove.inRiverForColorTurn[startNode->columnNum-1] = false;
            }
            removeRiverForColorTurn(possibleMove);
        }

        int evaluationFunction(){
            int whiteTurn = 0;
            int blackTurn = 0;
            int rawScore;
            if(whiteLionPosition != nullptr){  //my lion's turn is alive
                if(blackLionPosition != nullptr){  //lion's opp turn is alive
                    if(whiteZebraPosition != nullptr){
                        whiteTurn+=300;
                    }
                    if(blackZebraPosition != nullptr){
                        blackTurn+=300;
                    }
                    whiteTurn+=whiteElephantPositions.size()*200;
                    blackTurn+=blackElephantPositions.size()*200;
                    whiteTurn+=whitePawnPositions.size()*100;
                    blackTurn+=blackPawnPositions.size()*100;
                    rawScore = whiteTurn - blackTurn;
                }
                else{ //black lion is dead
                    rawScore = 10000;
                }
            }
            else{  //white lion is dead
                rawScore = -10000;
            }
            if(sideToMove == "black"){
                rawScore*=-1;
            }
            return rawScore;
        }

        int evaluationFunctionAdvanced(){
            //Material Score
            int whiteTurn = 0;
            int blackTurn = 0;
            int materialScore;
            if(whiteLionPosition != nullptr){  //my lion's turn is alive
                if(blackLionPosition != nullptr){  //lion's opp turn is alive
                    if(whiteZebraPosition != nullptr){
                        whiteTurn+=300;
                    }
                    if(blackZebraPosition != nullptr){
                        blackTurn+=300;
                    }
                    whiteTurn+=whiteElephantPositions.size()*200;
                    blackTurn+=blackElephantPositions.size()*200;
                    whiteTurn+=whitePawnPositions.size()*100;
                    blackTurn+=blackPawnPositions.size()*100;
                    materialScore = whiteTurn - blackTurn;
                    if(whiteTurn ==0 && blackTurn ==0){
                        return materialScore;
                    }
                }
                else{ //black lion is dead
                    materialScore = 10000;
                    if(sideToMove == "black"){
                        materialScore*=-1;
                    }
                    return materialScore;
                }
            }
            else{  //white lion is dead
                materialScore = -10000;
                if(sideToMove == "black"){
                    materialScore*=-1;
                }
                return materialScore;
            }

            //Mobility Score
            int mobilityScoreWhite =0;
            int mobilityScoreBlack =0; 
            int mobilityScore = 0;
            int attackScoreTurn =0;
            int attackScoreNotTurn =0;
            int attackScoreWhite = 0;
            int attackScoreBlack = 0;
            if(sideToMove =="white"){
                mobilityScoreWhite = moves.size();
                mobilityScoreBlack = movesNotTurn.size();
            }
            else{
                mobilityScoreWhite = movesNotTurn.size();
                mobilityScoreBlack = moves.size();
            }
            mobilityScore = mobilityScoreWhite - mobilityScoreBlack;
            
            //Attack Score
            for(PossibleMove move: moves){
                attackScoreTurn+= move.value;
            }
            for(PossibleMove move: movesNotTurn){
                attackScoreNotTurn+= move.value;
            }
            if(sideToMove =="white"){
                attackScoreWhite = attackScoreTurn;
                attackScoreBlack = attackScoreNotTurn;
            }
            else{
                attackScoreWhite = attackScoreNotTurn;
                attackScoreBlack = attackScoreTurn;
            }
            int attackScore = attackScoreWhite - attackScoreBlack;

            int rawScore = materialScore + mobilityScore + attackScore;
            if(sideToMove == "black"){
                rawScore*=-1;
            }
            return rawScore;
        }
};

bool isGameOver(int rawScore){
    if(rawScore == -10000 || rawScore == 10000) {
        return true;
    }
    return false;
}

Grid makeMove(Grid currentState, PossibleMove &possibleMove){
    currentState.updateStateGivenMove(possibleMove);
    Grid nextState(possibleMove.positionOfPieces, possibleMove.sideToMove[0], possibleMove.moveNumber);
    return nextState;
}

int miniMax(Grid currentState, int depth){
    int rawScore = currentState.evaluationFunction();
    bool gameOver = isGameOver(rawScore);
    if(gameOver || depth <=0){
        return rawScore;
    }
    int value = -10000000;
    int eval;
    for(PossibleMove move: currentState.moves){
        //all for the new state
        Grid nextState = makeMove(currentState, move);
        nextState.createGrid();
        nextState.addLocationOfPiecesAndSetNodeAdjacencies();

        //lion's moves
        Node *nodeLion = nextState.getLionTurnPosition();
        if(nodeLion != nullptr){
            nextState.getLionMoves(nodeLion);
            // grid.printPeiceMove(nodeLion);
        }

        //zebra's moves
        Node *nodeZebra = nextState.getZebraTurnPosition();
        if(nodeZebra != nullptr){
            nextState.getZebraMoves(nodeZebra);
            // grid.printPeiceMove(nodeZebra);
        }

        //elephant's moves
        vector<Node*> nodeElephants = nextState.getElephantsTurnPosition();
        for(Node *nodeElephant: nodeElephants){
            nextState.getElephantsMoves(nodeElephant);
            // grid.printPeiceMove(nodeElephant);
        }

        //pawn's moves
        vector<Node*> nodePawns = nextState.getPawnsTurnPosition();
        for(Node *nodePawn: nodePawns){
            nextState.getPawnsMoves(nodePawn);
            // grid.printPeiceMove(nodePawn);
        }
        
        eval = -miniMax(nextState, depth-1);
        value = max(value, eval);
    }
    return value;
}

int main(){

    //read FEN string
    int N = 1;
    cin >> N;
    vector<string> positionOfPiecesArray(N);
    vector<char> sideToMoveArray(N);
    vector<int> moveNumberArray(N);
    vector<string> moveToBePlayedArray(N);
    string position = "7/7/4l2/7/3L3/7/7";
    char side = 'w';
    int moveNum = 46;
    // string moveToBePlayed = "d1d2";
    for(int i = 0; i < N; i++){
        cin >> position >> side >> moveNum;
        positionOfPiecesArray[i] = position;
        sideToMoveArray[i] = side;
        moveNumberArray[i] = moveNum;
        // moveToBePlayedArray[i] = moveToBePlayed;
    }

    //create a grid
    for(int i = 0; i < N; i++){
        Grid grid(positionOfPiecesArray[i], sideToMoveArray[i], moveNumberArray[i]);
        grid.createGrid();
        // grid.printGrid();
        grid.addLocationOfPiecesAndSetNodeAdjacencies();
        // grid.printLocationOfPieces();
        // grid.printSideToMove();

        //lion's moves
        Node *nodeLion = grid.getLionTurnPosition();
        if(nodeLion != nullptr){
            grid.getLionMoves(nodeLion);
            // grid.printPeiceMove(nodeLion);
        }
        Node *nodeLionNotTurn = grid.getLionNotTurnPosition();
        if(nodeLionNotTurn != nullptr){
            grid.getLionMovesNotTurn(nodeLionNotTurn);
            // grid.printPeiceMove(nodeLion);
        }

        //zebra's moves
        Node *nodeZebra = grid.getZebraTurnPosition();
        if(nodeZebra != nullptr){
            grid.getZebraMoves(nodeZebra);
            // grid.printPeiceMove(nodeZebra);
        }
        Node *nodeZebraNotTurn = grid.getZebraNotTurnPosition();
        if(nodeZebraNotTurn != nullptr){
            grid.getZebraMovesNotTurn(nodeZebraNotTurn);
            // grid.printPeiceMove(nodeZebra);
        }

        //elephant's moves
        vector<Node*> nodeElephants = grid.getElephantsTurnPosition();
        for(Node *nodeElephant: nodeElephants){
            grid.getElephantsMoves(nodeElephant);
            // grid.printPeiceMove(nodeElephant);
        }
        vector<Node*> nodeElephantsNotTurn = grid.getElephantsNotTurnPosition();
        for(Node *nodeElephantNotTurn: nodeElephantsNotTurn){
            grid.getElephantsMovesNotTurn(nodeElephantNotTurn);
            // grid.printPeiceMove(nodeElephant);
        }

        //pawn's moves
        vector<Node*> nodePawns = grid.getPawnsTurnPosition();
        for(Node *nodePawn: nodePawns){
            grid.getPawnsMoves(nodePawn);
            // grid.printPeiceMove(nodePawn);
        }
        vector<Node*> nodePawnsNotTurn = grid.getPawnsNotTurnPosition();
        for(Node *nodePawnNotTurn: nodePawnsNotTurn){
            grid.getPawnsMovesNotTurn(nodePawnNotTurn);
            // grid.printPeiceMove(nodePawn);
        }

        // PossibleMove possibleMove = grid.moves[0];
        // cout <<grid.moves[0].moveNumber << endl;
        // PossibleMove *possibleMove = grid.getPossibleMove(moveFromAndTo[0], moveFromAndTo[1]);
        // cout << possibleMove.positionOfPieces << " " << possibleMove.sideToMove[0] << " " << possibleMove.moveNumber<< endl;
        // if(possibleMove.winner == "white"){
        //     cout << "White wins" << endl;
        // }
        // else if(possibleMove.winner == "black"){
        //     cout << "Black wins" << endl;
        // }
        // else{
        //     cout << "Continue" << endl;
        // }

        int eval = grid.evaluationFunctionAdvanced();
        cout << eval << endl;

        // int minimax = miniMax(grid, 2);
        // cout << minimax << endl;
        // for(PossibleMove move: grid.moves){
        //     cout <<move.nodeFrom->position<<move.nodeTo->position<<" "<<move.value << endl;
        // }
    }

    return 0;
}