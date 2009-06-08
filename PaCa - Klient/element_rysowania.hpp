#ifndef ELEMENT_RYSOWANIA_H
#define ELEMENT_RYSOWANIA_H

#include "globals.h"
class element_rysowania
{
    public:
        /** Default constructor */
        element_rysowania(int x, int y, SDL_Surface* obraz):x(x),y(y),obraz(obraz){};
        element_rysowania();
        /** Default destructor */
        //~element_rysowania();
        int GetX(){return x;};
        int GetY(){return y;};
        SDL_Surface *GetObraz(){return obraz;};

    protected:
    private:
    int x, y;
    SDL_Surface* obraz;
};

#endif // ELEMENT_RYSOWANIA_H
