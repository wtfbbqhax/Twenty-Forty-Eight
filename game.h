#ifndef GAME_H_W7KDWYQ4
#define GAME_H_W7KDWYQ4

#include <iomanip>
//#include <ordered_set>
#include <list>
#include <vector>
#include <array>
#include <stdint.h>

using namespace std;

enum Move { Up, Down, Left, Right };

typedef uint16_t            Tile;
typedef array<Tile,4>       GridLine;
typedef array<GridLine,4>   _Grid;

typedef pair<int,int>       _Coord;
typedef vector<_Coord>      CoordList;

const list<Tile> tile_values = {
      0,     2,     4,     8,    16,    32,    64,   128,
    256,   512,  1024,  2048,  4096,  8192, 16384, 32768,
};


struct Grid : _Grid {
    Grid()
    {
        clear();
    }

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

    CoordList findAll(Tile knee)
    {
        auto *me = data();
        CoordList list;

        for (int x = 0; x < size(); x++)
        for (int y = 0; y < size(); y++)
            if (me[x][y] == knee)
              list.push_back(make_pair(x,y));

        return list;
    }

    bool isFull()
    {
        for (auto line = begin(); line != end(); line++) {
            for (auto tile: *line) {
                if (!tile)
                    return false;
            }
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

    void reset()
    {
        _grid.clear();
        _score = 0;
        gameover = false;
        addRandomTile();
        addRandomTile();
        notifyReset();
    } 

    Grid grid() { return _grid; } 
    unsigned score() { return _score; }
    bool gameOver() { return gameover; }
    void move(Move m);
    bool addRandomTile();
    bool addRandomTile(Tile val);

    void notify();
    void notifyReset();
    void attach(Observer *obs) { views.push_back(obs); }

    void setGameState(Grid &g, unsigned s)
    {
        _grid = g;
        _score = s;
        gameover = false; 
        notify();
    }
    
#ifdef DEBUG_CMDS
    void insert(int v)
    {
        addRandomTile(v);
        notify();
    }
#endif
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
    virtual void reset() = 0;
};

#endif /* GAME_H_W7KDWYQ4 */
