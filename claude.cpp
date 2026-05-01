/*
 * ============================================================
 *   CHESS GAME IN C++ — OOP PROJECT
 *   Features: Classes, Inheritance, Polymorphism,
 *             Composition, Encapsulation
 *   Two-player console chess with full rule validation
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;

// ============================================================
//  ENUMS
// ============================================================
enum class Color { WHITE, BLACK, NONE };

// ============================================================
//  BASE CLASS: Piece  (Encapsulation + Inheritance root)
// ============================================================
class Piece {
protected:
    Color color;
    char  symbol;       // uppercase = White, lowercase = Black
    bool  hasMoved;

public:
    Piece(Color c, char sym) : color(c), hasMoved(false) {
        symbol = (c == Color::WHITE) ? toupper(sym) : tolower(sym);
    }
    virtual ~Piece() {}

    // --- Getters (Encapsulation) ---
    Color getColor()  const { return color;    }
    char  getSymbol() const { return symbol;   }
    bool  getHasMoved() const { return hasMoved; }
    void  setHasMoved(bool v) { hasMoved = v;  }

    bool isWhite() const { return color == Color::WHITE; }
    bool isBlack() const { return color == Color::BLACK; }

    // Pure virtual — Polymorphism
    virtual bool isValidMove(int fromRow, int fromCol,
                             int toRow,   int toCol,
                             const vector<vector<Piece*>>& board) const = 0;

    virtual string getName() const = 0;
};

// ============================================================
//  DERIVED CLASSES (Inheritance + Polymorphism)
// ============================================================

// ---------- PAWN ----------
class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c, 'P') {}
    string getName() const override { return "Pawn"; }

    bool isValidMove(int fr, int fc, int tr, int tc,
                     const vector<vector<Piece*>>& board) const override {
        int dir = (color == Color::WHITE) ? -1 : 1; // White moves up (decreasing row)
        int startRow = (color == Color::WHITE) ? 6 : 1;

        // One step forward
        if (tc == fc && tr == fr + dir && board[tr][tc] == nullptr)
            return true;

        // Two steps from starting row
        if (tc == fc && fr == startRow && tr == fr + 2 * dir
            && board[fr + dir][fc] == nullptr && board[tr][tc] == nullptr)
            return true;

        // Diagonal capture
        if (abs(tc - fc) == 1 && tr == fr + dir
            && board[tr][tc] != nullptr
            && board[tr][tc]->getColor() != color)
            return true;

        return false;
    }
};

// ---------- ROOK ----------
class Rook : public Piece {
public:
    Rook(Color c) : Piece(c, 'R') {}
    string getName() const override { return "Rook"; }

    bool isValidMove(int fr, int fc, int tr, int tc,
                     const vector<vector<Piece*>>& board) const override {
        if (fr == tr && fc == tc) return false;
        if (fr != tr && fc != tc) return false;

        // Check path is clear
        int dr = (tr == fr) ? 0 : (tr > fr ? 1 : -1);
        int dc = (tc == fc) ? 0 : (tc > fc ? 1 : -1);
        int r = fr + dr, c = fc + dc;
        while (r != tr || c != tc) {
            if (board[r][c] != nullptr) return false;
            r += dr; c += dc;
        }
        // Destination: empty or enemy
        return board[tr][tc] == nullptr || board[tr][tc]->getColor() != color;
    }
};

// ---------- KNIGHT ----------
class Knight : public Piece {
public:
    Knight(Color c) : Piece(c, 'N') {}
    string getName() const override { return "Knight"; }

    bool isValidMove(int fr, int fc, int tr, int tc,
                     const vector<vector<Piece*>>& board) const override {
        int dr = abs(tr - fr), dc = abs(tc - fc);
        if (!((dr == 2 && dc == 1) || (dr == 1 && dc == 2))) return false;
        return board[tr][tc] == nullptr || board[tr][tc]->getColor() != color;
    }
};

// ---------- BISHOP ----------
class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c, 'B') {}
    string getName() const override { return "Bishop"; }

    bool isValidMove(int fr, int fc, int tr, int tc,
                     const vector<vector<Piece*>>& board) const override {
        if (fr == tr && fc == tc) return false;
        if (abs(tr - fr) != abs(tc - fc)) return false;
        int dr = (tr > fr) ? 1 : -1;
        int dc = (tc > fc) ? 1 : -1;
        int r = fr + dr, c = fc + dc;
        while (r != tr || c != tc) {
            if (board[r][c] != nullptr) return false;
            r += dr; c += dc;
        }
        return board[tr][tc] == nullptr || board[tr][tc]->getColor() != color;
    }
};

// ---------- QUEEN ----------
class Queen : public Piece {
public:
    Queen(Color c) : Piece(c, 'Q') {}
    string getName() const override { return "Queen"; }

    bool isValidMove(int fr, int fc, int tr, int tc,
                     const vector<vector<Piece*>>& board) const override {
        if (fr == tr && fc == tc) return false;
        // Rook-like
        if (fr == tr || fc == tc) {
            int dr = (tr == fr) ? 0 : (tr > fr ? 1 : -1);
            int dc = (tc == fc) ? 0 : (tc > fc ? 1 : -1);
            int r = fr + dr, c = fc + dc;
            while (r != tr || c != tc) {
                if (board[r][c] != nullptr) return false;
                r += dr; c += dc;
            }
            return board[tr][tc] == nullptr || board[tr][tc]->getColor() != color;
        }
        // Bishop-like
        if (abs(tr - fr) == abs(tc - fc)) {
            int dr = (tr > fr) ? 1 : -1;
            int dc = (tc > fc) ? 1 : -1;
            int r = fr + dr, c = fc + dc;
            while (r != tr || c != tc) {
                if (board[r][c] != nullptr) return false;
                r += dr; c += dc;
            }
            return board[tr][tc] == nullptr || board[tr][tc]->getColor() != color;
        }
        return false;
    }
};

// ---------- KING ----------
class King : public Piece {
public:
    King(Color c) : Piece(c, 'K') {}
    string getName() const override { return "King"; }

    bool isValidMove(int fr, int fc, int tr, int tc,
                     const vector<vector<Piece*>>& board) const override {
        if (abs(tr - fr) > 1 || abs(tc - fc) > 1) return false;
        return board[tr][tc] == nullptr || board[tr][tc]->getColor() != color;
    }
};

// ============================================================
//  BOARD CLASS  (Composition: contains Piece pointers)
// ============================================================
class Board {
private:
    vector<vector<Piece*>> grid; // 8x8 board

    void clearBoard() {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++) {
                delete grid[r][c];
                grid[r][c] = nullptr;
            }
    }

public:
    Board() : grid(8, vector<Piece*>(8, nullptr)) { setupPieces(); }

    ~Board() { clearBoard(); }

    // Deep-copy for move simulation
    Board(const Board& other) : grid(8, vector<Piece*>(8, nullptr)) {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                if (other.grid[r][c]) {
                    Piece* p = other.grid[r][c];
                    Color  col = p->getColor();
                    char   sym = toupper(p->getSymbol());
                    Piece* np = nullptr;
                    if      (sym == 'P') np = new Pawn(col);
                    else if (sym == 'R') np = new Rook(col);
                    else if (sym == 'N') np = new Knight(col);
                    else if (sym == 'B') np = new Bishop(col);
                    else if (sym == 'Q') np = new Queen(col);
                    else if (sym == 'K') np = new King(col);
                    if (np) np->setHasMoved(p->getHasMoved());
                    grid[r][c] = np;
                }
    }

    void setupPieces() {
        clearBoard();
        // Black back rank
        grid[0][0] = new Rook(Color::BLACK);
        grid[0][1] = new Knight(Color::BLACK);
        grid[0][2] = new Bishop(Color::BLACK);
        grid[0][3] = new Queen(Color::BLACK);
        grid[0][4] = new King(Color::BLACK);
        grid[0][5] = new Bishop(Color::BLACK);
        grid[0][6] = new Knight(Color::BLACK);
        grid[0][7] = new Rook(Color::BLACK);
        for (int c = 0; c < 8; c++) grid[1][c] = new Pawn(Color::BLACK);

        // White back rank
        grid[7][0] = new Rook(Color::WHITE);
        grid[7][1] = new Knight(Color::WHITE);
        grid[7][2] = new Bishop(Color::WHITE);
        grid[7][3] = new Queen(Color::WHITE);
        grid[7][4] = new King(Color::WHITE);
        grid[7][5] = new Bishop(Color::WHITE);
        grid[7][6] = new Knight(Color::WHITE);
        grid[7][7] = new Rook(Color::WHITE);
        for (int c = 0; c < 8; c++) grid[6][c] = new Pawn(Color::WHITE);
    }

    Piece* getPiece(int r, int c) const { return grid[r][c]; }

    const vector<vector<Piece*>>& getGrid() const { return grid; }

    bool inBounds(int r, int c) const {
        return r >= 0 && r < 8 && c >= 0 && c < 8;
    }

    // Move piece (no validation here — caller validates)
    bool movePiece(int fr, int fc, int tr, int tc) {
        delete grid[tr][tc];
        grid[tr][tc] = grid[fr][fc];
        grid[fr][fc] = nullptr;
        grid[tr][tc]->setHasMoved(true);

        // Pawn promotion to Queen
        if (grid[tr][tc] && toupper(grid[tr][tc]->getSymbol()) == 'P') {
            if ((grid[tr][tc]->isWhite() && tr == 0) ||
                (grid[tr][tc]->isBlack() && tr == 7)) {
                Color c = grid[tr][tc]->getColor();
                delete grid[tr][tc];
                grid[tr][tc] = new Queen(c);
                cout << "  *** Pawn promoted to Queen! ***\n";
            }
        }
        return true;
    }

    // Find king position for given color
    pair<int,int> findKing(Color col) const {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                if (grid[r][c] && grid[r][c]->getColor() == col
                    && toupper(grid[r][c]->getSymbol()) == 'K')
                    return {r, c};
        return {-1, -1};
    }

    // Is square (r,c) attacked by 'attacker' color?
    bool isSquareAttacked(int r, int c, Color attacker) const {
        for (int fr = 0; fr < 8; fr++)
            for (int fc = 0; fc < 8; fc++) {
                Piece* p = grid[fr][fc];
                if (p && p->getColor() == attacker)
                    if (p->isValidMove(fr, fc, r, c, grid))
                        return true;
            }
        return false;
    }

    // Is 'col' king currently in check?
    bool isInCheck(Color col) const {
        auto [kr, kc] = findKing(col);
        if (kr == -1) return true; // King captured = always in check
        Color opp = (col == Color::WHITE) ? Color::BLACK : Color::WHITE;
        return isSquareAttacked(kr, kc, opp);
    }

    // Does moving (fr,fc)->(tr,tc) leave own king in check?
    bool moveLeavesKingInCheck(int fr, int fc, int tr, int tc, Color col) const {
        Board temp(*this);
        temp.movePiece(fr, fc, tr, tc);
        return temp.isInCheck(col);
    }

    // Does 'col' have any legal move?
    bool hasLegalMoves(Color col) const {
        for (int fr = 0; fr < 8; fr++)
            for (int fc = 0; fc < 8; fc++) {
                Piece* p = grid[fr][fc];
                if (!p || p->getColor() != col) continue;
                for (int tr = 0; tr < 8; tr++)
                    for (int tc = 0; tc < 8; tc++)
                        if (p->isValidMove(fr, fc, tr, tc, grid))
                            if (!moveLeavesKingInCheck(fr, fc, tr, tc, col))
                                return true;
            }
        return false;
    }

    // Print the board
    void display() const {
        cout << "\n    a   b   c   d   e   f   g   h\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";
        for (int r = 0; r < 8; r++) {
            cout << (8 - r) << " |";
            for (int c = 0; c < 8; c++) {
                bool darkSquare = (r + c) % 2 != 0;
                if (grid[r][c]) {
                    cout << " " << grid[r][c]->getSymbol() << " |";
                } else {
                    cout << (darkSquare ? ":::": "   ") << "|";
                }
            }
            cout << " " << (8 - r) << "\n";
            cout << "  +---+---+---+---+---+---+---+---+\n";
        }
        cout << "    a   b   c   d   e   f   g   h\n\n";
    }
};

// ============================================================
//  GAME CLASS
// ============================================================
class Game {
private:
    Board      board;
    Color      currentTurn;
    int        moveCount;
    bool       gameOver;
    string     winner;

    // Convert "e2" -> (row=6, col=4)
    bool parseSquare(const string& s, int& row, int& col) const {
        if (s.size() < 2) return false;
        col = s[0] - 'a';
        row = 8 - (s[1] - '0');
        return board.inBounds(row, col);
    }

    string colorName(Color c) const {
        return (c == Color::WHITE) ? "White" : "Black";
    }

    void printLegend() const {
        cout << "  Pieces: P=Pawn  R=Rook  N=Knight  B=Bishop  Q=Queen  K=King\n";
        cout << "  UPPERCASE=White  lowercase=Black  :::=dark square\n\n";
    }

    void printStatus() const {
        cout << "  Move " << moveCount << " | "
             << colorName(currentTurn) << "'s turn";
        if (board.isInCheck(currentTurn))
            cout << "  *** CHECK! ***";
        cout << "\n";
    }

public:
    Game() : currentTurn(Color::WHITE), moveCount(1), gameOver(false) {}

    void run() {
        cout << "\n";
        cout << "  ╔══════════════════════════════════╗\n";
        cout << "  ║      CHESS GAME IN C++ (OOP)     ║\n";
        cout << "  ║   Two-Player Console Edition     ║\n";
        cout << "  ╚══════════════════════════════════╝\n\n";
        cout << "  Enter moves as: <from><to>  (e.g., e2e4)\n";
        cout << "  Type 'resign' to forfeit. Type 'help' for legend.\n\n";
        printLegend();

        while (!gameOver) {
            board.display();
            printStatus();

            // Check for checkmate / stalemate before asking for input
            if (!board.hasLegalMoves(currentTurn)) {
                if (board.isInCheck(currentTurn)) {
                    Color opp = (currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;
                    cout << "\n  *** CHECKMATE! " << colorName(opp) << " wins! ***\n\n";
                } else {
                    cout << "\n  *** STALEMATE! The game is a draw. ***\n\n";
                }
                gameOver = true;
                break;
            }

            string input;
            cout << "  >>> ";
            cin >> input;

            if (input == "resign") {
                Color opp = (currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;
                cout << "\n  " << colorName(currentTurn) << " resigned. "
                     << colorName(opp) << " wins!\n\n";
                gameOver = true;
                break;
            }

            if (input == "help") {
                printLegend();
                continue;
            }

            // Parse move
            if (input.size() < 4) {
                cout << "  Invalid input. Enter move like 'e2e4'.\n\n";
                continue;
            }

            string fromStr = input.substr(0, 2);
            string toStr   = input.substr(2, 2);
            int fr, fc, tr, tc;

            if (!parseSquare(fromStr, fr, fc) || !parseSquare(toStr, tr, tc)) {
                cout << "  Invalid square. Columns a-h, rows 1-8.\n\n";
                continue;
            }

            Piece* piece = board.getPiece(fr, fc);

            if (!piece) {
                cout << "  No piece at " << fromStr << ".\n\n";
                continue;
            }
            if (piece->getColor() != currentTurn) {
                cout << "  That is not your piece.\n\n";
                continue;
            }
            if (!piece->isValidMove(fr, fc, tr, tc, board.getGrid())) {
                cout << "  Illegal move for " << piece->getName() << ".\n\n";
                continue;
            }
            if (board.moveLeavesKingInCheck(fr, fc, tr, tc, currentTurn)) {
                cout << "  That move leaves your King in check!\n\n";
                continue;
            }

            // Execute move
            board.movePiece(fr, fc, tr, tc);

            // Switch turn
            currentTurn = (currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;
            if (currentTurn == Color::WHITE) moveCount++;
        }

        cout << "  Thank you for playing!\n\n";
    }
};

// ============================================================
//  MAIN
// ============================================================
int main() {
    Game chess;
    chess.run();
    return 0;
}