#include <iostream>
#include <cmath>
using namespace std;

class Board;

// ================= PIECE BASE =================
class Piece {
protected:
    char color;
    bool hasMoved;

public:
    Piece(char c) : color(c), hasMoved(false) {}

    char getColor() const {
        return color;
    }

    bool moved() const {
        return hasMoved;
    }

    void setMoved() {
        hasMoved = true;
    }

    virtual char getSymbol() const = 0;

    virtual bool isValidMove(int startX, int startY,
                             int destX, int destY,
                             Board* board) = 0;

    virtual ~Piece() {}
};

// ================= PIECES =================

class Rook : public Piece {
public:
    Rook(char c) : Piece(c) {}

    char getSymbol() const override {
        return (color == 'W') ? 'R' : 'r';
    }

    bool isValidMove(int startX, int startY,
                     int destX, int destY,
                     Board* board) override {
        
        if (startX == destX || startY == destY) {
            return board->pathClear(startX, startY, destX, destY);
        }

        return false;
    }
};

class Bishop : public Piece {
public:
    Bishop(char c) : Piece(c) {}

    char getSymbol() const override {
        return (color == 'W') ? 'B' : 'b';
    }

    bool isValidMove(int startX, int startY,
                     int destX, int destY,
                     Board* board) override {

        return abs(startX - destX) == abs(startY - destY) &&
               board->pathClear(startX, startY, destX, destY);
    }
};

class Queen : public Piece {
public:
    Queen(char c) : Piece(c) {}
    char getSymbol() { return color == 'W' ? 'Q' : 'q'; }

    bool isValidMove(int sx, int sy, int dx, int dy, Board* b) {
        return (sx == dx || sy == dy || abs(sx - dx) == abs(sy - dy))
            && b->pathClear(sx, sy, dx, dy);
    }
};
class Knight : public Piece {
public:
    Knight(char c) : Piece(c) {}

    char getSymbol() {
        if (color == 'W')
            return 'N';
        else
            return 'n';
    }
    bool isValidMove(int sx, int sy, int dx, int dy, Board*) {
        int r = abs(sx - dx);
        int c = abs(sy - dy);

        if (r == 2 && c == 1)
            return true;

        if (r == 1 && c == 2)
            return true;

        return false;
    }
};

class King : public Piece {
public:
    King(char c) : Piece(c) {}
    char getSymbol() { return color == 'W' ? 'K' : 'k'; }

    bool isValidMove(int sx, int sy, int dx, int dy, Board*) {
        return (abs(sx - dx) <= 1 && abs(sy - dy) <= 1) 
    || (sx == dx && abs(sy - dy) == 2);
    }
};

class Pawn : public Piece {
public:
    Pawn(char c) : Piece(c) {}
    char getSymbol() {
    if (color == 'W')
        return 'P';
    else
        return 'p';
}

    bool isValidMove(int sx, int sy, int dx, int dy, Board* b) {

        int dir = (color == 'W') ? -1 : 1;

        // forward 1
        if (sy == dy && b->grid[dx][dy] == NULL) {

            if (dx == sx + dir)
                return true;

            // first move double
            if ((color == 'W' && sx == 6) || (color == 'B' && sx == 1)) {
                if (dx == sx + 2 * dir &&
                    b->grid[sx + dir][sy] == NULL &&
                    b->grid[dx][dy] == NULL)
                    return true;
            }
        }

        // capture
        if (abs(dy - sy) == 1 && dx == sx + dir) {
            return (b->grid[dx][dy] && b->grid[dx][dy]->getColor() != color);
        }

        return false;
    }
};
// ================= BOARD =================
class Board {
public:
    Piece* grid[8][8];

    Board() {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                grid[i][j] = NULL;
    }

    bool inside(int x, int y) {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    bool pathClear(int sx, int sy, int dx, int dy) {
        int stepX = (dx == sx) ? 0 : (dx > sx ? 1 : -1);
        int stepY = (dy == sy) ? 0 : (dy > sy ? 1 : -1);

        int x = sx + stepX;
        int y = sy + stepY;

        while (x != dx || y != dy) {
            if (grid[x][y] != NULL)
                return false;
            x += stepX;
            y += stepY;
        }
        return true;
    }

    // ================= KING FIND =================
    void findKing(char turn, int &kx, int &ky) {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (grid[i][j] &&
                    grid[i][j]->getColor() == turn &&
                    grid[i][j]->getSymbol() == (turn == 'W' ? 'K' : 'k')) {
                    kx = i; ky = j;
                    return;
                }
    }

    // ================= CHECK =================
    bool isCheck(char turn) {
        int kx = -1, ky = -1;
        findKing(turn, kx, ky);

        if (kx == -1) return true;

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (grid[i][j] && grid[i][j]->getColor() != turn)
                    if (grid[i][j]->isValidMove(i, j, kx, ky, this))
                        return true;

        return false;
    }

    // ================= SIMULATION =================
    bool simulate(int sx, int sy, int dx, int dy, char turn) {
        Piece* a = grid[sx][sy];
        Piece* b = grid[dx][dy];

        grid[dx][dy] = grid[sx][sy];
        grid[sx][sy] = NULL;

        bool safe = !isCheck(turn);

        grid[sx][sy] = a;
        grid[dx][dy] = b;

        return safe;
    }

    // ================= MOVE =================
    bool move(int sx, int sy, int dx, int dy, char turn) {

        if (!inside(sx, sy) || !inside(dx, dy))
            return false;

        Piece* p = grid[sx][sy];
        if (!p || p->getColor() != turn)
            return false;

        if (grid[dx][dy] && grid[dx][dy]->getColor() == turn)
            return false;

        if (!p->isValidMove(sx, sy, dx, dy, this))
            return false;

        if (!simulate(sx, sy, dx, dy, turn))
            return false;

        // adding castling
if ((p->getSymbol() == 'K' || p->getSymbol() == 'k') && !p->moved()) {

    if (sx == dx && abs(dy - sy) == 2) {

        // King side
        if (dy == 6) {
            Piece* rook = grid[sx][7];

            if (rook && !rook->moved() &&
                grid[sx][5] == NULL && grid[sx][6] == NULL &&
                !isCheck(turn) &&
                simulate(sx, sy, sx, 5, turn) &&
                simulate(sx, sy, sx, 6, turn)) {

                grid[sx][6] = p;
                grid[sx][4] = NULL;

                grid[sx][5] = rook;
                grid[sx][7] = NULL;

                p->setMoved();
                rook->setMoved();

                return true;
            }
        }

        // Queen side
        if (dy == 2) {
            Piece* rook = grid[sx][0];

            if (rook && !rook->moved() &&
                grid[sx][1] == NULL &&
                grid[sx][2] == NULL &&
                grid[sx][3] == NULL &&
                !isCheck(turn) &&
                simulate(sx, sy, sx, 3, turn) &&
                simulate(sx, sy, sx, 2, turn)) {

                grid[sx][2] = p;
                grid[sx][4] = NULL;

                grid[sx][3] = rook;
                grid[sx][0] = NULL;

                p->setMoved();
                rook->setMoved();

                return true;
            }
        }
    }
}

        delete grid[dx][dy];
        grid[dx][dy] = p;
        grid[sx][sy] = NULL;
        grid[dx][dy]->setMoved();

      
  //adding pawn promotion
if (grid[dx][dy] && (grid[dx][dy]->getSymbol() == 'P' || grid[dx][dy]->getSymbol() == 'p')) {

    if ((grid[dx][dy]->getColor() == 'W' && dx == 0) ||
        (grid[dx][dy]->getColor() == 'B' && dx == 7)) {

        char choice;
        cout << "Promote pawn to (Q/R/B/N): ";
        cin >> choice;

        delete grid[dx][dy];

        switch (toupper(choice)) {
            case 'R': grid[dx][dy] = new Rook(turn); break;
            case 'B': grid[dx][dy] = new Bishop(turn); break;
            case 'N': grid[dx][dy] = new Knight(turn); break;
            default:  grid[dx][dy] = new Queen(turn); break;
        }
    }
}
        return true;
    }

    // ================= CHECKMATE =================
    bool checkmate(char turn) {
        if (!isCheck(turn))
            return false;

        for (int sx = 0; sx < 8; sx++) {
            for (int sy = 0; sy < 8; sy++) {

                if (grid[sx][sy] && grid[sx][sy]->getColor() == turn) {

                    for (int dx = 0; dx < 8; dx++) {
                        for (int dy = 0; dy < 8; dy++) {

                            if (grid[sx][sy]->isValidMove(sx, sy, dx, dy, this)) {
                                if (simulate(sx, sy, dx, dy, turn))
                                    return false;
                            }
                        }
                    }
                }
            }
        }
        return true;
    }

    // ================= DISPLAY =================
    void display() {
        cout << "\n   0 1 2 3 4 5 6 7\n";
        for (int i = 0; i < 8; i++) {
            cout << i << "  ";
            for (int j = 0; j < 8; j++) {
                if (grid[i][j]) cout << grid[i][j]->getSymbol() << " ";
                else cout << ". ";
            }
            cout << endl;
        }
    }
};

// ================= GAME =================
class Game {
public:
    Board b;
    char turn;

    Game() {
        turn = 'W';

        for (int i = 0; i < 8; i++) {
            b.grid[1][i] = new Pawn('B');
            b.grid[6][i] = new Pawn('W');
        }

        b.grid[0][0] = new Rook('B');
        b.grid[0][7] = new Rook('B');
        b.grid[7][0] = new Rook('W');
        b.grid[7][7] = new Rook('W');

        b.grid[0][1] = new Knight('B');
        b.grid[0][6] = new Knight('B');
        b.grid[7][1] = new Knight('W');
        b.grid[7][6] = new Knight('W');

        b.grid[0][2] = new Bishop('B');
        b.grid[0][5] = new Bishop('B');
        b.grid[7][2] = new Bishop('W');
        b.grid[7][5] = new Bishop('W');

        b.grid[0][3] = new Queen('B');
        b.grid[7][3] = new Queen('W');

        b.grid[0][4] = new King('B');
        b.grid[7][4] = new King('W');
    }

    void play() {
        int sx, sy, dx, dy;

        while (true) {
            b.display();

            if (b.isCheck(turn))
                cout << "CHECK!\n";

            if (b.checkmate(turn)) {
                cout << "CHECKMATE! "
                     << (turn == 'W' ? "Black" : "White")
                     << " wins!\n";
                break;
            }

            cout << (turn == 'W' ? "White" : "Black") << " move: ";
            cin >> sx >> sy >> dx >> dy;

            if (!b.move(sx, sy, dx, dy, turn)) {
                cout << "Invalid move!\n";
                continue;
            }

            turn = (turn == 'W') ? 'B' : 'W';
        }
    }
};

int main() {
    Game g;
    g.play();
}
