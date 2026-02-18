#pragma once

#include "PieceData.h"
#include "oxygine-framework.h"
#include <SDL.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>

using namespace oxygine;
using namespace std;

const int TILE_SIZE = 70;
const int BOARD_OFFSET_X = 200;
const int BOARD_OFFSET_Y = 40;

class ChessGame 
{
private:
    struct Piece {
        spColorRectSprite sprite;
        PieceData data;
        bool isAlive;
        
        Piece() : isAlive(false) {}
    };
    
    Piece board[8][8];
    vector<spColorRectSprite> dyingPieces;

    void logMessage(const string& msg) {
        cout << "DEBUG: " << msg << endl;
    }

public:
    Point selectedPos;
    bool isWhiteTurn;
    spStage stage;
    bool isMoving;

    ChessGame(spStage s);
    ~ChessGame();

    void init();
    void updatePieceColor(spColorRectSprite sprite, bool isWhite, bool isSelected);
    void createPiece(int x, int y, PieceType type, bool isWhite);
    void setupPieces();
    bool checkMove(int fx, int fy, int tx, int ty);
    void killPiece(int x, int y);
    void onClick(int gx, int gy);
};