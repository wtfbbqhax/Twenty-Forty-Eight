#ifndef DEBUG_H_ZQKVK7TG
#define DEBUG_H_ZQKVK7TG
#include "game.h"

class DebugDisplay: public Observer {
public:
    DebugDisplay(Game *g): Observer(g) { }

    void reset() {}

    void update()
    {
        Game *game = this->model();
        Grid grid = game->grid();

        cout << "   Score: " << game->score() << endl;

        for (int x=0; x < grid.size(); x++)
        {
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
    }
};

#endif /* DEBUG_H_ZQKVK7TG */
