#ifndef GAME_H
#define GAME_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define INCLUDE_SDL

#include <string>
#include "SDL_include.h"
#include "State.hpp"

class Game {
    public:
        ~Game();

        void Run();

        SDL_Renderer* GetRenderer();

        State& GetState();

        static Game& GetInstance();        

    private:
        Game(std::string title, int width, int height);

        static Game* instance;

        SDL_Window* window;

        SDL_Renderer* renderer;

        State* state;
};

#endif