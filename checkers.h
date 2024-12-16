#include <iostream>
#include <stdexcept>
using namespace std;

const int BS = 8;        
const char EMPTY = '.';  
const char P1 = 'x';    
const char P2 = 'o';  

class Board {
private:
    char board[BS][BS];
public:
    Board();
    void initialize();
    bool bound(int x, int y) const;
    bool empty(int x, int y) const;
    char getp(int x, int y) const;
    void set(int x, int y, char get);
    char& operator()(int x, int y);
    bool operator==(const Board& b) const;
    friend ostream& operator<<(ostream& os, const Board& b);
    bool boardmove(int x1, int y1, int x2, int y2, char player);
    bool has_valid_moves(char player) const;
};
class PlayerBase {
protected:
    char get; 
    bool iscomputer;
public:
    PlayerBase(char get, bool iscomputer) : get(get), iscomputer(iscomputer) {}
    virtual ~PlayerBase() {}
    virtual char getp() const { return get; }
    virtual bool playermove(Board& board) = 0;
};
class Player : public PlayerBase {
public:
    Player(char get, bool iscomputer);
    bool playermove(Board& board) override;
};
class Game {
private:
    Board board;
    PlayerBase* p1;
    PlayerBase* p2;
    PlayerBase* cp;
public:
    Game(int gm);
    ~Game();
    void sp();
    void play(); 
    bool check_game_over();
    void declare_winner();
};
