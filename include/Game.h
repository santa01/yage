#ifndef GAME_H
#define	GAME_H

#include "Renderer.h"

class Game {
    public:
        Game();

        void start();

    private:
        Renderer renderer;

        Game(const Game&);
        Game& operator =(const Game&);
};

#endif	/* GAME_H */
