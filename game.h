#include <stdint.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>

using namespace std;

enum class Move { Up, Down, Left, Right };
#define Up      Move::Up
#define Down    Move::Down
#define Left    Move::Left
#define Right   Move::Right

typedef uint16_t            Tile;
typedef array<Tile,4>       GridLine;
typedef array<GridLine,4>   _Grid;

typedef pair<int,int>       _Coord;
typedef vector<_Coord>      CoordList;

struct Grid : _Grid {

    Grid() { clear(); }

    void clear()
    {
        GridLine _nil = {{0,0,0,0}};
        fill(_nil);
    }

    void set(_Coord xy, Tile t) 
    {
        int x=get<0>(xy);
        int y=get<1>(xy);
        auto * me = data();
        me[x][y] = t;
    }

    bool isFull()
    {
        for ( auto ln = begin(); ln != end(); ln++ )
        {
            for ( auto ti : *ln )
                if ( ti == 0 )
                    return false;
        }
        return true;
    }
};

class Game {
    typedef vector< class Observer * > observer_type;
    observer_type views;
    Grid _grid;
    unsigned _score;
    bool gameover;
public:
    Game() { reset(); }

    void reset() {
        gameover = false;
        _grid.clear();
        _score = 0;
        addRandomTile();
        addRandomTile();
        notify();
    } 

    Grid grid() { return _grid; } 
    unsigned score() { return _score; }
    bool gameOver() { return gameover; }
    void move(Move m); // { notify(); };
    bool addRandomTile();
    bool addRandomTile(Tile val);
    void notify();
    void attach(Observer *obs) { views.push_back(obs); }
};

class Observer {
    Game *_model;
protected:
    Game* model() { return _model; }
public:
    Observer(Game *mod) {
        _model=mod;
        _model->attach(this);
    }
    virtual ~Observer() { }
    virtual void update() = 0;
};

class Display: public Observer {
public:
    Display(Game *g): Observer(g) { }
    void update();
};
