#ifndef GAME_H
#define GAME_H
#include "window.h"

class Game
{
public:
    Game();
    ~Game();
    void run();
    void end();

private:
    Window *wnd = nullptr;
};

#endif // GAME_H
