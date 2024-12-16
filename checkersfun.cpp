#include "checkers.h"

Board::Board() {
    initialize();
}

void Board::initialize() {
    for (int i = 0; i < BS; i++) {
        for (int j = 0; j < BS; j++) {
            if ((i + j) % 2 == 1) {
                if (i < 3)
                    board[i][j] = P1;
                else if (i > 4)
                    board[i][j] = P2;
                else
                    board[i][j] = EMPTY;
            } else {
                board[i][j] = EMPTY;
            }
        }
    }
}

bool Board::bound(int x, int y) const {
    return x >= 0 && x < BS && y >= 0 && y < BS;
}

bool Board::empty(int x, int y) const {
    return board[x][y] == EMPTY;
}

char Board::getp(int x, int y) const {
    return board[x][y];
}

void Board::set(int x, int y, char get) {
    board[x][y] = get;
}

char& Board::operator()(int x, int y) {
    return board[x][y];
}

bool Board::operator==(const Board& b) const {
    for (int i = 0; i < BS; i++) {
        for (int j = 0; j < BS; j++) {
            if (board[i][j] != b.board[i][j]) {
                return false;
            }
        }
    }
    return true;
}

ostream& operator<<(ostream& os, const Board& b) {
    os << "  ";
    for (int i = 0; i < BS; i++)
        os << i << " ";
    os << endl;
    for (int i = 0; i < BS; i++) {
        os << i << " ";
        for (int j = 0; j < BS; j++) {
            os << b.board[i][j] << " ";
        }
        os << endl;
    }
    return os;
}

bool Board::boardmove(int x1, int y1, int x2, int y2, char player) {
    if (!bound(x2, y2) || !empty(x2, y2)) {
        return false;
    }
    int dir = (player == P1) ? 1 : -1;
    if (x2 == x1 + dir && abs(y2 - y1) == 1) {
        swap(board[x1][y1], board[x2][y2]);
        return true;
    } else if (x2 == x1 + 2 * dir && abs(y2 - y1) == 2) {
        int xMid = (x1 + x2) / 2;
        int yMid = (y1 + y2) / 2;
        char opp = (player == P1) ? P2 : P1;
        if (board[xMid][yMid] == opp) {
            board[x1][y1] = EMPTY;
            board[xMid][yMid] = EMPTY;
            board[x2][y2] = player;
            return true;
        }
    }
    return false;
}

bool Board::has_valid_moves(char player) const {
    for (int i = 0; i < BS; i++) {
        for (int j = 0; j < BS; j++) {
            if (board[i][j] == player) {
                int dir = (player == P1) ? 1 : -1;
                for (int dy = -1; dy <= 1; dy += 2) {
                    int x2 = i + dir;
                    int y2 = j + dy;
                    if (bound(x2, y2) && empty(x2, y2)) {
                        return true;
                    }
                    x2 = i + 2 * dir;
                    y2 = j + 2 * dy;
                    if (bound(x2, y2) && empty(x2, y2)) {
                        int xMid = (i + x2) / 2;
                        int yMid = (j + y2) / 2;
                        if (board[xMid][yMid] != EMPTY && board[xMid][yMid] != board[i][j]) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

Player::Player(char get, bool iscomputer) : PlayerBase(get, iscomputer) {}

bool Player::playermove(Board& board) {
    int x1, y1, x2, y2;
    while (true) {
        if (iscomputer) {
            for (int i = 0; i < BS; ++i) {
                for (int j = 0; j < BS; ++j) {
                    if (board.getp(i, j) == get) {
                        for (int dx = -2; dx <= 2; dx++) {
                            for (int dy = -2; dy <= 2; dy++) {
                                x2 = i + dx;
                                y2 = j + dy;
                                if (board.boardmove(i, j, x2, y2, get)) {
                                    cout << "Computer (" << get << ") moved from (" << i << ", " << j << ") to (" << x2 << ", " << y2 << ").\n";
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
            throw runtime_error("No valid moves for computer");
        } else {
            cout << "Player (" << get << "), enter the coordinates of the piece you want to move (row col): ";
            cin >> x1 >> y1;
            cout << "Enter the coordinates of the destination (row col): ";
            cin >> x2 >> y2;
            if (board.bound(x1, y1) && board.getp(x1, y1) == get && board.boardmove(x1, y1, x2, y2, get)) {
                return true;
            } else {
                cout << "Invalid move. Try again.\n";
            }
        }
    }
}

Game::Game(int gm) {
    p1 = new Player(P1, gm == 3);
    p2 = new Player(P2, gm != 1);
    cp = p1;
}

Game::~Game() {
    delete p1;
    delete p2;
}

void Game::sp() {
    cp = (cp == p1) ? p2 : p1;
}

void Game::play() {
    while (true) {
        cout << board;
        cout << "Current turn: " << cp->getp() << endl;
        if (cp->playermove(board)) {
            if (check_game_over()) {
                declare_winner();
                break;
            }
            sp();
        }
    }
}

bool Game::check_game_over() {
    bool p1HasMoves = board.has_valid_moves(P1);
    bool p2HasMoves = board.has_valid_moves(P2);
    return !p1HasMoves && !p2HasMoves;
}

void Game::declare_winner() {
    bool p1HasMoves = board.has_valid_moves(P1);
    bool p2HasMoves = board.has_valid_moves(P2);

    if (!p1HasMoves && !p2HasMoves) {
        cout << "The game is a draw!\n";
    } else if (!p1HasMoves) {
        cout << "Player 2 (O) wins!\n";
    } else if (!p2HasMoves) {
        cout << "Player 1 (X) wins!\n";
    }
}
