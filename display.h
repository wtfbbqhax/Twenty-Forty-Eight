#include <vector>

#include "game.h"

class Display: public Observer {
    vector<string> console; 

    void clearScreen(void);
    void drawWaterfall(void);

public:

    Display(Game *g): Observer(g) { }
    void Console(void);
    void reset(void);
    void update(void);
};

