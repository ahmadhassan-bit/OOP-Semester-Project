#include<iostream>
using namespace std;

class Piece {
public:
    string color;
    string symbol;
    int row;
    int col;

    Piece() {
        color = "none";
        symbol = ".";
        row = 0;
        col = 0;
    }

    Piece(string c, string s, int r, int co) {
        color = c;
        symbol = s;
        row = r;
        col = co;
    }

    void setPosition(int r, int c) {
        row = r;
        col = c;
    }

    string getColor() {
        return color;
    }

    string getSymbol() {
        return symbol;
    }

    virtual string getType() {
        return "Piece";
    }

    virtual bool isValidMove(int toRow, int toCol) {
        return false;
    }
};

class Pawn : public Piece {
public:
    Pawn(string c, int r, int co) {
        color = c;
        row = r;
        col = co;
        if (c == "White") {
            symbol = "P";
        } else {
            symbol = "p";
        }
    }

    string getType() {
        return "Pawn";
    }

    bool isValidMove(int toRow, int toCol) {
        if (color == "White") {
            if (toCol == col && toRow == row - 1) {
                return true;
            }
            if (toCol == col && toRow == row - 2 && row == 6) {
                return true;
            }
        } else {
            if (toCol == col && toRow == row + 1) {
                return true;
            }
            if (toCol == col && toRow == row + 2 && row == 1) {
                return true;
            }
        }
        return false;
    }
};

class Rook : public Piece {
public:
    Rook(string c, int r, int co) {
        color = c;
        row = r;
        col = co;
        if (c == "White") {
            symbol = "R";
        } else {
            symbol = "r";
        }
    }

    string getType() {
        return "Rook";
    }

    bool isValidMove(int toRow, int toCol) {
        if (toRow == row && toCol != col) {
            return true;
        }
        if (toCol == col && toRow != row) {
            return true;
        }
        return false;
    }
};

class Knight : public Piece {
public:
    Knight(string c, int r, int co) {
        color = c;
        row = r;
        col = co;
        if (c == "White") {
            symbol = "N";
        } else {
            symbol = "n";
        }
    }

    string getType() {
        return "Knight";
    }

    bool isValidMove(int toRow, int toCol) {
        int dr = toRow - row;
        int dc = toCol - col;
        if (dr < 0) dr = -dr;
        if (dc < 0) dc = -dc;
        if (dr == 2 && dc == 1) {
            return true;
        }
        if (dr == 1 && dc == 2) {
            return true;
        }
        return false;
    }
};

class Bishop : public Piece {
public:
    Bishop(string c, int r, int co) {
        color = c;
        row = r;
        col = co;
        if (c == "White") {
            symbol = "B";
        } else {
            symbol = "b";
        }
    }

    string getType() {
        return "Bishop";
    }

    bool isValidMove(int toRow, int toCol) {
        int dr = toRow - row;
        int dc = toCol - col;
        if (dr < 0) dr = -dr;
        if (dc < 0) dc = -dc;
        if (dr == dc) {
            return true;
        }
        return false;
    }
};

class Queen : public Piece {
public:
    Queen(string c, int r, int co) {
        color = c;
        row = r;
        col = co;
        if (c == "White") {
            symbol = "Q";
        } else {
            symbol = "q";
        }
    }

    string getType() {
        return "Queen";
    }

    bool isValidMove(int toRow, int toCol) {
        int dr = toRow - row;
        int dc = toCol - col;
        if (dr < 0) dr = -dr;
        if (dc < 0) dc = -dc;
        if (toRow == row && toCol != col) {
            return true;
        }
        if (toCol == col && toRow != row) {
            return true;
        }
        if (dr == dc) {
            return true;
        }
        return false;
    }
};

class King : public Piece {
public:
    King(string c, int r, int co) {
        color = c;
        row = r;
        col = co;
        if (c == "White") {
            symbol = "K";
        } else {
            symbol = "k";
        }
    }

    string getType() {
        return "King";
    }

    bool isValidMove(int toRow, int toCol) {
        int dr = toRow - row;
        int dc = toCol - col;
        if (dr < 0) dr = -dr;
        if (dc < 0) dc = -dc;
        if (dr <= 1 && dc <= 1) {
            return true;
        }
        return false;
    }
};

class Board {
public:
    Piece* grid[8][8];
    bool whiteKingAlive;
    bool blackKingAlive;

    Board() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                grid[i][j] = NULL;
            }
        }
        whiteKingAlive = true;
        blackKingAlive = true;
        setupBoard();
    }

    void setupBoard() {
        grid[0][0] = new Rook("Black", 0, 0);
        grid[0][1] = new Knight("Black", 0, 1);
        grid[0][2] = new Bishop("Black", 0, 2);
        grid[0][3] = new Queen("Black", 0, 3);
        grid[0][4] = new King("Black", 0, 4);
        grid[0][5] = new Bishop("Black", 0, 5);
        grid[0][6] = new Knight("Black", 0, 6);
        grid[0][7] = new Rook("Black", 0, 7);

        for (int i = 0; i < 8; i++) {
            grid[1][i] = new Pawn("Black", 1, i);
        }

        for (int i = 0; i < 8; i++) {
            grid[6][i] = new Pawn("White", 6, i);
        }

        grid[7][0] = new Rook("White", 7, 0);
        grid[7][1] = new Knight("White", 7, 1);
        grid[7][2] = new Bishop("White", 7, 2);
        grid[7][3] = new Queen("White", 7, 3);
        grid[7][4] = new King("White", 7, 4);
        grid[7][5] = new Bishop("White", 7, 5);
        grid[7][6] = new Knight("White", 7, 6);
        grid[7][7] = new Rook("White", 7, 7);
    }

    void displayBoard() {
        cout << "\n  a b c d e f g h" << endl;
        cout << "  ________________" << endl;
        for (int i = 0; i < 8; i++) {
            cout << 8 - i << "|";
            for (int j = 0; j < 8; j++) {
                if (grid[i][j] == NULL) {
                    cout << ". ";
                } else {
                    cout << grid[i][j]->getSymbol() << " ";
                }
            }
            cout << "|" << endl;
        }
        cout << "  ________________" << endl;
    }

    bool isGameOver() {
        if (!whiteKingAlive) {
            cout << "\n*** Black wins! White King has been captured! ***" << endl;
            return true;
        }
        if (!blackKingAlive) {
            cout << "\n*** White wins! Black King has been captured! ***" << endl;
            return true;
        }
        return false;
    }

    bool movePiece(int fromRow, int fromCol, int toRow, int toCol, string currentTurn) {
        if (fromRow < 0 || fromRow > 7 || fromCol < 0 || fromCol > 7) {
            cout << "Invalid position! Row and col must be 0-7." << endl;
            return false;
        }
        if (toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) {
            cout << "Invalid position! Row and col must be 0-7." << endl;
            return false;
        }

        Piece* p = grid[fromRow][fromCol];

        if (p == NULL) {
            cout << "No piece at that position!" << endl;
            return false;
        }

        if (p->getColor() != currentTurn) {
            cout << "That is not your piece!" << endl;
            return false;
        }

        if (grid[toRow][toCol] != NULL && grid[toRow][toCol]->getColor() == currentTurn) {
            cout << "You cannot capture your own piece!" << endl;
            return false;
        }

        if (!p->isValidMove(toRow, toCol)) {
            cout << "Invalid move for " << p->getType() << "!" << endl;
            return false;
        }

        if (grid[toRow][toCol] != NULL) {
            string capturedType = grid[toRow][toCol]->getType();
            string capturedColor = grid[toRow][toCol]->getColor();
            cout << currentTurn << " captured " << capturedColor << " " << capturedType << "!" << endl;

            if (capturedType == "King") {
                if (capturedColor == "White") {
                    whiteKingAlive = false;
                } else {
                    blackKingAlive = false;
                }
            }

            delete grid[toRow][toCol];
        }

        grid[toRow][toCol] = p;
        grid[fromRow][fromCol] = NULL;
        p->setPosition(toRow, toCol);

        return true;
    }

    ~Board() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (grid[i][j] != NULL) {
                    delete grid[i][j];
                    grid[i][j] = NULL;
                }
            }
        }
    }
};

int main() {
    Board b;
    string turn = "White";
    int fr, fc, tr, tc;

    cout << "=============================" << endl;
    cout << "     Welcome to Chess!       " << endl;
    cout << "=============================" << endl;
    cout << "How to play:" << endl;
    cout << "  Enter: fromRow fromCol toRow toCol" << endl;
    cout << "  Rows and cols are 0 to 7" << endl;
    cout << "  Top-left = (0,0), Bottom-right = (7,7)" << endl;
    cout << "  White pieces are UPPERCASE" << endl;
    cout << "  Black pieces are lowercase" << endl;
    cout << "  Capture the King to win!" << endl;

    while (true) {
        b.displayBoard();

        if (b.isGameOver()) {
            break;
        }

        cout << "\n" << turn << "'s turn" << endl;
        cout << "Enter move (fromRow fromCol toRow toCol): ";
        cin >> fr >> fc >> tr >> tc;

        bool moved = b.movePiece(fr, fc, tr, tc, turn);

        if (moved) {
            if (turn == "White") {
                turn = "Black";
            } else {
                turn = "White";
            }
        } else {
            cout << "Invalid move. Try again." << endl;
        }
    }

    cout << "\nGame Over. Thanks for playing!" << endl;

    return 0;
}
