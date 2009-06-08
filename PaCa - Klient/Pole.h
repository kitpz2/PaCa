#ifndef POLE_H
#define POLE_H
#include "globals.h"
#include <iostream>
using namespace std;
//SDL_Surface *ciemna_woda, *woda, *strzal;
class Pole
{
    public:
        /** Default constructor */
        Pole(int x=0, int y=0, char typ=0);
        /** Default destructor */
        ~Pole();
        /**Obsługa zdarzeń
        \param offset przesinięcia mapy*/
        int handle_events(SDL_Event&, int, int);
        inline char &Get(){return typ;};
        void Set(int nx, int ny, char ntyp=0){polozenie.x=nx*ROZMIAR_X;polozenie.y=ny*ROZMIAR_Y;typ=ntyp;};
        void Set(char ntyp){typ=ntyp;};

    private:
        int x,y;
        char typ;
        SDL_Rect polozenie;

};

#endif // POLE_H
