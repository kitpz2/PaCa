#ifndef POLE_H
#define POLE_H
#include "globals.h"
#include <iostream>
using namespace std;
//SDL_Surface *ciemna_woda, *woda, *strzal;
class Pole
{
    public:
        /** Domyślny konstruktor */
        Pole(int x=0, int y=0, char typ=0);
        ~Pole();
        /**Obsługa zdarzeń
        \param offset przesinięcia mapy
        \return co się wydarzyło*/
        int handle_events(SDL_Event&, int, int);
        /**Pobiera typ danego pola
        \return referencja do typu pola*/
        inline char &Get(){return typ;};
        /**Ustawia dane dla danego pola
        \param nx pozycja x na której stać ma dane pole
        \param ny pozycja y na której stać ma dane pole
        \param ntyp tym danego pola
        */
        void Set(int nx, int ny, char ntyp=0){polozenie.x=nx*ROZMIAR_X;polozenie.y=ny*ROZMIAR_Y;typ=ntyp;};
        /**Ustawia tylko typ pola
        \param ntym typ danego pola*/
        void Set(char ntyp){typ=ntyp;};

    private:
        /**pozycja x i y pola*/
        int x,y;
        /**typ pola*/
        char typ;
        /**Zmienna pomocnicza potrzebna przy wyliczaniu kolizji z myszką*/
        SDL_Rect polozenie;

};

#endif // POLE_H
