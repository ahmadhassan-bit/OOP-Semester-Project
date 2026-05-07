// ============================ chess.h ============================

#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include <cmath>
#include <cctype>

using namespace std;

class Board;

// ================= PIECE BASE =================
class Piece {
protected:
    char color;
    bool hasMoved;

public:
    Piece(char c);

    char getColor() const;

    bool moved() const;

    void setMoved();

    virtual char getSymbol() const = 0;

    virtual bool isValidMove(int startX, int startY,
                             int destX, int destY,
                             Board* board) = 0;

    virtual ~Piece() {}
};

// ================= PIECES =================

class Rook : public Piece {
public:
    Rook(char c);

    char getSymbol() const override;

    bool isValidMove(int startX, int startY,
                     int destX, int destY,
                     Board* board) override;
};

class Bishop : public Piece {
public:
    Bishop(char c);

    char getSymbol() const override;

    bool isValidMove(int startX, int startY,
                     int destX, int destY,
                     Board* board) override;
};

class Queen : public Piece {
public:
    Queen(char c);

    char getSymbol() const override;

    bool isValidMove(int startX, int startY,
                     int destX, int destY,
                     Board* board) override;
};

class Knight : public Piece {
public:
    Knight(char c);

    char getSymbol() const override;

    bool isValidMove(int sx, int sy, int dx, int dy, Board*) override;
};

class King : public Piece {
public:
    King(char c);

    char getSymbol() const override;

    bool isValidMove(int sx, int sy, int dx, int dy, Board*) override;
};

class Pawn : public Piece {
public:
    Pawn(char c);

    char getSymbol() const override;

    bool isValidMove(int sx, int sy, int dx, int dy, Board* b) override;
};

// ================= BOARD =================

class Board {
public:
    Piece* grid[8][8];

    Board();

    bool inside(int x, int y);

    bool pathClear(int sx, int sy, int dx, int dy);

    void findKing(char turn, int &kx, int &ky);

    bool isCheck(char turn);

    bool simulate(int sx, int sy, int dx, int dy, char turn);

    bool move(int sx, int sy, int dx, int dy, char turn);

    bool checkmate(char turn);

    void display();
};

// ================= GAME =================

class Game {
public:
    Board b;
    char turn;

    Game();

    void play();
};

#endif