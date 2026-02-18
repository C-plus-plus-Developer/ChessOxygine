#pragma once
enum PieceType { EMPTY, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };

struct PieceData {
    PieceType type;
    bool isWhite;
    bool hasMoved;
    bool active;
    PieceData() : type(EMPTY), isWhite(true), hasMoved(false), active(false) {}
};