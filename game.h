#include <stdint.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>

using namespace std;

enum Move { Up, Down, Left, Right };

typedef uint16_t            Tile;
typedef array<Tile,4>       GridLine;
typedef array<GridLine,4>   _Grid;

struct Grid : _Grid {
    typedef pair<int,int> grid_iterator;

    Grid() { clear(); }

    void clear()
    {
        GridLine _nil = {{0,0,0,0}};
        fill(_nil);
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
