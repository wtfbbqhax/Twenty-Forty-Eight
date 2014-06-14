#ifndef BBQBOT_H_VQM2CU79
#define BBQBOT_H_VQM2CU79

#include "game.h"

class BruteForce : public Observer {
public:
    virtual void update()
    {
        // identify matching patterns
        // select pattern for desired outcome
        // select pattern least likely to lead to bad outcomes after serveral moves
        // weigh against likelyhood of undesired random generation (probability)
        game->move();
    }

    BruteForce(Game *g): game(g), Observer(g) {}
    virtual ~Observer() {}
    virtual void reset() {}
private:
    Game *game;
};

#endif /* BBQBOT_H_VQM2CU79 */
