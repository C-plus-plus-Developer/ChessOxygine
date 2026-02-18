#include "Game.h"

ChessGame::ChessGame(spStage s) : stage(s), selectedPos(-1, -1), isWhiteTurn(true), isMoving(false) {
    logMessage("ChessGame constructor");
    init();
}

ChessGame::~ChessGame() {
    logMessage("ChessGame destructor");
    
    for(int y=0; y<8; y++) {
        for(int x=0; x<8; x++) {
            if (board[y][x].sprite.get()) {
                board[y][x].sprite->detach();
                board[y][x].sprite = nullptr;
            }
        }
    }
    dyingPieces.clear();
    
    logMessage("ChessGame destructor finished");
}

void ChessGame::init() {
    logMessage("Initializing game");
    
    // Фон
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            spColorRectSprite tile = new ColorRectSprite();
            tile->setSize(TILE_SIZE, TILE_SIZE);
            tile->setPosition(BOARD_OFFSET_X + x * TILE_SIZE, BOARD_OFFSET_Y + y * TILE_SIZE);
            tile->setColor((x + y) % 2 == 0 ? Color(200, 200, 200) : Color(100, 100, 100));
            tile->setTouchEnabled(false);
            tile->attachTo(stage);
        }
    }

    setupPieces();
    logMessage("Game initialized");
}

void ChessGame::setupPieces() {
    logMessage("Setting up pieces");
    //Заполняем доску пешками
    for(int x=0; x<8; x++) {
        createPiece(x, 1, PAWN, false);
        createPiece(x, 6, PAWN, true);
    }
    //Потом остальными
    PieceType order[] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};
    for(int x=0; x<8; x++) {
        createPiece(x, 0, order[x], false);
        createPiece(x, 7, order[x], true);
    }
    logMessage("Pieces setup complete");
}

void ChessGame::createPiece(int x, int y, PieceType type, bool isWhite) {
    logMessage("Creating piece at " + to_string(x) + "," + to_string(y));
    
    board[y][x].data.type = type;
    board[y][x].data.isWhite = isWhite;
    board[y][x].data.active = true;
    board[y][x].data.hasMoved = false;
    board[y][x].isAlive = true;

    // Создаем цветной прямоугольник 
    spColorRectSprite pieceSprite = new ColorRectSprite();
    pieceSprite->setSize(TILE_SIZE * 0.8f, TILE_SIZE * 0.8f);
    
    // Устанавливаем цвет в зависимости от цвета фигуры
    updatePieceColor(pieceSprite, isWhite, false); // false = не выделен

    pieceSprite->setAnchor(0.5f, 0.5f);
    pieceSprite->setPosition(
        BOARD_OFFSET_X + x * TILE_SIZE + TILE_SIZE/2, 
        BOARD_OFFSET_Y + y * TILE_SIZE + TILE_SIZE/2
    );
    
    pieceSprite->setTouchEnabled(false);
    pieceSprite->attachTo(stage);
    
    board[y][x].sprite = pieceSprite;
    
    logMessage("Piece created");
}

void ChessGame::updatePieceColor(spColorRectSprite sprite, bool isWhite, bool isSelected) {
    if (isSelected) {
        sprite->setColor(Color::Green); // Зеленый для выбранной фигуры
    } else {
        if (isWhite) {
            sprite->setColor(Color(240, 230, 180)); // Светло-бежевый
        } else {
            sprite->setColor(Color(60, 60, 70));    // Темно-серый
        }
    }
}

bool ChessGame::checkMove(int fx, int fy, int tx, int ty) {
    if (!board[fy][fx].isAlive) return false;
    
    PieceData p = board[fy][fx].data;
    
    if (board[ty][tx].isAlive && board[ty][tx].data.isWhite == p.isWhite) return false;

    int dx = tx - fx;
    int dy = ty - fy;
    int absDx = abs(dx);
    int absDy = abs(dy);
    int dir = p.isWhite ? -1 : 1;

    // Проверка препятствий для слонов, ладей и ферзей
    if (p.type == ROOK || p.type == BISHOP || p.type == QUEEN) {
        if (dx != 0 && dy != 0 && absDx != absDy) return false;
        
        int stepX = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
        int stepY = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
        
        int x = fx + stepX;
        int y = fy + stepY;
        
        while (x != tx || y != ty) {
            if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
            if (board[y][x].isAlive) return false;
            x += stepX;
            y += stepY;
        }
    }

    switch (p.type) {
        case PAWN:
            if (p.isWhite && dy >= 0) return false;
            if (!p.isWhite && dy <= 0) return false;
            if (dx == 0) {
                if (board[ty][tx].isAlive) return false;
                if (absDy == 1) return true;
                if (absDy == 2 && !p.hasMoved) {
                    int midY = fy + dir;
                    if (midY < 0 || midY >= 8) return false;
                    if (board[midY][fx].isAlive) return false;
                    return true;
                }
            } else if (absDx == 1 && absDy == 1) {
                if (board[ty][tx].isAlive) return true;
            }
            return false;
        case KNIGHT: 
            return (absDx == 1 && absDy == 2) || (absDx == 2 && absDy == 1);
        case ROOK: 
        case BISHOP: 
        case QUEEN: 
            return true;
        case KING: 
            return (absDx <= 1 && absDy <= 1);
        default: 
            return false;
    }
}

void ChessGame::killPiece(int x, int y) {
    if (!board[y][x].isAlive) return;
    
    logMessage("Killing piece at " + to_string(x) + "," + to_string(y));
    
    spColorRectSprite piece = board[y][x].sprite;
    spColorRectSprite weakPiece = piece;
    
    spTween tween = piece->addTween(Actor::TweenAlpha(0), 300);
    tween->setDoneCallback([this, weakPiece](Event* ev) {
        logMessage("Piece death animation completed");
        
        for (size_t i = 0; i < dyingPieces.size(); i++) {
            if (dyingPieces[i].get() == weakPiece.get()) {
                weakPiece->detach();
                dyingPieces.erase(dyingPieces.begin() + i);
                logMessage("Piece removed from dyingPieces");
                break;
            }
        }
    });
    
    dyingPieces.push_back(piece);
    
    board[y][x].sprite = nullptr;
    board[y][x].isAlive = false;
    board[y][x].data.active = false;
}

void ChessGame::onClick(int gx, int gy) {
    if (isMoving) {
        logMessage("Move in progress, ignoring click");
        return;
    }

    if (gx < 0 || gx >= 8 || gy < 0 || gy >= 8) return;

    logMessage(string("onClick at ") + to_string(gx) + "," + to_string(gy));

    // ВЫБОР
    if (selectedPos.x == -1) {
        if (board[gy][gx].isAlive && board[gy][gx].data.isWhite == isWhiteTurn) {
            // Снимаем выделение с предыдущей выбранной фигуры 
            if (selectedPos.x != -1) {
                updatePieceColor(board[selectedPos.y][selectedPos.x].sprite, 
                                board[selectedPos.y][selectedPos.x].data.isWhite, false);
            }
            
            selectedPos = Point(gx, gy);
            updatePieceColor(board[gy][gx].sprite, board[gy][gx].data.isWhite, true);
            logMessage("Piece selected");
        }
    }
    // ХОД
    else {
        if (!board[selectedPos.y][selectedPos.x].isAlive) {
            logMessage("Selected piece is dead!");
            selectedPos = Point(-1, -1);
            return;
        }

        if (gx == selectedPos.x && gy == selectedPos.y) {
            // Отмена выбора
            logMessage("Cancelling selection");
            updatePieceColor(board[gy][gx].sprite, board[gy][gx].data.isWhite, false);
            selectedPos = Point(-1, -1);
        }
        else if (checkMove(selectedPos.x, selectedPos.y, gx, gy)) {
            logMessage("Move is valid, executing");
            
            isMoving = true;
            
            Piece movingPiece = board[selectedPos.y][selectedPos.x];
            int oldX = selectedPos.x;
            int oldY = selectedPos.y;
            
            // Захват
            if (board[gy][gx].isAlive) {
                logMessage("Capturing piece");
                killPiece(gx, gy);
            }

            // Обновляем логику
            board[gy][gx] = movingPiece;
            board[gy][gx].data.hasMoved = true;
            
            // Очищаем старую позицию
            board[oldY][oldX].sprite = nullptr;
            board[oldY][oldX].isAlive = false;
            board[oldY][oldX].data.active = false;

            // Анимируем движение
            Vector2 targetPos(
                BOARD_OFFSET_X + gx * TILE_SIZE + TILE_SIZE/2,
                BOARD_OFFSET_Y + gy * TILE_SIZE + TILE_SIZE/2
            );
            
            spTween moveTween = movingPiece.sprite->addTween(Actor::TweenPosition(targetPos), 200);
            moveTween->setDoneCallback([this](Event* ev) {
                logMessage("Move animation completed");
                this->isMoving = false;
            });

            // Возвращаем нормальный цвет
            updatePieceColor(movingPiece.sprite, movingPiece.data.isWhite, false);

            // Смена хода
            isWhiteTurn = !isWhiteTurn;
            selectedPos = Point(-1, -1);
            
            logMessage(string("Move complete. New turn: ") + (isWhiteTurn ? "White" : "Black"));
        } else {
            logMessage("Move is invalid");
            updatePieceColor(board[selectedPos.y][selectedPos.x].sprite, 
                            board[selectedPos.y][selectedPos.x].data.isWhite, false);
            selectedPos = Point(-1, -1);
        }
    }
}