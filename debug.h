#ifndef DEBUG_H_ZQKVK7TG
#define DEBUG_H_ZQKVK7TG

#include "cvar.h"
#include "game.h"

extern cvar_t * g_cheats;
extern cvar_t * g_drawResetAnim;

class Undo: public Observer {
    struct Record {
        Grid grid; unsigned score;
        Record(Grid _g, unsigned _s): grid(_g), score(_s) {};
    };
    vector<Record> history;
public:
    Undo(Game *g): Observer(g) { }
    void update()
    {
        Game *game=this->model();
        Record rec = { game->grid(), game->score() };
        history.push_back(rec);
    }

    void reset()
    {
        history.clear();
    }

    void undo()
    {
        if ( history.size() )
        {
            Game *game=this->model();

            history.pop_back();
            Record rec = history.back();
            history.pop_back();

            game->setGameState(rec.grid, rec.score);
        }
    }
};

class DebugDisplay: public Observer {
public:
    DebugDisplay(Game *g): Observer(g) { }

    void reset() 
    {
        cout << "[!] RESET" << endl;
    }

    void update()
    {
        Game *game = this->model();
        Grid grid = game->grid();

        cout << "   Score: " << game->score() << endl;

        cout << "    Y=      0    1    2    3" << endl << endl;
        for (int x=0; x < grid.size(); x++)
        {
            cout << "   x=" << x << "  ";
            for (int y=0; y < grid.size(); y++)
            {
                Tile tile = grid[x][y];

                if (!tile)
                {
                    cout << "    .";
                }
                else
                {
                    cout << setw(5) << tile;
                }
            }
            cout << endl;
        }

        cout << endl << "!! DEBUG DATA" << endl;
        for ( auto value : tile_values )
        {

#define DbgTag "?¿"

            CoordList all = grid.findAll(value);
            if (all.size()) {
                cout << DbgTag "findAll(" << value << ")=";
                for (auto xy : all)
                {
                    int x = get<0>(xy);
                    int y = get<1>(xy);

                    cout << "(" << x << ","  << y << ") ";
                }
                cout << endl;
            }
        }

        cout <<  "g_cheats "
             <<  Cvar_GetStringValue(g_cheats) <<  endl
             <<  "g_drawResetAnim "
             <<  Cvar_GetStringValue(g_drawResetAnim) <<  endl;
    }
};

#endif /* DEBUG_H_ZQKVK7TG */
