#include <vector>
#include <random>

#include "game.h"

class Display: public Observer {
public:
    Display(Game *g) :
        Observer(g),
        rng(std::random_device{}())
    {
    }

    void Console(void);
    void reset(void);
    void update(void);

private:
    void clearScreen(void);
    void drawWaterfall(void);

    vector<string> console; 
    std::mt19937 rng;
};

